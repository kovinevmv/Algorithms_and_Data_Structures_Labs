const _ = require('lodash');
const {Manager} = require('../../manager');

class Automat {
    get isCurrentEdge() {
        return _.isArray(this.current)
    }

    static extend(obj) {
        const ta = _.extend(new Automat(), obj);
        ta.afterCreated();
        return ta
    }

    afterCreated() {
        for (const [name, node] of Object.entries(this.nodes)) {
            node.resolve(this, name)
        }
        for (const actionName of Object.keys(this.actions)) {
            this.actions[actionName] = this.actions[actionName].bind(this)
        }


        this.state = {};
        this.local = {};
        this.next = true;
        this.current = this.node(this.start);
        this.beforeExec = this.beforeExec.bind(this);
        this.sync = null;
        this.lastIncTime = 0;
        this.mgr = new Manager();
        this.ta = this;
        this.s = this.settings;
    }

    getState() {
        return this.state;
    }

    process(data) {
        this.data = data;
        this.mgr.data = data;
        this.incTimers();
        if (this.beforeExec) {
            this.beforeExec()
        }
        const ret = this.execute();
        return ret
    }

    incTimers() {
        if (this.data.time > this.lastIncTime) {
            this.lastIncTime = this.data.time;
            for (const key of Object.keys(this.timers)) {
                this.timers[key] += 1
            }
        }
    }

    execute() {
        if (this.sync) {
            this.logAction(this.sync);
            return this.action(this.sync)()
        }
        if (this.next) {
            if (this.isCurrentEdge) {
                return this.goNextEdge()
            }
            return this.goNextState()
        }
        if (this.isCurrentEdge) {
            return this.execEdge()
        }
        return this.execState()
    }

    logCurrent() {
        if (this.isCurrentEdge) {
            process.stdout.write(`(${this.current[0].name}): ${this.current.length} >`)
        } else {
            process.stdout.write(` ${this.current.name} >`)
        }
    }

    logAction(name) {
    }

    goNextEdge() {
        this.next = false;
        this.current = this.current[0].to;
        return this.execute()
    }

    goNextState() {
        for (const edge of this.current.edges) {
            if (edge.guard) {
                if (!edge.guard()) {
                    continue
                }
            }
            if (edge.sync && !edge.syncActive) {
                this.logAction(edge.actionName);
                if (!edge.sync()) {
                    continue
                }
            }
            this.current = [edge];
            this.next = false;
            return this.execute()
        }
        if (this.current.action) {
            this.logAction(this.current.actionName);
            const actionRes = this.current.action();
            if (actionRes) {
                return actionRes
            }
        }
        console.warn("!!! I'm stuck")
    }

    execEdge() {

        for (const edge of this.current) {
            if (edge.guard) {
                if (!edge.guard()) {
                    continue
                }
            }
            if (edge.assign) {
                edge.assign()
            }
            if (edge.sync && edge.syncActive) {
                this.logAction(edge.actionName);
                return edge.sync()
            }
        }
        this.next = true;
        return this.execute()
    }

    execState() {
        if (this.current.action) {
            this.logAction(this.current.actionName);
            const actionRes = this.current.action();
            if (!actionRes && this.next) {
                return this.execute()
            }
            return actionRes
        }
        this.next = true;
        return this.execute()
    }

    node(name) {
        const node = this.nodes[name];
        if (!_.isNil(node)) {
            return node
        }
        throw new Error(`Node ${name} does not exist`)
    }

    action(name) {
        const action = this.actions[name];
        if (!_.isNil(action)) {
            return action
        }
        throw new Error(`Action ${name} does not exist`)
    }

    make_msg(a, ...v) {
        switch (a) {
            case 'turn':
                this.data.angle -= v[0];
                break
        }
        if (!_.isEmpty(v)) {
            return [a, v.join(' ')]
        }
        return [a]
    }
}




const {Node} = require('./Node');
const {Edge} = require('./Edge');

module.exports = {Node, Edge, Automaton: Automat};
