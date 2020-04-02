const msg_parse = require('./msg');
class Agent {
    constructor(x, y) {
        // Начальные координаты
        this.start_x = x;
        this.start_y = y;
        // Номер игрока
        this.id = null;
        // Позиция
        this.position = null;
        // Игра начата
        this.run = false;
        // Матч идёт
        this.play_on = false;
        // Действие
        this.act = null;

        // Контроллер
        this.controller = {
            acts: [
                { act: 'flag', fl: 'flt' },
                { act: 'flag', fl: 'fglt' },
                { act: 'kick', fl: 'b', goal: 'gr' }
            ],
            curr_act: 0,
        }
    }

    msgGot(msg) {
        // Получение сообщения

        // Приведение к строке
        let data = msg.toString('utf8');
        // Разбор сообщения
        this.processMsg(data);
        // Отправка команды
        this.sendCmd();
    }

    setSocket(socket) {
        // Настройка сокета
        this.socket = socket
    }
    socketSend(cmd, value) {
        // Отправка команды
        this.socket.sendMsg(`(${cmd} ${value})`);
    }
    processMsg(msg) {
        // Обработка сообщения
        // Разбор сообщения
        let data = msg_parse.parseMsg(msg);
        if(!data) {
            console.log('Parse error\n' + msg);
            return;
        }
        if(data.cmd == 'init') {
            // Инициализация
            this.initAgent(data.p);
        }

        if(data.cmd == 'hear') {
            // Анализ что агент слышит
            this.analyzeHear(data.msg, data.cmd, data.p);
        }
        // Анализ окружения агента
        this.analyzeEnv(data.msg, data.cmd, data.p);
    }

    initAgent(p) {
        if(p[0] == 'l') {
            // Левая половина поля
            this.position = 'l';
        } else if (p[0] == 'r') {
            // Правая половина поля
            this.position = 'r';
        }
        if(p[1]) {
            // id игрока
            this.id = p[1];
        }
        this.socketSend('move', `${this.start_x} ${this.start_y}`);
    }

    analyzeHear(msg, cmd, p) {
        if (!this.run) {
            // Первое (hear) - начало игры
            this.run = true;
        }
        if (p[1] == 'referee' && p[2] === 'play_on') {
            // Команда начала матча
            this.play_on = true;
        }
        if (msg.includes('goal')) {
            this.controller.curr_act = 0;
        }
    }

    analyzeEnv(msg, cmd, p) {
        // Анализ окружения агента
        if (cmd === 'see') {
            const act = this.controller.acts[this.controller.curr_act];
            if (act.act === 'flag') {
                const flag = p.filter(o => o.cmd && o.cmd.p.join('') ===
                    act.fl);
                if (flag.length) {
                    if (flag[0].p[0] > 7 && flag[0].p[1] == 0) {

                        this.act = { n: 'dash', v: 90 };

                    } else if (flag[0].p[0] < 7 && flag[0].p[0] > 3 &&
                        flag[0].p[1]=== 0) {
                        this.act = { n: 'dash', v: 40 };
                    } else if (flag[0].p[1] !== 0) {
                        this.act = { n: 'turn', v: flag[0].p[1] };
                    } else {
                        this.controller.curr_act += 1;

                    }
                } else {

                    this.act = { n: 'turn', v: 30 };

                }
            } else if (act.act === 'kick') {
                const ball = p.filter(o => o.cmd && o.cmd.p[0] === 'b');
                const goal = p.filter(o => o.cmd && o.cmd.p.join('') ===
                    act.goal);
                if (ball.length) {
                    if (ball[0].p[0] > 0.5 && ball[0].p[1] !== 0) {

                        this.act = { n: 'turn', v: ball[0].p[1] };

                    } else if (ball[0].p[0] > 0.5 && ball[0].p[1] == 0) {

                        this.act = { n: 'dash', v: 90 };
                    } else if (!goal.length) {

                        this.act = { n: 'kick', v: '10 30'};

                    } else if (goal.length && Math.abs(goal[0].p[1]) > 10) {

                        this.act = { n: 'kick', v: `10 ${goal[0].p[1]}`};

                    } else if (goal.length) {

                        this.act = { n: 'kick', v: `100 ${goal[0].p[1]}`};

                    }
                } else {

                    this.act = { n: 'turn', v: 30 };

                }
            }
        }
    }

    sendCmd() {
        // Есть действие и агент активен
        if (this.act && this.run) {
            // Матч начался
            if (this.play_on) {
                if (this.act.n === 'kick') {
                    // Пнуть мяч
                    this.socketSend(this.act.n, `${this.act.v}`);
                } else {
                    // Движение и поворот
                    this.socketSend(this.act.n, this.act.v);
                }
            }
        }
        // Сброс команды
        this.act = null;
    }
}
// Экспорт игрока
module.exports = Agent;