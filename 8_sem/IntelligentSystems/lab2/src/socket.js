// Модуль для работы с UDP
const dgram = require('dgram');
module.exports = function(agent, teamName, version) {
    // Создание сокета
    const socket = dgram.createSocket({ type: 'udp4', reuseAddr: true });
    // Задание сокета для агента
    agent.setSocket(socket);

    socket.on('message', (msg, info) => {
        // Обработка полученного сообщения
        agent.msgGot(msg);
    });
    socket.sendMsg = function(msg) {
        // Отправка сообщения серверу
        socket.send(Buffer.from(msg), 6000, 'localhost', (err, bytes) => {
            if(err) {
                throw err;
            }
        });
    }
    // Инициализация игрока на сервере (без параметра goalie)
    socket.sendMsg(`(init ${teamName} (version ${version}))`);
}