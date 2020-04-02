const Agent = require('./agent');
const VERSION = 7;
const {teamA_config} = require('./config/team_config');


function createPlayer(team, role, position) {
    console.log(`CREATE: player ${team}, ${role}, (${position.x} ${position.y})`);
    setTimeout(() => {
        createAgent(team, 0, position.x, position.y, role)
    }, 100);
}


teamA_config.forEach((player) => {
    createPlayer('A', player.role, player.position);
    createPlayer('B', player.role, player.position);

});

function createAgent(teamName, speed, x, y, goalie) {
    const isGoalie = goalie === 'goalie' ? goalie : null;
    let agent = new Agent(speed, teamName, isGoalie);

    require('./socket')(agent, teamName, VERSION, isGoalie);
    setTimeout(() => {
        agent.socketSend("move", `${x} ${y}`)
    }, 20)
}

