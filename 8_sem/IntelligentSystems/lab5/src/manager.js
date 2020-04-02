const Flatten = require('@flatten-js/core');
const _ = require('lodash');
const {align, mod, limit} = require('./util/utils');
const {Flags} = require('./locator');

class Manager {
    constructor() {
        this.data = null
    }

    knowMyself() {
        return !(_.isNil(this.data.angle) || _.isNaN(this.data.angle))
    }

    isBallVisible() {
        return !_.isNil(this.data.ball)
    }

    playersVisible() {
        return !_.isEmpty(this.data.players) ? this.data.players.length : 0
    }

    checkHear(heard, who, msg) {
        return !_.isNil(heard) && heard.who === who && who.msg.startsWith(msg)
    }

    half(point) {
        return point.x > 0 ? 'r' : 'l'
    }

    closestPlayerToTarget(point, team) {
        var dist = Infinity;
        var min = null;
        for (const player of this.data.players) {
            if (player.team !== team || _.isNil(player.team)) {
                continue
            }
            const {length} = this.vectorFromTo(player, point);
            if (length < dist) {
                dist = length;
                min = player
            }
        }
        return {
            dist,
            player: min,
        }
    }

    movePointByVector(point, target, angle, distance) {
        const vector = Flatten.vector(
            Flatten.point(point.x, point.y),
            Flatten.point(target.x, target.y)
        )
            .rotate((angle / Math.PI) * 180)
            .normalize()
            .multiply(distance);
        const a = {
            x: point.x + vector.x,
            y: point.y + vector.y,
        };
        const b = {
            x: target.x + vector.x,
            y: target.y + vector.y
        };
        return {a, b}
    }

    transformPoint(point, player, angle) {
        let matrix = new Flatten.Matrix();
        matrix = matrix.translate(-player.x, -player.y);
        matrix = matrix.rotate(angle / 180 * Math.PI);
        return Flatten.point(point.x, point.y).transform(matrix)
    }

    flag(flag) {
        if (_.isString(flag)) {
            return Flags[flag]
        }
        return flag
    }

    vectorFromTo(point1, point2) {
        const vector = Flatten.vector(
            Flatten.point(point1.x, point1.y),
            Flatten.point(point2.x, point2.y)
        );
        const angle = mod((vector.slope / Math.PI) * 180, 360);
        return {length: vector.length, angle}
    }

    vectorTo(point) {
        return this.vectorFromTo(this.data.self, point)
    }

    angleTo(angle, limit_) {
        const delta = mod(this.data.angle - angle, 360);
        if (limit_) {
            return align(limit(delta, limit_))
        }
        return align(delta)
    }

    get_slowdown(v_start, v_end, dist, approach_dist) {
        if (dist >= approach_dist) {
            return v_start
        }
        return (
            v_end + (1 - (approach_dist - dist) / approach_dist) * (v_start - v_end)
        )
    }
}

module.exports = {Manager};
