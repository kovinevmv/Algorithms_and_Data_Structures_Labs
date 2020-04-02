const DT = {
    state: {
        leader: 'p',
        command: null
    },
    root: {
        exec(mgr, state) {
            state.command = null
        },
        next: 'playerVisible'
    },
    playerVisible: {
        condition: (mgr, state) => mgr.getVisible(state.leader),
        trueCond: 'playerSeek',
        falseCond: 'rotate',
    },
    rotate: {
        exec(mgr, state) {
            state.command = {
                n: 'turn',
                v: '90'
            }
        },
        next: 'sendCommand',
    },
    playerSeek: {
        condition: (mgr, state) =>
            mgr.getDistance(state.leader) < 1 && Math.abs(mgr.getAngle(state.leader)) < 40,
        trueCond: 'closePlayer',
        falseCond: 'farPlayer'
    },
    closePlayer: {
        exec(mgr, state) {
            state.command = {
                n: 'turn',
                v: '30'
            }
        },
        next: 'sendCommand'
    },
    farPlayer: {
        condition: (mgr, state) => mgr.getDistance(state.leader) > 10,
        trueCond: 'bigAngleToPlayer',
        falseCond: 'notSoFarPlayer'
    },
    bigAngleToPlayer: {
        condition: (mgr, state) => mgr.getAngle(state.leader) > 5,
        trueCond: 'turnToPlayer',
        falseCond: 'runToPlayer'
    },
    turnToPlayer: {
        exec(mgr, state) {
            state.command = {
                n: 'turn',
                v: String(mgr.getAngle(state.leader))
            }
        },
        next: 'sendCommand'
    },
    runToPlayer: {
        exec(mgr, state) {
            state.command = {
                n: 'dash',
                v: '80'
            }
        },
        next: 'sendCommand'
    },
    notSoFarPlayer: {
        condition: (mgr, state) => mgr.getAngle(state.leader) > 40 || mgr.getAngle(state.leader) < 25,
        trueCond: 'turnToPlayerMinus30',
        falseCond: 'closeDistToPlayer'
    },
    turnToPlayerMinus30: {
        exec(mgr, state) {
            state.command = {
                n: 'turn',
                v: String(mgr.getAngle(state.leader) - 30)
            }
        },
        next: 'sendCommand'
    },
    closeDistToPlayer: {
        condition: (mgr, state) => mgr.getDistance(state.leader) < 7,
        trueCond: 'smallDashPlayer',
        falseCond: 'bigDashPlayer'
    },
    smallDashPlayer: {
        exec(mgr, state) {
            state.command = {
                n: 'dash',
                v: '20'
            }
        },
        next: 'sendCommand'
    },
    bigDashPlayer: {
        exec(mgr, state) {
            state.command = {
                n: 'dash',
                v: '40'
            }
        },
        next: 'sendCommand'
    },
    sendCommand: {
        command: (mgr, state) => state.command
    }
}

module.exports = DT