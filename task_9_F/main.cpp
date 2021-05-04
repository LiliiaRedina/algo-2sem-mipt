#include <iostream>
#include <vector>
#include <queue>

struct Edge{
    int beg_vert;
    int end_vert;
    int weight;

    Edge(): beg_vert(-1), end_vert(-1), weight(0) {}

    Edge(int beg_vert, int end_vert, int weight):
            beg_vert(beg_vert), end_vert(end_vert), weight(weight) {}
};

struct Vertex{
    int num;
    int dist_from_beg;
    std::vector<Edge> edges;

    Vertex(): num(-1), dist_from_beg(1e9) {}

    bool operator<(const Vertex& other) const {
        return dist_from_beg > other.dist_from_beg;
    }
};

void fill_matrix(std::vector<Vertex>& matrix_graph, int num_vert, int num_edges) {
    int cin_beg, cin_end, cin_weight;

    for (int i = 0; i < num_vert; ++i) {
        matrix_graph[i].num = i;
    }

    for (int i = 0; i < num_edges; ++i) {
        std::cin >> cin_beg >> cin_end >> cin_weight;

        matrix_graph[cin_beg - 1].num = cin_beg - 1;
        matrix_graph[cin_beg - 1].edges.emplace_back(cin_beg - 1, cin_end - 1, cin_weight);
        matrix_graph[cin_end - 1].num = cin_end - 1;
        matrix_graph[cin_end - 1].edges.emplace_back(cin_end - 1, cin_beg - 1, cin_weight);
    }
}

void dijkstra(std::vector<Vertex>& matrix_graph, int beg_vert) {
    std::priority_queue<Vertex> q;
    matrix_graph[beg_vert].dist_from_beg = 0;
    q.push(matrix_graph[beg_vert]);

    while (!q.empty()) {
        Vertex v = q.top();

        q.pop();
        for (auto to: v.edges) {
            if (v.dist_from_beg + to.weight < matrix_graph[to.end_vert].dist_from_beg) {

                matrix_graph[to.end_vert].dist_from_beg = v.dist_from_beg + to.weight;
                q.push(matrix_graph[to.end_vert]);
            }
        }
    }
}

int main() {
    int num_vert, num_edges;

    std::cin >> num_vert >> num_edges;

    int beg_vert, end_vert;

    std::cin >> beg_vert >> end_vert;
    --beg_vert;
    --end_vert;

    std::vector<Vertex> matrix_graph(num_vert);

    fill_matrix(matrix_graph, num_vert, num_edges);

    dijkstra(matrix_graph, beg_vert);

    const int inf = 1e9;

    if (matrix_graph[end_vert].dist_from_beg != inf) {
        std::cout << matrix_graph[end_vert].dist_from_beg;
    } else {
        std::cout << -1;
    }

    return 0;
}