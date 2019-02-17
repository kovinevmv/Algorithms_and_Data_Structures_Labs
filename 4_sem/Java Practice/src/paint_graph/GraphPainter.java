package paint_graph;

import business_logic.*;

import javax.swing.*;
import java.awt.*;

public class GraphPainter {

    private final JPanel mainPanel;
    private final GraphPanel graphpanel;
    private final JTable table;

    public GraphPainter(Graph graph){
        this.graphpanel = new GraphPanel(graph);

        JPanel tablePanel = new JPanel(new BorderLayout());
        Object[][] data = new Object[1][graph.getNameList().length];

        table = new JTable(data, graph.getNameList());
        tablePanel.add(table.getTableHeader(), BorderLayout.NORTH);
        tablePanel.add(table, BorderLayout.SOUTH);

        mainPanel = new JPanel(new BorderLayout());
        mainPanel.add(tablePanel, BorderLayout.PAGE_END);
        mainPanel.add(graphpanel, BorderLayout.CENTER);
    }

    public JPanel getGraphpanel() {
        return mainPanel;
    }

    public void fillVertex(int node){
        graphpanel.changeColorOfNode(node, new Color(0,255,0));
    }

    public void fillEdge(int begin, int end, EDGECOLOR state) {
        if (state.equals(EDGECOLOR.ACTIVE))
            graphpanel.changeColorOfEdge(begin, end, new Color(255,0,0));
        else if (state.equals(EDGECOLOR.NOTACTIVE))
            graphpanel.changeColorOfEdge(begin, end, new Color(0,0,0));

    }
    public void updateTable(int node, int vertexSortNumber){
        table.setValueAt(vertexSortNumber,  0, node);
    }
}
