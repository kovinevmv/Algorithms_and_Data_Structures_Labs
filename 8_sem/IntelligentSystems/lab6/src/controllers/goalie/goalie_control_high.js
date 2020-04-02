const CTRL_HIGH = {
    execute(input) {
        if (this.last === "catch") return this.afterCatch(input);
        const immediate = this.immediateReaction(input);
        if (immediate) return immediate;
        const defend = this.defendGoal(input);
        if (defend) return defend;
        if (this.last === "defend")
            input.newAction = "return";
        this.last = "previous"
    },
    immediateReaction(input) {
        if (input.canCatch) {
            this.last = "catch";
            return {n: "catch", v: input.ball.angle}
        }
        if (input.canKick) {
            this.last = "kick";
            if (input.goalOther) {
                if (input.goalOther.dist > 40)
                    return {n: "kick", v: `30 ${input.goalOther.angle}`};
                return {n: "kick", v: `110 ${input.goalOther.angle}`}
            }
            return {n: "kick", v: `10 45`}
        }
    },

    afterCatch(input) {
        if (input.goalOther) {
            this.last = "kick";
            return {n: "kick", v: `110 ${input.goalOther.angle}`}
        } else {
            return {n: "turn", v: '60'}
        }
    },

    defendGoal(input) {
        if (input.ball) {
            const close = input.closest("b");
            if ((close[0] && close[0].dist + 1 > input.ball.dist)
                || (input.ball.dist < 20 && input.ball.dist > 1.2)) {
                this.last = "defend";
                if (Math.abs(input.ball.angle) > 5)
                    return {n: "turn", v: input.ball.angle};
                if (input.ball.dist > 1)
                    return {n: "dash", v: 110};
                else
                    return {n: "dash", v: 30}
            }
        }
    },
};
module.exports = CTRL_HIGH;
