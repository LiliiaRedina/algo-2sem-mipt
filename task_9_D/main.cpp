#include <iostream>
#include <vector>

struct Edge{
    int beg_vert;
    int end_vert;
    long long weight;

    Edge(): beg_vert(-1), end_vert(-1), weight(0) {}

    Edge(int beg_vert, int end_vert, long long weight):
            beg_vert(beg_vert), end_vert(end_vert), weight(weight) {}
};

struct Vertex{
    int num;
    long long dist_from_beg;
    std::vector<Edge> edges;

    Vertex(): num(-1), dist_from_beg(9e18) {}

    bool operator<(const Vertex& other) const {
        return dist_from_beg > other.dist_from_beg;
    }
};

void dfs(std::vector<Vertex>& matrix_graph, std::vector<bool>& is_used, int vert) {
    is_used[vert] = true;
    matrix_graph[vert].dist_from_beg = -9e18;

    for (auto to: matrix_graph[vert].edges) {
        if (!is_used[to.end_vert]) {
            dfs(matrix_graph, is_used, to.end_vert);
        }
    }
}

void fill_matrix(std::vector<Vertex>& matrix_graph, std::vector<Vertex>& reverse_matrix_graph,
                 int num_vert, int num_edges) {
    int cin_beg, cin_end;
    long long cin_weight;

    for (int i = 0; i < num_vert; ++i) {
        matrix_graph[i].num = i;
        reverse_matrix_graph[i].num = i;
    }

    for (int i = 0; i < num_edges; ++i) {
        std::cin >> cin_beg >> cin_end >> cin_weight;

        matrix_graph[cin_beg - 1].edges.emplace_back(cin_beg - 1, cin_end - 1, cin_weight);
        reverse_matrix_graph[cin_end - 1].edges.emplace_back(cin_end - 1, cin_beg - 1, cin_weight);
    }
}

void floyd(const std::vector<Vertex>& reverse_matrix_graph, std::vector<std::vector<long long>>& dp,
           std::vector<int>& parents, int num_vert, int num_edges, int beg_vert, long long inf,
           int& vert_path_cycle) {
    std::vector<int> new_parents(num_vert, -1);

    for (int i = 0; i < num_vert; ++i) {
        dp[i][0] = beg_vert == i ? 0 : inf;
    }

    parents[beg_vert] = beg_vert;

    for (int k = 1; k <= num_edges; ++k) {
        for (int i = 0; i < num_vert; ++i) {
            long long min_new_path = inf;
            int prev_vert = -1;

            for (auto to: reverse_matrix_graph[i].edges) {
                if (dp[to.end_vert][(k - 1) % 2] != inf && min_new_path > dp[to.end_vert][(k - 1) % 2] + to.weight) {
                    min_new_path = dp[to.end_vert][(k - 1) % 2] + to.weight;
                    prev_vert = to.end_vert;
                }
            }

            if (min_new_path < dp[i][(k - 1) % 2]) {
                dp[i][k % 2] = min_new_path;
                new_parents[i] = prev_vert;
            } else {
                dp[i][k % 2] = dp[i][(k - 1) % 2];
                new_parents[i] = parents[i];
            }
        }

        for (int i = 0; i < num_vert; ++i) {
            parents[i] = new_parents[i];
            new_parents[i] = -1;
        }
    }

    for (int i = 0; i < num_vert; ++i) {
        for (auto to: reverse_matrix_graph[i].edges) {
            if (dp[to.end_vert][num_edges % 2] != inf && dp[i][num_edges % 2] > dp[to.end_vert][num_edges % 2] + to.weight) {
                vert_path_cycle = i;
                break;
            }
        }
    }
}

void fill_dist_from_beg_in_matrix(std::vector<Vertex>& matrix_graph,
                                  const std::vector<std::vector<long long>>& dp, int num_vert, int num_edges) {
    for (int i = 0; i < num_vert; ++i) {
        matrix_graph[i].dist_from_beg = dp[i][num_edges % 2];
    }
}

void find_cycle(std::vector<Vertex>& matrix_graph, std::vector<int>& parents,
                int num_vert, int num_edges, int vert_path_cycle) {
    std::vector<bool> is_used(num_vert, false);

    if (vert_path_cycle != -1) {
        for (int i = 0; i < num_edges; ++i) {
            vert_path_cycle = parents[vert_path_cycle];
        }

        dfs(matrix_graph, is_used, vert_path_cycle);
    }
}

int main() {
    int num_vert, num_edges;

    std::cin >> num_vert >> num_edges;

    int beg_vert;

    std::cin >> beg_vert;
    --beg_vert;

    std::vector<Vertex> matrix_graph(num_vert);
    std::vector<Vertex> reverse_matrix_graph(num_vert);

    fill_matrix(matrix_graph, reverse_matrix_graph, num_vert, num_edges);

    const long long inf = 9e18;

    std::vector<std::vector<long long>> dp(num_vert, std::vector<long long>(2, inf));
    std::vector<int> parents(num_vert, -1);

    int vert_path_cycle = -1;

    floyd(reverse_matrix_graph, dp, parents, num_vert, num_edges, beg_vert, inf, vert_path_cycle);

    fill_dist_from_beg_in_matrix(matrix_graph, dp, num_vert, num_edges);

    find_cycle(matrix_graph, parents, num_vert, num_edges, vert_path_cycle);

    for (int i = 0; i < num_vert; ++i) {
        if (matrix_graph[i].dist_from_beg == inf) {
            std::cout << "*\n";
        } else if (matrix_graph[i].dist_from_beg == -inf) {
            std::cout << "-\n";
        } else {
            std::cout << matrix_graph[i].dist_from_beg << "\n";
        }
    }

    return 0;
}