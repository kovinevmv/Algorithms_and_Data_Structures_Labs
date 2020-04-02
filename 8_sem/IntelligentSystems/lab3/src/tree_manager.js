const coordsApi = require('./getCoords')
module.exports = {
    getAction(dt, p) {
        this.p = p
        function execute(dt, title, Manager) {
            const action = dt[title]
            if (typeof action.exec == "function") {
                action.exec(Manager, dt.state)
                return execute(dt, action.next, Manager)
            }
            if (typeof action.condition == "function") {
                const cond = action.condition(Manager, dt.state)
                if (cond) return execute(dt, action.trueCond, Manager)
                return execute(dt, action.falseCond, Manager)
            }
            if (typeof action.command == "function") {
                return action.command(Manager, dt.state)
            }
            throw new Error(`Unexpected node in DT: ${title}`)
        }
        return execute(dt, "root", this)
    },
    getMyPos() {
        const flags = this.p.filter((obj) => obj.cmd && (obj.cmd.p[0] === 'f' || obj.cmd.p[0] === 'g'))
        const FlagsCoords = {
            ftl50: {x: -50, y: 39}, ftl40: {x: -40, y: 39},
            ftl30: {x: -30, y: 39}, ftl20: {x: -20, y: 39},
            ftl10: {x: -10, y: 39}, ft0: {x: 0, y: 39},
            ftr10: {x: 10, y: 39}, ftr20: {x: 20, y: 39},
            ftr30: {x: 30, y: 39}, ftr40: {x: 40, y: 39},
            ftr50: {x: 50, y: 39}, fbl50: {x: -50, y: -39},
            fbl40: {x: -40, y: -39}, fbl30: {x: -30, y: -39},
            fbl20: {x: -20, y: -39}, fbl10: {x: -10, y: -39},
            fb0: {x: 0, y: -39}, fbr10: {x: 10, y: -39},
            fbr20: {x: 20, y: -39}, fbr30: {x: 30, y: -39},
            fbr40: {x: 40, y: -39}, fbr50: {x: 50, y: -39},
            flt30: {x:-57.5, y: 30}, flt20: {x:-57.5, y: 20},
            flt10: {x:-57.5, y: 10}, fl0: {x:-57.5, y: 0},
            flb10: {x:-57.5, y: -10}, flb20: {x:-57.5, y: -20},
            flb30: {x:-57.5, y: -30}, frt30: {x: 57.5, y: 30},
            frt20: {x: 57.5, y: 20}, frt10: {x: 57.5, y: 10},
            fr0: {x: 57.5, y: 0}, frb10: {x: 57.5, y: -10},
            frb20: {x: 57.5, y: -20}, frb30: {x: 57.5, y: -30},
            fglt: {x:-52.5, y: 7.01}, fglb: {x:-52.5, y:-7.01},
            gl: {x:-52.5, y: 0}, gr: {x: 52.5, y: 0}, fc: {x: 0, y: 0},
            fplt: {x: -36, y: 20.15}, fplc: {x: -36, y: 0},
            fplb: {x: -36, y:-20.15}, fgrt: {x: 52.5, y: 7.01},
            fgrb: {x: 52.5, y:-7.01}, fprt: {x: 36, y: 20.15},
            fprc: {x: 36, y: 0}, fprb: {x: 36, y:-20.15},
            flt: {x:-52.5, y: 34}, fct: {x: 0, y: 34},
            frt: {x: 52.5, y: 34}, flb: {x:-52.5, y: -34},
            fcb: {x: 0, y: -34}, frb: {x: 52.5, y: -34},
            distance(p1, p2) {
                return Math.sqrt((p1.x-p2.x)**2+(p1.y-p2.y)**2)
            },
        }
        if (flags.length >= 2) {
            let myPos = null
            if (flags.length === 2) {
                myPos = coordsApi.getAnswerForTwoFlags(flags, FlagsCoords)
            } else {
                myPos = coordsApi.getAnswerForThreeFlags(flags, FlagsCoords)
            }
            // console.log(myPos)
            return myPos
        }
        return null
    },
    getVisible(goal) {
        // console.log(`getVisible <${goal}>: ${goal === 'p'
        //     ? Boolean(this.p.find(obj => obj.cmd && obj.cmd.p[0] === goal))
        //     : Boolean(this.p.find(obj => obj.cmd && obj.cmd.p.join('') === goal))}\n`)
        return goal === 'p'
            ? Boolean(this.p.find(obj => obj.cmd && obj.cmd.p[0] === goal))
            : Boolean(this.p.find(obj => obj.cmd && obj.cmd.p.join('') === goal))
    },
    getDistance(goal) {
        const goalObj = goal === 'p'
            ? this.p.find(obj => obj.cmd && obj.cmd.p[0] === goal)
            : this.p.find(obj => obj.cmd && obj.cmd.p.join('') === goal)
        console.log(`getDistance <${goal}>: ${goalObj && goalObj.p.length > 1 ? goalObj.p[0] : null}\n`)
        return goalObj && goalObj.p.length > 1 ? goalObj.p[0] : null
    },
    getAngle(goal) {
        const goalObj = goal === 'p'
            ? this.p.find(obj => obj.cmd && obj.cmd.p[0] === goal)
            : this.p.find(obj => obj.cmd && obj.cmd.p.join('') === goal)
        // if (!goalObj) {
        //     this.p.forEach((o) => console.log(goal, o.p, o.cmd))
        // }
        return goalObj ? (goalObj.p.length === 1 ? goalObj.p[0] : goalObj.p[1]) : null
    }
}
