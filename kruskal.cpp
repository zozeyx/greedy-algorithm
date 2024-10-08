#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <chrono> // 실행 시간 측정을 위해 추가

using namespace std;

class UnionFind {
public:
    vector<int> parent, rank;
    
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    
    int find(int u) {
        if (u == parent[u]) return u;
        return parent[u] = find(parent[u]); // 경로 압축
    }
    
    void unite(int u, int v) {
        u = find(u);
        v = find(v);
        if (u != v) {
            if (rank[u] > rank[v]) {
                parent[v] = u;
            } else {
                parent[u] = v;
                if (rank[u] == rank[v]) ++rank[v];
            }
        }
    }
};

// 간선 (u, v, weight)
bool compareEdges(const tuple<int, int, int>& edge1, const tuple<int, int, int>& edge2) {
    return get<2>(edge1) < get<2>(edge2);
}

void kruskal(int vertices, vector<tuple<int, int, int>>& edges) {
    UnionFind uf(vertices);
    vector<tuple<int, int, int>> mst; // 최소 신장 트리
    
    // 간선을 가중치 순으로 정렬
    sort(edges.begin(), edges.end(), compareEdges);
    
    // Kruskal 알고리즘 수행
    for (auto& edge : edges) {
        int u = get<0>(edge);
        int v = get<1>(edge);
        int weight = get<2>(edge);
        
        // 사이클이 생기지 않으면 간선을 선택
        if (uf.find(u) != uf.find(v)) {
            uf.unite(u, v);
            mst.push_back(edge);
        }
    }
    
    // 최소 신장 트리 출력
    for (auto& edge : mst) {
        int u = get<0>(edge);
        int v = get<1>(edge);
        int weight = get<2>(edge);
        cout << "(" << u << ", " << v << ", " << weight << ")" << endl;
    }
}

int main() {
    // 시작 시간 기록
    auto start = chrono::high_resolution_clock::now();
    
    // (vertex a, vertex b, edge weight)
    vector<tuple<int, int, int>> edges = {
        {1, 2, 1}, {2, 5, 1}, {1, 5, 2}, {0, 3, 2}, {3, 4, 3},
        {0, 4, 4}, {1, 3, 4}, {3, 5, 7}, {0, 1, 8}, {4, 5, 9}
    };
    
    int vertices = 6; // vertex a: 0, b: 1, c: 2, d: 3, e: 4, f: 5

    // Kruskal 알고리즘 실행
    kruskal(vertices, edges);
    
    // 종료 시간 기록 및 실행 시간 계산
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Running Time: " << duration.count() << " seconds" << endl;
    
    return 0;
}
