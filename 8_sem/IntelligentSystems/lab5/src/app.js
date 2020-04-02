const {Automaton} = require('./automat_logic/automat/Automat');
const {Command, flags} = require('@oclif/command');
const {AUTOMAT_TYPES} = require("./automat_logic/types/types");

const Agent = require('./agent')
const VERSION = 7;

class App extends Command {
    async run() {
        const {flags} = this.parse(App);
        const teamName = 'team' + flags.team.toUpperCase();
        const automatonData = AUTOMAT_TYPES[flags.automaton];
        const automaton = Automaton.extend(automatonData);
        const agent = new Agent(automaton);

        agent.position = (flags.team === 'b') ? 'r' : 'l';
        agent.teamName = teamName;
        require('./socket')(agent, teamName, VERSION, flags.goalie);

        console.log(`${teamName}. Type: ${flags.automaton}`);
        if (flags.automaton === "GOALIE"){ flags.x = -50; flags.y = 1;}

        setTimeout(() => {
            agent.socketSend('move', `${flags.x} ${flags.y}`); // Размещение игрока на поле
            agent.log = !flags['no-log']
        }, 100)
    }
}

App.flags = {
    automaton: flags.string({
        required: true,
        options: Object.keys(AUTOMAT_TYPES),
        default: Object.keys(AUTOMAT_TYPES)[0],
    }),
    team: flags.string({
        default: 'a',
    }),
    'no-log': flags.boolean({
        default: true,
    }),
    x: flags.string({
        parse: (input) => Number(input),
        dependsOn: ['y'],
        default: -20,
    }),
    y: flags.string({
        parse: (input) => Number(input),
        dependsOn: ['x'],
        default: 20,
    }),
    goalie: flags.boolean({
        default: false,
        char: 'g',
    }),
};

App.run().catch(require('@oclif/errors/handle'));
