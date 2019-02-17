package paint_graph;

import business_logic.Graph;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class GraphPanel extends JPanel {
    private Edge[] edges;
    private Node[] nodes;

    public GraphPanel(Graph _graph) {
        super();

        int numberNodes = _graph.getNameList().length;
        int matrixSize = _graph.getIndMatrix().length;

        nodes = new Node[numberNodes];
        for (int i = 0; i < numberNodes; i++) {
            int x = (int) (getWidth()  / 2 + Math.cos(i * 2 * Math.PI / numberNodes) * 200);
            int y = (int) (getHeight() / 2 - Math.sin(i * 2 * Math.PI / numberNodes) * 200);
            nodes[i] = new Node(x, y, _graph.getNameList()[i]);
        }

        int numberEdges = 0;
        for (int i = 0; i < matrixSize; i++)
            for (int j = 0; j < matrixSize; j++)
                if (_graph.getIndMatrix()[i][j] == 1 && i != j)
                    numberEdges++;

        int k = 0;
        edges = new Edge[numberEdges];
        for (int i = 0; i < matrixSize; i++)
            for (int j = 0; j < matrixSize; j++)
                if (_graph.getIndMatrix()[i][j] == 1 && i != j)
                    edges[k++] = new Edge(nodes[i], nodes[j]);

        refreshScreen();
    }

    private void adjust() {
        for (int i = 0; i < nodes.length; i++) {
            int x = (int) (getWidth() / 2 + Math.cos(i * 2 * Math.PI / nodes.length) * 200);
            int y = (int) (getHeight() / 2 - Math.sin(i * 2 * Math.PI / nodes.length) * 200);
            nodes[i].setX(x);
            nodes[i].setY(y);
        }
    }

    private void refreshScreen() {
        Timer timer = new Timer(0, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                repaint();
            }
        });
        timer.setRepeats(true);
        timer.setDelay(17);
        timer.start();
    }

    @Override
    public void paint(Graphics g) {
        adjust();
        g.setColor(Color.WHITE);
        g.fillRect(0, 0, getWidth(), getHeight());

        g.setColor(Color.BLACK);
        addText(g, "Граф", 25, getWidth() / 2, 20);

        for (int i = 0; i < edges.length; i++) {
            Edge e = new Edge(edges[i]);
            ((Graphics2D) g).setStroke(new BasicStroke(2));
            g.setColor(e.getColorEdge());
            g.drawLine(e.getSource().getX(), e.getSource().getY(),
                    e.getDestination().getX(), e.getDestination().getY());

            int arrowSize = e.getArrowSize();

            int[] xpoints = {
                    (int) (e.getOffsetDestination().x),
                    (int) (e.getOffsetDestination().x + Math.sin(e.getAngle() - Math.PI / 2.7) * arrowSize),
                    (int) (e.getOffsetDestination().x + Math.sin(e.getAngle() - Math.PI + Math.PI / 2.7) * arrowSize)
            };

            int[] ypoints = {
                    (int) (e.getOffsetDestination().y),
                    (int) (e.getOffsetDestination().y + Math.cos(e.getAngle() - Math.PI / 2.7) * arrowSize),
                    (int) (e.getOffsetDestination().y + Math.cos(e.getAngle() - Math.PI + Math.PI / 2.7) * arrowSize)
            };

            g.fillPolygon(xpoints, ypoints, 3);
            g.drawPolygon(xpoints, ypoints, 3);
        }

        ((Graphics2D) g).setStroke(new BasicStroke(2));
        for (int i = 0; i < nodes.length; i++) {
            Node n = new Node(nodes[i]);

            g.setColor(n.getColor());
            g.fillOval(n.getX() - n.getRadius() / 2, n.getY() - n.getRadius() / 2,
                    n.getRadius(), n.getRadius());

            g.setColor(Color.BLACK);
            g.drawOval(n.getX() - n.getRadius() / 2, n.getY() - n.getRadius() / 2,
                    n.getRadius(), n.getRadius());
            addText(g, n.getName(), n.getTextSize(), n.getX(), n.getY());
        }
    }

    private void addText(Graphics g, String text, int size, double x, double y) {
        Font font = new Font("Default", Font.PLAIN, size);
        g.setFont(font);

        g.drawString(text,
                (int) x - g.getFontMetrics(font).stringWidth(text) / 2,
                (int) y - g.getFontMetrics(font).getHeight() / 2 + g.getFontMetrics(font).getAscent());
    }

    public void changeColorOfNode(int node, Color color) {
        //System.out.println("Change color node: " + nodes[node].getName());
        nodes[node].setColor(color);
    }

    public void changeColorOfEdge(int node1, int node2, Color color) {
        //System.out.println("Change color edge: " + nodes[node1].getName() + " " + nodes[node2].getName());
        for (int i = 0; i < edges.length; i++) {
            if (edges[i].getDestination().equals(nodes[node1]) && edges[i].getSource().equals(nodes[node2])) {
                edges[i].setColorEdge(color);
                return;
            }
            if (edges[i].getDestination().equals(nodes[node2]) && edges[i].getSource().equals(nodes[node1])) {
                edges[i].setColorEdge(color);
                return;
            }
        }
    }
}
