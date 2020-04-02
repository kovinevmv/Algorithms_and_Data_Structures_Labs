const FL = 'flag', KI = 'kick'

const DT = {
    state: {
        next: 0,
        sequence: [
            {
              act: FL,
              fl: 'gr',
              maxDistance: 2
            },
            {
              act: KI,
              fl: 'b',
              goal: 'gl'
            }
        ],
        command: null
    },
    root: {
      exec(mgr, state) {
          state.action = state.sequence[state.next];
          state.command = null
      },
      next: 'goalVisible'
    },
    goalVisible: {
        condition: (mgr, state) => mgr.getVisible(state.action.fl),
        trueCond: 'rootNext',
        falseCond: 'rotate'
    },
    rotate: {
        exec(mgr, state) {
            state.command = {
                n: 'turn',
                v: '45'
            }
        },
        next: 'sendCommand'
    },
    rootNext: {
        condition: (mgr, state) => state.action.act === FL,
        trueCond: 'flagSeek',
        falseCond: 'rotateToBall'
    },
    flagSeek: {
        condition: (mgr, state) => mgr.getDistance(state.action.fl) < state.action.maxDistance,
        trueCond: 'checkMinDistance',
        falseCond: 'farGoal'
    },
    checkMinDistance: {
        condition: (mgr, state) => state.action.minDistance && mgr.getDistance(state.action.fl) < state.action.minDistance,
        trueCond: 'tooCloseGoal',
        falseCond: 'closeFlag'
    },
    closeFlag: {
        exec(mgr, state) {
            state.next++;
            state.action = state.sequence[state.next]
        },
        next: 'rootNext',
    },
    tooCloseGoal: {
      condition: (mgr, state) => mgr.getAngle(state.action.fl) > 4,
      trueCond: 'rotateToGoal',
      falseCond: 'runFromGoal',
    },
    runFromGoal: {
        exec(mgr, state) {
          state.command = {
              n: 'dash',
              v: -100
          }
        },
        next: 'sendCommand'
    },
    farGoal: {
        condition: (mgr, state) => mgr.getAngle(state.action.fl) > 4,
        trueCond: 'rotateToGoal',
        falseCond: 'runToGoal',
    },
    rotateToGoal: {
        exec(mgr, state) {
            state.command = {
                n: 'turn',
                v: mgr.getAngle(state.action.fl)
            }
        },
        next: 'sendCommand',
    },
    runToGoal: {
        exec(mgr, state) {
            state.command = {
                n: 'dash',
                v: 100
            }
        },
        next: 'sendCommand'
    },
    rotateToBall: {
        condition: (mgr, state) => mgr.getAngle(state.action.fl) > 4,
        trueCond: 'rotateToGoal',
        falseCond: 'checkMaxDistanceToBall'
    },
    checkMinDistanceToBall: {
        condition: (mgr, state) => mgr.getDistance(state.action.fl) > 1.2,
        trueCond: 'farGoal',
        falseCond: 'checkDistanceForKickAndCatch'
    },
    checkMaxDistanceToBall: {
        condition: (mgr, state) => mgr.getDistance(state.action.fl) < 28,
        trueCond: 'checkMinDistanceToBall',
        falseCond: 'rotateToGoal'
    },
    checkDistanceForKickAndCatch: {
        condition: (mgr, state) => {
            let myPos = mgr.getMyPos()
            return myPos ? (myPos.x > 40 && Math.abs(myPos.y) < 10) : true
        },
        trueCond: 'doCatch',
        falseCond: 'checkDistanceForKick',
    },
    checkDistanceForKick: {
        condition: (mgr, state) => mgr.getDistance(state.action.fl) <= 0.5,
        trueCond: 'doKick',
        falseCond: 'farGoal',
    },
    doCatch: {
        exec(mgr, state) {
          state.command = {
              n: 'catch',
              v: String(mgr.getAngle(state.action.fl))
          }
        },
        next: 'sendCommand'
    },
    doKick: {
        condition: (mgr, state) => mgr.getVisible(state.action.goal),
        trueCond: 'ballGoalVisible',
        falseCond: 'ballGoalInvisible',
    },
    ballGoalVisible: {
        exec(mgr, state) {
            state.command = {
                n: 'kick',
                v: `100 ${mgr.getAngle(state.action.goal)}`
            }
            state.next = 0
        },
        next: 'sendCommand'
    },
    ballGoalInvisible: {
        exec(mgr, state) {
            state.command = {
                n: 'kick',
                v: '10 45'
            }
        },
        next: 'sendCommand'
    },
    sendCommand: {
      command: (mgr, state) => state.command
    }
}

module.exports = DT
