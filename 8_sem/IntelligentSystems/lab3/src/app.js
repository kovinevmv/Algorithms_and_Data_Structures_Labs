const Agent = require('./agent') // Импорт агента
const VERSION = 7 // Версия сервера
const readline = require('readline');
const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

getCoords()

function createAgent(teamName, speed, x, y, goalie) {
  let agent = new Agent(speed, teamName) // Создание экземпляра агента
  require('./socket')(agent, teamName, VERSION, goalie) //Настройка сокета
  setTimeout(() => {
    agent.socketSend("move", `${x} ${y}`)
  }, 20)
}

function getCoords() {
  rl.question('X ', (firstX) => {
    rl.question('Y ', (firstY) => {
      rl.question('TeamName ', async (teamName) => {
        await createAgent(teamName, 0, firstX, firstY)
        await createAgent(teamName, 0, Number(firstX) - 10, firstY)
        await createAgent('goalieTeam', 0, -15, -10, 'goalie')
        rl.close();
      })
    })
  })
}
