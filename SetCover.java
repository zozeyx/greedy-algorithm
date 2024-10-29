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
        connections.add(link);
    }

    public void initializeNodes() {
        for (Link link : connections) {
            allNodes.add(link.node1);
            allNodes.add(link.node2);
        }
        nodeList = new ArrayList<>(allNodes);
    }

    public void generateSets() {
        nodeGroups = new ArrayList<>();

        for(String node : nodeList) {
            List<String> subset = new ArrayList<>();

            for(Link link : connections) {
                if(link.node1.equals(node)) {
                    subset.add(link.node2);
                }
                else if(link.node2.equals(node)) {
                    subset.add(link.node1);
                }
            }
            subset.add(node);
            nodeGroups.add(subset);
        }
    }

    public List<List<String>> approximateSetCover() {
        List<List<String>> selectedGroups = new ArrayList<>();
        Set<String> uncoveredNodes = new HashSet<>(allNodes);

        while(!uncoveredNodes.isEmpty()) {
            int maxCoverage = 0;
            List<String> bestGroup = null;

            for(List<String> group : nodeGroups) {
                int coveredNodes = 0;
                
                for(String node : uncoveredNodes) {
                    if(group.contains(node)) coveredNodes++;
                }
                if(maxCoverage < coveredNodes) {
                    bestGroup = group;
                    maxCoverage = coveredNodes;
                }
            }
            if(bestGroup != null) {
                uncoveredNodes.removeAll(bestGroup);
                selectedGroups.add(bestGroup);
                nodeGroups.remove(bestGroup);
            }
        }

        return selectedGroups;
    }

    public List<List<String>> optimalSetCover() {
        Set<String> coveredNodes = null;
        int groupSize = nodeGroups.size();

        for(int i = 1; i <= groupSize; i++) {
            List<List<List<String>>> groupCombinations = getCombinations(nodeGroups, i);

            for(List<List<String>> groupCombo : groupCombinations) {
                coveredNodes = new HashSet<>();

                for(List<String> subset : groupCombo) {
                    coveredNodes.addAll(subset);
                }

                if(coveredNodes.containsAll(allNodes)) {
                    return groupCombo;
                }
            }
        }

        return null;
    }

    public List<List<List<String>>> getCombinations(List<List<String>> nodeGroups, int size) {
        List<List<List<String>>> result = new ArrayList<>();
        findCombinations(nodeGroups, new ArrayList<>(), 0, size, result);

        return result;
    }

    private void findCombinations(List<List<String>> nodeGroups, List<List<String>> current, int start, int size, List<List<List<String>>> result) {
        if(current.size() == size) {
            result.add(new ArrayList<>(current));
            return;
        }

        for(int i = start; i < nodeGroups.size(); i++) {
            current.add(nodeGroups.get(i));
            findCombinations(nodeGroups, current, i + 1, size, result);
            current.remove(current.size() - 1);
        }
    }
}
