const _ = require('lodash');

class Node {
    static extend(obj) {
        return _.extend(new Node(), obj)
    }

    resolve(ta, name) {
        this.name = name;
        this.ta = ta;
        if (this.action) {
            this.actionName = this.action;
            this.action = ta.action(this.action)
        }
        for (const edge of this.edges) {
            edge.resolve(ta, this)
        }
    }

    getEdgesTo(to) {
        return this.edges.filter(
            (edge) => edge.to.name === to || edge.to.name === to.name
        )
    }
}

module.exports = {Node};