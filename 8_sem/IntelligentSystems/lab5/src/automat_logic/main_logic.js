const _ = require('lodash');

const actions = {
    isBallVisible() {
        return this.ta.mgr.isBallVisible()
    },
    runToTarget() {
        this.ta.next = !_.isNil(this.ta.local.setNext)
            ? this.ta.local.setNext
            : false;
        if (!this.ta.local.target) {
            this.ta.next = true;
            return false
        }
        const {angle, length} = this.ta.mgr.vectorTo(this.ta.local.target);
        if (length <= (this.ta.local.minLen || this.ta.s.run.minLen)) {
            this.ta.next = true;
            this.ta.local.target = null;
            return false
        }
        if (Math.abs(this.ta.data.angle - angle) >= this.ta.s.run.minAngle) {
            const delta = this.ta.mgr.angleTo(angle);
            return this.ta.make_msg('turn', delta)
        }
        return this.ta.make_msg(
            'dash',
            this.ta.local.speed || this.ta.s.run.normal
        )
    },
    lookForBall() {
        this.ta.next = false;
        if (this.ta.mgr.isBallVisible()) {
            this.ta.next = true;
            return
        }
        return this.ta.make_msg('turn', 60)
    },
};

module.exports = {actions};
