import java.util.*;

public class SetCoverSolver {
    public static void main(String[] args) {
        Network graph = new Network();

        // 직접 입력된 데이터로 그래프 생성
        String[][] data = {
            {"1", "8"}, {"1", "2"}, {"1", "3"},
            {"2", "8"}, {"2", "4"}, {"2", "3"},
            {"3", "4"}, {"8", "4"}, {"4", "7"},
            {"4", "5"}, {"7", "5"}, {"7", "6"},
            {"5", "6"}, {"6", "9"}, {"6", "10"}
        };

        for (String[] edgeData : data) {
            String node1 = edgeData[0];
            String node2 = edgeData[1];
            Link link = new Link(node1, node2);
            graph.addLink(link);
        }

        graph.initializeNodes();
        graph.generateSets();
        
        long startTime = System.currentTimeMillis();
        List<List<String>> coverSolution = graph.approximateSetCover();
        long endTime = System.currentTimeMillis();

        System.out.println("Approximate Set Cover:");
        for (List<String> group : coverSolution) {
            System.out.println(group);
        }
        System.out.println("Approximate Set Cover Time (ms): " + (endTime - startTime));

        System.out.println();

        graph.generateSets();
        startTime = System.currentTimeMillis();
        coverSolution = graph.optimalSetCover();
        endTime = System.currentTimeMillis();

        System.out.println("Optimal Set Cover:");
        for (List<String> group : coverSolution) {
            System.out.println(group);
        }
        System.out.println("Optimal Set Cover Time (ms): " + (endTime - startTime));
    }
}

class Link {
    String node1;
    String node2;

    public Link(String node1, String node2) {
        this.node1 = node1;
        this.node2 = node2;
    }

    @Override
    public String toString() {
        return "(" + node1 + ", " + node2 + ")";
    }
}

class Network {
    List<Link> connections;
    Set<String> allNodes;
    List<String> nodeList;
    List<List<String>> nodeGroups;

    public Network() {
        connections = new ArrayList<>();
        allNodes = new HashSet<>();
    }

    public void addLink(Link link) {
        connections.add
