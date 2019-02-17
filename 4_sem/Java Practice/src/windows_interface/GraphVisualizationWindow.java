package windows_interface;

import javax.swing.*;
import business_logic.Graph;
import business_logic.STATE;
import business_logic.Sorter;
import paint_graph.GraphPainter;

import java.awt.*;
import java.awt.event.ActionEvent;


/**
 * Window with loaded graph and alghoritm visualization
 * @author Grigoriev Ivan
 */

public class GraphVisualizationWindow extends JFrame {

    private Sorter sorter;

    public GraphVisualizationWindow(Graph graph) {
        super("Topological sorting of a graph");

        GraphPainter graphPainter = new GraphPainter(graph);
        this.sorter = new Sorter(graph, graphPainter);

        JPanel next = new JPanel(new FlowLayout());

        JButton nextButton = new JButton(new AbstractAction("Next step") {
            @Override
            public void actionPerformed(ActionEvent e) {
                sorter.nextStep();
                if(sorter.getState() == STATE.END) {
                    JOptionPane.showMessageDialog(next,
                            "Alghoritm is over",
                            "Alghoritm completion",
                            JOptionPane.ERROR_MESSAGE);
                    setEnabled(false);
                }
            }
        });
        
        next.add(nextButton);

        add(graphPainter.getGraphpanel());
        add(next, BorderLayout.SOUTH);
    }
}
