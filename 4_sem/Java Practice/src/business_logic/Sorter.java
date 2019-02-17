package business_logic;

import paint_graph.GraphPainter;
import paint_graph.EDGECOLOR;

import java.util.*;

public class Sorter {
    private final int[][] indMatrix;
    private final GraphPainter painter;
    private Deque<Integer> stk;
    private boolean[] visited;
    private int current;
    private int from;
    private STATE state;
    private int sortNumber;

    public Sorter(Graph graph, GraphPainter painter) {
        this.indMatrix = graph.getIndMatrix();
        this.painter = painter;
        this.stk = new ArrayDeque<>();
        this.visited = new boolean[indMatrix.length];
        this.current = 0;
        this.from = -1;
        this.state = STATE.WORK;
        this.sortNumber = 1;
    }

    public STATE getState() {
        return state;
    }

    public void nextStep() {
        if (state == STATE.END) {
               return;
        }

        if (visited[current] == false) {
            painter.fillVertex(current);
            visited[current] = true;
        }

        for (int v = 0; v < indMatrix[current].length; v++) {
            if (indMatrix[current][v] == 1) {
                if (visited[v] == false) {
                    painter.fillEdge(current, v, EDGECOLOR.ACTIVE);

                    stk.addLast(from);
                    stk.addLast(current);

                    from = current;
                    current = v;

                    return;
                }
            }
        }

        painter.updateTable(current, indMatrix.length + 1 - sortNumber);
        sortNumber++;

        if (from != -1) {
            painter.fillEdge(from, current, EDGECOLOR.NOTACTIVE);

            current = stk.pollLast();
            from = stk.pollLast();

            return;
        }
        else {
            for (int v = 0; v < indMatrix.length; v++) {
                if (visited[v] == false) {
                    current = v;
                    painter.fillVertex(v);
                    visited[current] = true;
                    from = -1;

                    return;
                }
            }

            state = STATE.END;
        }
    }
}