const {Edge, Node} = require('./automat/Automat');
const {Flags} = require('../locator.js');
const {actions: commonActions} = require('./main_logic');
const _ = require('lodash');

const nodes = {
    start: Node.extend({
        edges: [
            Edge.extend({
                to: 'far',
                sync: 'isBallFar?',
                assign() {
                    this.ta.timers.t = 0
                },
            }),
            Edge.extend({
                to: 'near',
                sync: 'isBallNear?',
                assign() {
                    this.ta.timers.t = 0
                },
            }),
            Edge.extend({
                to: 'close',
                sync: 'isBallClose?',
                assign() {
                    this.ta.timers.t = 0
                },
            }),
        ],
    }),
    far: Node.extend({
        edges: [
            Edge.extend({
                to: 'near',
                sync: 'isBallNear?',
                assign() {
                    this.ta.timers.t = 0
                },
            }),
            Edge.extend({
                to: 'close',
                sync: 'isBallClose?',
                assign() {
                    this.ta.timers.t = 0
                },
            }),
            Edge.extend({
                to: 'returnToPost',
                sync: 'isReturnNeeded?',
                assign() {
                    this.ta.timers.t = 0
                },
            }),
            Edge.extend({
                to: 'lookAtBall',
                sync: 'isBallVisible?',
            }),
        ],
        action: 'lookForBall',
    }),
    near: Node.extend({
        edges: [
            Edge.extend({
                to: 'far',
                sync: 'isBallFar?',
                assign() {
                    this.ta.timers.t = 0
                },
            }),
            Edge.extend({
                to: 'close',
                sync: 'isBallClose?',
                assign() {
                    this.ta.timers.t = 0;
                    this.ta.local.setNext = null
                },
            }),
        ],
        action: 'runToBall',
    }),
    close: Node.extend({
        edges: [
            Edge.extend({
                to: 'far',
                sync: 'isBallFar?',
                assign() {
                    this.ta.timers.t = 0
                },
            }),
            Edge.extend({
                to: 'near',
                sync: 'isBallNear?',
                assign() {
                    this.ta.timers.t = 0
                },
            }),
            Edge.extend({
                to: 'catch',
                assign() {
                    this.ta.timers.catch = 0
                },
            }),
        ],
    }),
    catch: Node.extend({
        edges: [
            Edge.extend({
                to: 'far',
                sync: 'isBallFar?',
                assign() {
                    this.ta.timers.t = 0
                },
            }),
            Edge.extend({
                to: 'kick',
                guard() {
                    return (
                        this.ta.timers.catch > 5 ||
                        !this.ta.data.body.catch ||
                        this.ta.mgr.checkHear(
                            this.ta.local.heardNow,
                            'referee',
                            'goalie_catch_ball'
                        )
                    )
                },
                sync: 'kickBall!',
            }),
            Edge.extend({
                to: 'start',
                guard() {
                    return this.ta.mgr.checkHear(
                        this.ta.local.heardNow,
                        'referee',
                        'free_kick'
                    )
                },
            }),
        ],
        action: 'catchBall',
    }),
    runToBall: Node.extend({
        edges: [
            Edge.extend({
                to: 'far',
                sync: 'isBallFar?',
                assign() {
                    this.ta.timers.t = 0
                },
            }),
            Edge.extend({
                to: 'start',
                sync: 'runToTarget!',
            }),
        ],
    }),
    kick: Node.extend({
        edges: [
            Edge.extend({
                to: 'far',
                sync: 'isBallFar?',
                assign() {
                    this.ta.timers.t = 0
                },
            }),
            Edge.extend({
                to: 'returnToPost',
            })
        ]
    }),
    returnToPost: Node.extend({
        edges: [
            Edge.extend({
                to: 'start',
                sync: 'runToTarget!',
            }),
        ],
    }),
    lookAtBall: Node.extend({
        edges: [
            Edge.extend({
                to: 'far',
                sync: 'lookAtBall!',
            }),
        ],
    }),
};

const actions = {
    ...commonActions,
    isBallFar() {
        return !this.ta.mgr.isBallVisible() || this.ta.data.ball.d >= this.ta.s.dist.far
    },
    catchBall() {
        console.log("Try to catch");
        this.ta.next = true;
        return this.ta.make_msg('catch', this.ta.data.ball.a)
    },
    kickBall() {
        console.log("Kick ball");

        this.ta.next = true;
        const {angle} = this.ta.mgr.vectorTo(this.ta.local.oppositeGoal);
        const delta = this.ta.mgr.angleTo(angle);
        return this.ta.make_msg('kick', 100, delta)
    },
    isBallNear() {
        return (
            this.ta.data.ball &&
            this.ta.data.ball.d < this.ta.s.dist.far &&
            this.ta.data.ball.d > this.ta.s.dist.close
        )
    },
    isBallClose() {
        return this.ta.data.ball && this.ta.data.ball.d <= this.ta.s.dist.close
    },
    isReturnNeeded() {
        const {length} = this.ta.mgr.vectorTo(this.ta.local.ownGoal);
        if (length > this.ta.s.run.minLen) {
            this.ta.local.target = this.ta.local.ownGoal;
            this.ta.local.setNext = null;
            return true
        }
        return false
    },
    runToBall() {
        this.ta.local.target = this.ta.data.ball;
        this.ta.local.setNext = true;
        return this.ta.actions.runToTarget()
    },
    lookAtBall() {
        const {angle} = this.ta.mgr.vectorTo(this.ta.data.ball);
        const delta = this.ta.mgr.angleTo(angle);
        this.ta.next = true;
        return this.ta.make_msg('turn', delta)
    },
    empty() {
        this.ta.next = true;
        return null
    },
    wait() {
        this.ta.next = true;
        return true
    },
};

const settings = {
    dist: {
        far: 20,
        close: 1,
    },
    run: {
        normal: 100,
        minAngle: 5,
        minLen: 1,
    },
    catch: {
        max_catch: 3,
    },
};

const automaton = {
    nodes,
    actions,
    timers: {t: 0, catch: null},
    start: 'start',
    settings,
    init() {
        if (this.agent.position === 'l') {
            this.local.ownGoal = Flags['gl'];
            this.local.oppositeGoal = Flags['gr']
        } else {
            this.local.ownGoal = Flags['gr'];
            this.local.oppositeGoal = Flags['gl']
        }
    },
    beforeExec() {
        this.local.heardNow = null;
        if (!_.isEmpty(this.data.hear)) {
            const last = this.data.hear[this.data.hear.length - 1];
            if (last.time === this.time) {
                this.local.heardNow = last
            }
        }
    },
};

module.exports = {nodes, actions, automaton};
