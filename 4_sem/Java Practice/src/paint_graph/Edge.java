package paint_graph;

import java.awt.*;
import java.awt.geom.Point2D;

class Edge {
    private final Node source;
    private final Node destination;

    private final int arrowSize;
    private Color colorEdge;

    public Edge(Node s, Node d){
        source = s;
        destination = d;
        colorEdge = new Color(0,0,0);
        arrowSize = 20;
    }

    public Edge(Edge other) {
        this.source = other.source;
        this.destination = other.destination;
        this.colorEdge = other.colorEdge;
        this.arrowSize = other.arrowSize;
    }

    private Point2D components(){
        return new Point2D.Double(destination.getX() - source.getX(),
                destination.getY() - source.getY());
    }

    private double length(){
        return Math.sqrt(((Point2D.Double) this.components()).x* ((Point2D.Double) this.components()).x +
                ((Point2D.Double) this.components()).y*((Point2D.Double) this.components()).y);
    }

    public double getAngle() {
        double ar_ang = Math.acos( (((Point2D.Double) this.components()).x) / this.length());
        if(destination.getY() >= source.getY())
            ar_ang = Math.PI * 2 - ar_ang;
        return ar_ang;
    }

    public Point2D.Double getOffsetDestination() {
        return new Point2D.Double(destination.getX() - ((Point2D.Double) this.components()).x * 
					destination.getRadius() / 2 / this.length(),
					destination.getY() -((Point2D.Double) this.components()).y *
					destination.getRadius() / 2 / this.length());
    }

    public Node getSource() {
        return source;
    }

    public Node getDestination() {
        return destination;
    }

    public Color getColorEdge() {
        return colorEdge;
    }

    public void setColorEdge(Color colorEdge) {
        this.colorEdge = colorEdge;
    }

    public int getArrowSize() {
        return arrowSize;
    }
}

