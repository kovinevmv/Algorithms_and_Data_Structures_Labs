package business_logic;

public class Graph {
    private final int[][] indMatrix;
    private final String[] nameList;

    public Graph(int[][] indMatrix, String[] nameList) {
        this.indMatrix = indMatrix;
        this.nameList = nameList;
    }

    public int[][] getIndMatrix() {
        return this.indMatrix;
    }

    public String[] getNameList() {
        return this.nameList;
    }
}