const msg_parse = require('./msg');
const locator = require('./locator');
const _ = require('lodash');
class Agent {
    constructor() {
        // Позиция
        this.position = null;
        // Номер игрока
        this.id = null;
        // Игра начата
        this.run = false;
        // Матч идёт
        this.play_on = false;

        // Действия
        this.act = null;

        // Скорость вращения
        this.rotation_speed = 0;
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
            this.analyzeHear(data.cmd, data.p);
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
    }

    analyzeHear(cmd, p) {
        if (!this.run) {
            // Первое (hear) - начало игры
            this.run = true;
        }
        if (p[1] == 'referee' && p[2] === 'play_on') {
            // Команда начала матча
            this.play_on = true;
        }
    }

    analyzeEnv(msg, cmd, p) {
        // Анализ окружения агента
        if (cmd === 'see') {
            const { flags, players } = locator.process_see({ msg, cmd, p});

            if (!_.isEmpty(flags)) {
                const { x, y } = locator.locate_self(flags);
                const seen_players = locator.locate_players(flags, players, x,
                    y);
                console.log(Math.round(x), Math.round(y), seen_players);
                12
            } else {
                console.log('Not enough flags to locate other players');
            }
        }
    }

    sendCmd() {
        if(this.run) {
            // Игра начата

            if (this.play_on) {
                // Матч начался
                this.socketSend('turn', this.rotation_speed);
            }
        }
    }
}
// Экспорт игрока
module.exports = Agent;