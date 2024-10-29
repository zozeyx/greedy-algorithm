import java.util.*;
import java.util.PriorityQueue;

class Edge implements Comparable<Edge> {
    int vertex, weight;

    public Edge(int vertex, int weight) {
        this.vertex = vertex;
        this.weight = weight;
    }

    @Override
    public int compareTo(Edge other) {
        return this.weight - other.weight; // 가중치 기준 오름차순 정렬
    }
}

public class Prim{
    public static void primMST(List<List<Edge>> graph, int vertices) {
        boolean[] visited = new boolean[vertices];
        PriorityQueue<Edge> pq = new PriorityQueue<>();
        List<String> mst = new ArrayList<>();
        int totalWeight = 0;

        // 시작 정점을 정점 2 (c)로 설정
        pq.offer(new Edge(2, 0));

        while (!pq.isEmpty()) {
            Edge current = pq.poll();
            int u = current.vertex;

            // 이미 방문한 정점이면 넘어감
            if (visited[u]) continue;

            // 방문 처리
            visited[u] = true;
            totalWeight += current.weight;

            // MST에 추가 (가중치가 0인 경우는 처음 선택된 정점)
            if (current.weight != 0) {
                mst.add("(" + u + ", " + current.vertex + ", " + current.weight + ")");
            }

            // 인접한 간선을 우선순위 큐에 추가
            for (Edge neighbor : graph.get(u)) {
                if (!visited[neighbor.vertex]) {
                    pq.offer(neighbor);
                }
            }
        }

        // MST 출력
        for (String edge : mst) {
            System.out.println(edge);
        }
    }

    public static void main(String[] args) {
        long startTime = System.nanoTime(); // 시작 시간 기록

        // 그래프 입력 (정점 a -> b, 가중치 c)
        int vertices = 6; // 정점 수 a:0, b:1, c:2, d:3, e:4, f:5

        List<List<Edge>> graph = new ArrayList<>();
        for (int i = 0; i < vertices; i++) {
            graph.add(new ArrayList<>());
        }

        // (정점 a -> b, 가중치 c) 형식으로 간선 추가
        graph.get(0).add(new Edge(1, 3));
        graph.get(0).add(new Edge(3, 2));
        graph.get(0).add(new Edge(4, 4));
        graph.get(1).add(new Edge(2, 1));
        graph.get(1).add(new Edge(3, 4));
        graph.get(2).add(new Edge(5, 1));
        graph.get(3).add(new Edge(0, 2));
        graph.get(3).add(new Edge(4, 3));
        graph.get(3).add(new Edge(5, 7));
        graph.get(4).add(new Edge(0, 4));
        graph.get(4).add(new Edge(3, 3));
        graph.get(4).add(new Edge(5, 9));
        graph.get(5).add(new Edge(2, 1));
        graph.get(5).add(new Edge(3, 7));
        graph.get(5).add(new Edge(4, 9));

        // Prim 알고리즘 실행 (시작점을 2로 설정)
        primMST(graph, vertices);

        long endTime = System.currentTimeMillis(); // 종료 시간 기록
        long duration = (endTime - startTime); // 실행 시간 계산
        System.out.println("Running Time: " + duration / 1000.0 + " s");
    }
}
