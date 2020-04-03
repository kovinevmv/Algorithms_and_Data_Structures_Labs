const _ = require('lodash');

class Edge {
    get name() {
        return `${this.from_name} -> ${this.to_name}`
    }
    static extend(obj) {
        return _.extend(new Edge(), obj)
    }

    resolve(ta, source) {
        this.ta = ta;

        this.to_name = this.to;
        this.to = ta.node(this.to);
        this.from = source;
        this.from_name = source.name;

        if (!_.isNil(this.sync)) {
            this.syncActive = this.sync.endsWith('!');
            this.actionName = this.sync;
            this.sync = ta.action(this.sync.substring(0, this.sync.length - 1))
        }
    }
}

module.exports = {Edge};