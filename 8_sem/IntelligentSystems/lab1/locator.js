const fmin = require('fmin');
const Flatten = require('@flatten-js/core');
const Flags = require('./flags');
function solve(data) {
    let start = [0, 0];
    function loss(x) {
        return data.reduce((mse, d) => {
            return mse + ( ( (d.x - x[0]) ** 2 + (d.y + x[1]) ** 2) - d.d ** 2)
                ** 2;
        }, 0);
    }
    return fmin.nelderMead(loss, start);
}
function process_p(p) {
    switch (p.length) {
        case 1:
            return { a: p[0] }
        default:
            return { d: p[0], a: p[1] }
    }
}
function process_see(see_data) {
    return see_data.p.slice(1).reduce((proc, datum) => {
        if (datum.cmd.p[0] === 'f') {
            const flag = Flags[datum.cmd.p.join('')];
            if (flag) {
                proc.flags.push({ x: flag.x, y: flag.y, ...process_p(datum.p)
                });
            }
        } else if (datum.cmd.p[0] === 'p') {
            proc.players.push({
                team: datum.cmd.p[1],
                number: datum.cmd.p[2],
                goalie: datum.cmd.p[3],
                ...process_p(datum.p)
            });
        }

        return proc
    }, { flags: [], players: [] });
}
function locate_self(flags) {
    const [ x, y ] = solve(flags).x;
    return { x, y };
}
function locate_players(flags, players, x, y) {
    const flag = flags[0];
    const p = Flatten.point(flag.x, flag.y);
    const self = Flatten.point(x, y);
    const v1 = Flatten.vector(self, p);

    return players.map(player => {
        const a = (flag.a - player.a) / 180 * Math.PI;
        const v2 = v1.rotate(a).normalize().multiply(player.d);

        return {
            team: player.team,
            number: player.number,
            goalie: player.goalie,
            x: self.x + v2.x,
            y: self.y + v2.y,
        }
    });
}
module.exports = {
    locate_self,
    process_see,
    locate_players,
};