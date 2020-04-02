const readline = require('readline')
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});
// Импорт агента
const Agent = require('./agent');
// Версия сервера
const VERSION = 7;
rl.question('Team [A/B]: ', answer => {
    var teamName = null;

    if (answer === 'A') {
        teamName = 'teamA';
    } else if (answer === 'B') {
        teamName = 'teamB';
    } else {
        console.log('unknow team name select default teamA');
        teamName = 'teamA';
    }

    // Создание экземпляра агента
    let agent = new Agent();

    rl.question('Enter rotation speed: ', answer => {
        agent.rotation_speed = answer;

        rl.question('Enter player coordinates: ', answer => {
            //Настройка сокета
            require('./socket')(agent, teamName, VERSION);
            // Размещение игрока на поле
            agent.socketSend('move', answer);
        });
    });
});