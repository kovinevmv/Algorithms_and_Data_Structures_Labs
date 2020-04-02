const Msg = require('./msg');
const locator = require('./locator');
const _ = require('lodash');


class Agent {
    constructor(automata) {
        this.position = 'l';
        this.log = false;
        this.message_idx = 0;
        this.time = 0;
        this.run = false;
        this.play_on = false;
        this.heard = [];

        this.agent_automat = automata;
        this.agent_automat.agent = this;
        if (this.agent_automat.init) {
            this.agent_automat.init()
        }
    }

    msgGot(msg) {
        let data = msg.toString("utf8");
        this.processMsg(data);
        this.sendCmd();
        this.message_idx++
    }

    setSocket(socket) {
        this.socket = socket;
    }

    socketSend(cmd, value) {
        this.socket.sendMsg(`(${cmd} ${value})`);
    }

    processMsg(msg) {
        let data = Msg.parseMsg(msg);
        if (!data) throw new Error('Parse error\n' + msg);

        if (data.cmd === 'hear') this.analyzeHear(data.cmd, data.p);
        if (data.cmd === 'init') this.initAgent(data.p);
        if (data.cmd === 'sense_body') this.analyzeBody(data);
        this.analyzeEnv(data.msg, data.cmd, data.p)
    }

    analyzeHear(cmd, p) {
        this.run = true;
        this.heard.push({time: this.time, msg: p[2], who: p[1]});
        if (this.log) {
            console.log(`${p[1]} saying ${p[2]}`)
        }
        if (p[1] === 'referee' && p[2] === 'play_on') {
            this.play_on = true
        }
    }

    initAgent(p) {
        if (p[0] === 'r') this.position = 'r';
        if (p[1]) this.id = p[1];
        if (this.agent_automat.init) {
            this.agent_automat.init()
        }
    }


    analyzeBody(data) {
        this.body = {};
        this.time = data.p[0];
        for (const datum of data.p.slice(1)) {
            switch (datum.cmd) {
                case 'catch':
                    this.body.catch = datum.p[0];
                    break;
                case 'stamina':
                    this.body.stamina = datum.p[0]
            }
        }
    }

    analyzeEnv(msg, cmd, p) {
        if (cmd === 'see') {
            this.time = p[0];
            this.data = {
                ...locator.analyze(msg, cmd, p),
                time: this.time,
                body: this.body,
                hear: this.heard
            };
            if (this.log) {
                console.log(`State: ${this.agent_automat.getState()}`);
                console.log(`Data: ${this.data}`)
            }
        }
    }

    sendCmd() {
        if (_.isNil(this.data)) {
            return
        }
        if (this.run) {
            // Игра начата
            if (this.play_on) {
                const act = this.agent_automat.process(this.data);
                if (!_.isNil(act) && act !== true && act !== false) {
                    this.socketSend(...act);
                    if (this.log) {
                        console.log('act:', act)
                    }
                }
                this.data = null
            }
        }
    }
}

module.exports = Agent;
