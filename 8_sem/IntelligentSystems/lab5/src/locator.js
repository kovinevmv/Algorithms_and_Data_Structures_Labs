const _ = require('lodash');
const fmin = require('fmin');
const Flatten = require('@flatten-js/core');
const mod = require('./util/utils').mod;

const Flags = {
    fb0: {x: 0, y: -39},
    fbl10: {x: -10, y: -39},
    fbl20: {x: -20, y: -39},
    fbl30: {x: -30, y: -39},
    fbl40: {x: -40, y: -39},
    fbl50: {x: -50, y: -39},
    fbr10: {x: 10, y: -39},
    fbr20: {x: 20, y: -39},
    fbr30: {x: 30, y: -39},
    fbr40: {x: 40, y: -39},
    fbr50: {x: 50, y: -39},
    fc: {x: 0, y: 0},
    fcb: {x: 0, y: -34},
    fct: {x: 0, y: 34},
    fglb: {x: -52.5, y: -7.01},
    fglt: {x: -52.5, y: 7.01},
    fgrb: {x: 52.5, y: -7.01},
    fgrt: {x: 52.5, y: 7.01},
    fl0: {x: -57.5, y: 0},
    flb10: {x: -57.5, y: -10},
    flb20: {x: -57.5, y: -20},
    flb30: {x: -57.5, y: -30},
    flb: {x: -52.5, y: -34},
    flt10: {x: -57.5, y: 10},
    flt20: {x: -57.5, y: 20},
    flt30: {x: -57.5, y: 30},
    flt: {x: -52.5, y: 34},
    fplb: {x: -36, y: -20.15},
    fplc: {x: -36, y: 0},
    fplt: {x: -36, y: 20.15},
    fprb: {x: 36, y: -20.15},
    fprc: {x: 36, y: 0},
    fprt: {x: 36, y: 20.15},
    fr0: {x: 57.5, y: 0},
    frb10: {x: 57.5, y: -10},
    frb20: {x: 57.5, y: -20},
    frb30: {x: 57.5, y: -30},
    frb: {x: 52.5, y: -34},
    frt10: {x: 57.5, y: 10},
    frt20: {x: 57.5, y: 20},
    frt30: {x: 57.5, y: 30},
    frt: {x: 52.5, y: 34},
    ft0: {x: 0, y: 39},
    ftl10: {x: -10, y: 39},
    ftl20: {x: -20, y: 39},
    ftl30: {x: -30, y: 39},
    ftl40: {x: -40, y: 39},
    ftl50: {x: -50, y: 39},
    ftr10: {x: 10, y: 39},
    ftr20: {x: 20, y: 39},
    ftr30: {x: 30, y: 39},
    ftr40: {x: 40, y: 39},
    ftr50: {x: 50, y: 39},
    gl: {x: -52.5, y: 0},
    gr: {x: 52.5, y: 0},
};

const minX = _.min(Object.values(Flags).map((flag) => flag.x));
const minY = _.min(Object.values(Flags).map((flag) => flag.y));
const maxX = _.max(Object.values(Flags).map((flag) => flag.x));
const maxY = _.max(Object.values(Flags).map((flag) => flag.y));

function error(x, y) {
    return Math.min(0, x - minX) ** 4 + Math.min(0, y - minY) ** 4 + Math.max(0, x - maxX) ** 4 + Math.max(0, y - maxY) ** 4
}

function solve(data) {
    let v = [0, 0];

    function func(v) {
        return data.reduce((mse, d) => {
            return mse + ((d.x - v[0]) ** 2 + (d.y - v[1]) ** 2 - d.d ** 2) ** 2
        }, 0) + error(v[0], v[1]) ** 4
    }

    return fmin.nelderMead(func, v)
}

function process_p(p) {
    switch (p.length) {
        case 1:
            return {a: p[0]};
        default:
            return {d: p[0], a: p[1]}
    }
}

function process_see(see_data) {
    return see_data.p.slice(1).reduce(
        (proc, datum) => {
            if (datum.cmd.p[0] === 'f' || datum.cmd.p[0] === 'g') {
                const flag = Flags[datum.cmd.p.join('')];
                if (flag) {
                    proc.flags.push({
                        n: datum.cmd.p.join(''),
                        x: flag.x,
                        y: flag.y,
                        ...process_p(datum.p),
                    })
                }
            } else if (datum.cmd.p[0] === 'p') {
                proc.players.push({
                    team: datum.cmd.p[1],
                    number: datum.cmd.p[2],
                    goalie: datum.cmd.p[3],
                    ...process_p(datum.p),
                })
            } else if (datum.cmd.p[0] === 'b') {
                proc.ball.push({...process_p(datum.p)})
            }
            return proc
        },
        {flags: [], players: [], ball: []}
    )
}

function locate_self(flags) {
    const [x, y] = solve(flags).x;
    return {x, y}
}

function locate_ball(flags, ball, x, y) {
    const flag = flags[0];
    const p = Flatten.point(flag.x, flag.y);
    const self = Flatten.point(x, y);
    const v1 = Flatten.vector(self, p);
    const _ball = ball[0];
    const a = ((flag.a - _ball.a) / 180) * Math.PI;
    const v2 = v1
        .rotate(a)
        .normalize()
        .multiply(_ball.d);
    return {x: self.x + v2.x, y: self.y + v2.y, a: _ball.a, d: _ball.d}
}

function locate_angle(flags, x, y) {
    const flag = flags[0];
    const p = Flatten.point(flag.x, flag.y);
    const self = Flatten.point(x, y);
    const v1 = Flatten.vector(self, p);
    return mod((v1.slope / Math.PI * 180 + flag.a), 360)
}

function locate_players(flags, players, x, y) {
    const flag = flags[0];
    const p = Flatten.point(flag.x, flag.y);
    const self = Flatten.point(x, y);
    const v1 = Flatten.vector(self, p);
    return players.map((player) => {
        const a = ((flag.a - player.a) / 180) * Math.PI;
        const v2 = v1
            .rotate(a)
            .normalize()
            .multiply(player.d);
        return {
            team: player.team && player.team.slice(1, -1),
            number: player.number,
            goalie: player.goalie,
            x: self.x + v2.x,
            y: self.y + v2.y,
            d: player.d,
            a: player.a
        }
    })
}

function analyze(msg, cmd, p) {
    const data = {};
    const {flags, players, ball} = process_see({msg, cmd, p});
    const {x, y} = locate_self(flags);
    data.self = Flatten.point(x, y);

    // FIXME optimize if starts to lag
    if (!_.isEmpty(flags)) {
        data.players = locate_players(flags, players, x, y);
        data.angle = locate_angle(flags, x, y)
    }
    if (!_.isEmpty(ball)) {
        data.ball = locate_ball(flags, ball, x, y)
    }
    return data
}

module.exports = {
    locate_self,
    process_see,
    locate_players,
    locate_ball,
    analyze,
    Flags,
};
