const CTRL_HIGH = {
    execute(input) {
        const immediate = this.immediateReaction(input);
        if (immediate) return immediate;
        const defend = this.defendGoal(input);
        if (defend) return defend;
        if (this.last == "defend")
            input.newAction = "return";
        this.last = "previous"
    },
    immediateReaction(input) {
        if (input.canKick) {
            this.last = "kick";
            if (input.id < 8) {
                if (input.playersListMy.length && input.id > 3) {
                    input.newAction = "return";
                    input.playersListMy.sort((p1, p2) => {
                        return p1.p[1] - p2.p[2]
                    });
                    if ((!input.goalOther || input.playersListMy[0].p[1] < input.goalOther.dist - 15)
                        && input.playersListMy[0].p[1] > 4 && (!input.goalOwn || input.goalOwn.dist > 25))
                        return {
                            n: "kick",
                            v: `${input.playersListMy[0].p[1] * 2} ${input.playersListMy[0].p[0]}`,
                            isSave: false
                        }
                }
                if (input.goalOther) {
                    if (input.goalOther.dist > 40)
                        return {n: "kick", v: `30 ${input.goalOther.angle}`, isSave: false};
                    return {n: "kick", v: `110 ${input.goalOther.angle}`, isSave: false}
                }
            } else {
                input.newAction = "return";
                const topFlag = (input.side === 'l') ? 'frt' : 'flt';
                const botFlag = (input.side === 'l') ? 'frb' : 'flb';
                if (input.goalOther) return {n: "kick", v: `80 ${input.goalOther.angle}`};
                else if (input.flags[topFlag]) return {n: "kick", v: `80 ${input.flags[topFlag].angle}`};
                else if (input.flags[botFlag]) return {n: "kick", v: `80 ${input.flags[botFlag].angle}`}
            }
            return {n: "kick", v: `10 45`, isSave: false, isSave: false}
        }
    },
    defendGoal(input) {
        if (input.ball) {
            const close = input.closest(true);
            if ((close[0] && close[0].dist > input.ball.dist)
                || !close[0] || (close[1] && close[1].dist > input.ball.dist)
                || !close[1]) {
                this.last = "defend";
                if (input.id < 4 && input.goalOwn && input.goalOwn.dist < 50) {
                    input.newAction = "return"
                } else if (input.id > 7 && input.goalOther && input.goalOther.dist < 50) {
                    input.newAction = "return"
                } else if (input.id > 3 && input.id < 8 && input.goalOwn && input.goalOwn.dist < 25) {
                    input.newAction = "return"
                } else {
                    if (Math.abs(input.ball.angle) > 5)
                        return {n: "turn", v: input.ball.angle, isSave: true};
                    if (input.ball.dist > 1)
                        return {n: "dash", v: 110, isSave: true};
                    else
                        return {n: "dash", v: 30, isSave: true}
                }
            }
        }
    },
};
module.exports = CTRL_HIGH;
