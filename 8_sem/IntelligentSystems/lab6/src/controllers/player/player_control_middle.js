const CTRL_MIDDLE = {
    action: "return",
    execute(input, controllers) {
        const next = controllers[0];
        switch (this.action) {
            case "return":
                input.cmd = this.actionReturnArrack(input);
                break;
            case "seekBall":
                input.cmd = this.seekBall(input);
                break
        }
        input.action = this.action;
        if (next) {
            const command = next.execute(input, controllers.slice(1));
            if (command) return command;
            if (input.newAction) this.action = input.newAction;
            return input.cmd
        }
    },
    actionReturnArrack(input) {
        if (input.steps.length) {
            step = input.steps.pop();
            console.log(step);
            if (step) {
                if (step.n === "turn") {
                    return {n: step.n, v: 180 - step.v, isSave: -1}
                }
                if (step.n === "dash") {
                    return {n: step.n, v: step.v, isSave: -1}
                }
            }
        }
        return {n: "turn", v: 45, isSave: true};
    },
    seekBall(input) {
        if (!input.ball) {
            this.action = "actionReturnArrack";
            return {n: "turn", v: 45, isSave: true};
        }
        else {
            return {n: "turn", v: input.ball.angle, isSave: true};
        }
    },
};
module.exports = CTRL_MIDDLE;