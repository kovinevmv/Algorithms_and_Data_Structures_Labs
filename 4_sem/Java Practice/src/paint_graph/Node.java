package paint_graph;

import java.awt.*;
import java.util.Objects;

class Node {

    private int x;
    private int y;
    private final String name;

    private final int radius;
    private Color colorNode;
    private final int textSize;

    public Node(int _x, int _y, String _name) {
        x = _x;
        y = _y;
        name = _name;
        radius = 42;
        colorNode = new Color(255,235,0);
        textSize = 20;
    }
    public Node(Node other) {
        this.x = other.x;
        this.y = other.y;
        this.name = other.name;
        this.radius = other.radius;
        this.colorNode = other.colorNode;
        this.textSize = other.textSize;
    }

    public void setColor(Color newColor){
        colorNode = newColor;
    }

    public Color getColor(){
        return colorNode;
    }

    public String getName() {
        return name;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public void setX(int x) {
        this.x = x;
    }

    public void setY(int y) {
        this.y = y;
    }

    public int getRadius() {
        return radius;
    }

    public int getTextSize(){
        return textSize;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o)
            return true;
        if (o == null || getClass() != o.getClass())
            return false;
        Node node = (Node) o;
        return Objects.equals(name, node.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name);
    }
}
