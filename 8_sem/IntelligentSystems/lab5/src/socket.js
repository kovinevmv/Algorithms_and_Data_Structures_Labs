const dgram = require('dgram'); // Модуль для работы с UDP
module.exports = function (agent, teamName, version, goalie) {
    // Создание сокета
    const socket = dgram.createSocket({type: 'udp4', reuseAddr: true});
    agent.setSocket(socket); // Задание сокета для агента
    socket.on('message', (msg) => {
        agent.msgGot(msg) // Обработка полученного сообщения
    });
    socket.sendMsg = function (msg) {
        // Отправка сообщения серверу
        socket.send(Buffer.from(msg), 6000, 'localhost', (err) => {
            if (err) throw err
        })
    };
    if (goalie) {
        socket.sendMsg(`(init ${teamName} (version ${version}) (goalie))`)
    } else {
        socket.sendMsg(`(init ${teamName} (version ${version}))`)
    }
};
