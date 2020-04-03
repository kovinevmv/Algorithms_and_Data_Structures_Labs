const {Edge, Node} = require('./automat/Automat');
const {Flags} = require('../locator.js');
const {actions: commonActions} = require('./main_logic');
const _ = require('lodash');

const nodes = {
    start: Node.extend({
        edges: [
            Edge.extend({
                to: 'locateBall',
                sync: 'resetLocal!',
            }),
        ],
    }),
    locateBall: Node.extend({
        edges: [
            Edge.extend({
                to: 'seeBall',
                sync: 'lookForBall!',
            }),
        ],
    }),
    seeBall: Node.extend({
        edges: [
            Edge.extend({
                to: 'kickBall',
                sync: 'canKick?',
            }),
            Edge.extend({
                to: 'runWithSlowdown',
                sync: 'shouldRunWithSlowdown?',
            }),
            Edge.extend({
                to: 'runFast',
                sync: 'shouldRunFast?',
            }),
            Edge.extend({
                to: 'returnToPost',
                sync: 'shouldReturnToPost?',
            }),
        ],
    }),
    runWithSlowdown: Node.extend({
        edges: [
            Edge.extend({
                to: 'kickBall',
                sync: 'canKick?',
            }),
            Edge.extend({
                to: 'runFast',
                sync: 'shouldRunFast?',
            }),
            Edge.extend({
                to: 'returnToPost',
                sync: 'shouldReturnToPost?',
            }),
        ],
        action: 'runWithSlowdown',
    }),
    runFast: Node.extend({
        edges: [
            Edge.extend({
                to: 'kickBall',
                sync: 'canKick?',
            }),
            Edge.extend({
                to: 'runWithSlowdown',
                sync: 'shouldRunWithSlowdown?',
            }),
            Edge.extend({
                to: 'returnToPost',
                sync: 'shouldReturnToPost?',
            }),
        ],
        action: 'runFast',
    }),
    returnToPost: Node.extend({
        edges: [
            Edge.extend({
                to: 'start',
                sync: 'returnToPost!',
            }),
        ],
    }),
    kickBall: Node.extend({
        edges: [
            Edge.extend({
                to: 'dribbleKick',
                sync: 'shouldDribble?',
            }),
            Edge.extend({
                to: 'returnToPost',
                sync: 'strongKick!',
            }),
        ],
    }),
    dribbleKick: Node.extend({
        edges: [
            Edge.extend({
                to: 'runWithSlowdown',
                sync: 'dribbleKick!',
            }),
        ],
    }),
};

const excludeReturn = ['start', 'locateBall', 'returnToPost'];

for (const [, node] of Object.entries(nodes).filter(
    ([name]) => !excludeReturn.includes(name)
)) {
    node.edges.unshift(
        Edge.extend({
            to: 'start',
            sync: 'isBallNotVisible?',
        })
    )
}

const actions = {
    ...commonActions,
    resetLocal() {
        this.ta.local.speed = null;
        this.ta.local.target = null;
        this.ta.local.setNext = null;
        this.ta.next = true
    },
    isBallNotVisible() {
        return !this.ta.mgr.isBallVisible()
    },
    canKick() {
        return this.ta.local.vectorToBall.length <= this.ta.s.kick.minDist
    },
    shouldRunWithSlowdown() {
        return (
            _.isNil(this.ta.local.vectorFromBallToGoalie) ||
            this.ta.local.vectorToBall.length <
            this.ta.local.vectorFromBallToGoalie.length * 1.5
        )
    },
    shouldRunFast() {
        return (
            !_.isNil(this.ta.local.vectorFromBallToGoalie) &&
            this.ta.local.vectorToBall.length >=
            this.ta.local.vectorFromBallToGoalie.length * 1.5 &&
            this.ta.local.vectorToBall.length <=
            this.ta.local.vectorFromBallToGoalie.length * 0.9
        )
    },
    shouldReturnToPost() {
        return (
            !_.isNil(this.ta.local.vectorFromBallToGoalie) &&
            this.ta.local.vectorToBall.length >=
            this.ta.local.vectorFromBallToGoalie.length * 0.9
        )
    },
    runWithSlowdown() {
        this.ta.local.target = this.ta.data.ball;
        this.ta.local.setNext = true;
        var l =  (this.ta.local.vectorToBall && this.ta.local.vectorToBall.length) ? this.ta.local.vectorToBall.length: 1;
        this.ta.local.speed = this.ta.mgr.get_slowdown(
            this.ta.s.slowdown.startSpeed,
            this.ta.s.slowdown.endSpeed,
            l,
            this.ta.s.slowdown.start
        );
        return this.ta.actions.runToTarget()
    },
    runFast() {
        this.ta.local.target = this.ta.data.ball;
        this.ta.local.setNext = true;
        this.ta.local.spped = this.ta.s.run.fast;
        return this.ta.actions.runToTarget()
    },
    returnToPost() {
        this.ta.local.target = this.ta.s.post;
        this.ta.local.setNext = null;
        this.ta.local.speed = null;
        return this.ta.actions.runToTarget()
    },
    shouldDribble() {
        if (
            !_.isNil(this.ta.vectorFromBallToGoalie) &&
            this.ta.vectorFromBallToGoalie.length < this.ta.s.kick.safeGoalieDist
        ) {
            return false
        }
        return (
            this.ta.mgr.angleTo(this.ta.local.vectorToKickTarget.angle) >=
            this.ta.s.kick.minAngle ||
            this.ta.local.vectorToKickTarget.length >= this.ta.s.kick.minDistToTarget
        )
    },
    dribbleKick() {
        const delta = this.ta.mgr.angleTo(
            this.ta.local.vectorToKickTarget.angle,
            this.ta.s.kick.minAngle
        );
        this.ta.next = true;
        return this.ta.make_msg('kick', this.ta.s.kick.dribbleForce, delta)
    },
    strongKick() {
        const delta = this.ta.mgr.angleTo(this.ta.local.vectorToKickTarget.angle);
        this.ta.next = true;
        return this.ta.make_msg('kick', this.ta.s.kick.normalForce, delta)
    },
};


const {settings} = require('./settings/player_pref');
const automaton = {
    nodes,
    actions,
    timers: {t: 0},
    start: 'start',
    settings,
    init() {
        if (this.agent.position === 'l') {
            this.local.ownGoal = Flags['gl'];
            this.local.oppositeGoal = Flags['gr'];
            this.settings.post.x = -this.settings.post.x;
            this.settings.post.y = -this.settings.post.y
        } else {
            this.local.ownGoal = Flags['gr'];
            this.local.oppositeGoal = Flags['gl']
        }
        this.local.kickTarget = this.local.oppositeGoal
    },
    beforeExec() {
        this.local.heardNow = null;
        if (!_.isEmpty(this.data.hear)) {
            const last = this.data.hear[this.data.hear.length - 1];
            if (last.time === this.time) {
                this.local.heardNow = last
            }
        }
        this.local.vectorToBall = null;
        this.local.vectorToGoalie = null;
        this.local.vectorFromBallToGoalie = null;
        if (!this.mgr.knowMyself()) {
            return
        }
        this.local.vectorToKickTarget = this.mgr.vectorTo(this.local.kickTarget);
        if (this.mgr.isBallVisible()) {
            this.local.vectorToBall = this.mgr.vectorTo(this.data.ball)
        }
        const goalie = this.data.players.find(
            (player) => player.team !== this.agent.teamName && player.goalie
        );
        if (!_.isNil(goalie)) {
            this.local.vectorToGoalie = this.mgr.vectorTo(goalie);
            if (this.mgr.isBallVisible()) {
                this.local.vectorFromBallToGoalie = this.mgr.vectorFromTo(
                    goalie,
                    this.data.ball
                )
            }
        }
    },
};

module.exports = {nodes, actions, automaton};
