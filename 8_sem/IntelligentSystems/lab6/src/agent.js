const Msg = require('./msg');
const CtrlHighPlayer = require('./controllers/player/player_control_high');
const CtrlMiddlePlayer = require('./controllers/player/player_control_middle');
const CtrlLowPlayer = require('./controllers/player/player_control_low');
const CtrlHighGoalie = require('./controllers/goalie/goalie_control_high');
const CtrlMiddleGoalie = require('./controllers/goalie/goalie_control_middle');
const CtrlLowGoalie = require('./controllers/goalie/goalie_control_low');
const {teamA_config, teamB_config} = require('./config/team_config');

class Agent {
    constructor(speed, teamName, isGoalie) {
        this.position = "l"; // По умолчанию - левая половина поля
        this.speed = speed;
        this.run = false; // Игра начата
        this.teamName = teamName;
        this.isGoalie = isGoalie;
        this.angle = 0;
        this.steps = []
    }


    msgGot(msg) {
        let data = msg.toString('utf8');
        this.processMsg(data);
        this.sendCmd()
    }

    setSocket(socket) {
        this.socket = socket
    }

    socketSend(cmd, value) {
        this.socket.sendMsg(`(${cmd} ${value})`)
    }

    processMsg(msg) {
        let data = Msg.parseMsg(msg);
        if (!data) throw new Error("Parse error\n" + msg);
        if (data.cmd === "hear") {
            if (data.msg.includes('play_on') || data.msg.includes('kick_off_'))
                this.run = true
        }
        if (data.cmd === "init") this.initAgent(data.p);
        this.analyzeEnv(data.msg, data.cmd, data.p)
    }


    initAgent(p) {
        if (p[0] === "r") this.position = "r";
        this.id = p[1]
    }

    analyzeEnv(msg, cmd, p) {
        if (cmd === 'hear' && (p[2].includes('goal_l_') || p[2].includes('goal_r_'))) {
            let position = null;
            if (this.teamName === 'A') position = teamA_config.filter(player => player.id === this.id)[0].position;
            if (this.teamName === 'B') position = teamA_config.filter(player => player.id === this.id)[0].position;

            this.act = {n: 'move', v: `${position.x} ${position.y}`};
        }
        if (cmd === 'see' && this.run) {
            if (!this.isGoalie) {
                var act = CtrlLowPlayer.execute(p, [CtrlMiddlePlayer, CtrlHighPlayer], this.teamName, this.position, this.id, this.steps);
                if (act.isSave === true) {
                    this.act = {n: act.n, v: act.v};
                    this.steps.push(this.act)
                } else if (act.isSave === -1) {
                    this.steps.pop()
                } else {
                    this.act = act
                }
            } else {
                var act1 = CtrlLowGoalie.execute(p, [CtrlMiddleGoalie, CtrlHighGoalie], this.teamName, this.position, this.id, this.steps);
                if (act1.isSave === true) {
                    this.act = {n: act1.n, v: act1.v};
                    this.steps.push(this.act)
                } else if (act1.isSave === -1) {
                    this.steps.pop()
                } else {
                    this.act = act1
                }
            }

        }


    }

    sendCmd() {
        if (this.run) {
            if (this.act) {
                if (this.act.n !== "move")
                    this.socketSend(this.act.n, this.act.v);
                else {
                    this.socketSend(this.act.n, this.act.v);
                    this.run = false
                }
            }
            this.act = null
        }
    }
}

module.exports = Agent;
