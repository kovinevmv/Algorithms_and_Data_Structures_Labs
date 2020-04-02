module.exports = {
    parseMsg(msg) {
        // Разбор сообщения
        if(msg.endsWith("\u0000")) {
            // Удаление символа в конце
            msg = msg.substring(0, msg.length - "\u0000".length);
        }
        let array = msg.match(/(\(|[-\d\.]+|[\\\"\w]+|\))/g);
        // Результирующее сообщение
        let res = { msg, p: [] }
        // Анализировать с индекса 0, результат в res
        this.parse(array, { idx: 0 }, res);
        // Выделить команду
        this.makeCmd(res);
        return res;
    },
    parse(array, index, res) {
        // Разбор сообщения в скобках

        // Всегда с открывающей скобки
        if(array[index.idx] != "(") {
            return;
        }
        index.idx++;

        // Разбор внутри скобок
        this.parseInner(array, index, res);
    },
    parseInner(array, index, res) {
        // Пока не встретится закрывающая скобка
        while (array[index.idx] != ")") {

            // Если внутри ещё одна скобка
            if(array[index.idx] == "(") {
                let r = { p: [] };

                // Рекурсивный вызов с index
                this.parse(array, index, r);
                res.p.push(r);
            } else {
                // Одиночный параметр
                let num = parseFloat(array[index.idx]);
                res.p.push(isNaN(num) ? array[index.idx] : num);
                index.idx++;
            }
        }
        index.idx++;
    },
    makeCmd(res) {
        // Выделение команды
        if(res.p && res.p.length > 0) {
            // Первый параметр - команда
            res.cmd = res.p.shift();
            // Выделить команды у параметров
            for(let value of res.p) {
                this.makeCmd(value);
            }
        }
    }
}