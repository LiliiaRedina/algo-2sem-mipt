#include <iostream>
#include <vector>

void fill_matrix(std::vector<std::vector<int>>& matrix_graph, int num_edges) {
    int input_vert1, input_vert2, input_weight;

    for (int i = 0; i < num_edges; ++i) {
        std::cin >> input_vert1 >> input_vert2 >> input_weight;

        matrix_graph[input_vert1 - 1][input_vert2 - 1] = input_weight;
        matrix_graph[input_vert2 - 1][input_vert1 - 1] = input_weight;
    }
}

int algo_prim(const std::vector<std::vector<int>>& matrix_graph, int inf) {
    int num_vert = matrix_graph.size();

    std::vector<bool> is_using(num_vert, false);
    std::vector<int> min_cost(num_vert, inf);

    int last_add;
    int min_cost_for_add = inf;
    int new_vert_for_add = 0;
    int common_cost_skel_graph = 0;

    is_using[0] = true;

    last_add = 0;

    for (int i = 0; i < num_vert - 1; ++i) {
        for (int vert = 0; vert < num_vert; ++vert) {
            if (is_using[vert]) {
                continue;
            }
            if (min_cost[vert] > matrix_graph[last_add][vert]) {
                min_cost[vert] = matrix_graph[last_add][vert];
            }
            if (min_cost_for_add > min_cost[vert]) {
                min_cost_for_add = min_cost[vert];
                new_vert_for_add = vert;
            }
        }

        is_using[new_vert_for_add] = true;
        last_add = new_vert_for_add;
        common_cost_skel_graph += min_cost_for_add;

        min_cost_for_add = inf;
    }

    return common_cost_skel_graph;
}

int main() {
    const int inf = 1e9;
    int num_vert, num_edges;

    std::cin >> num_vert >> num_edges;

    std::vector<std::vector<int>> matrix_graph(num_vert, std::vector<int>(num_vert, inf));

    fill_matrix(matrix_graph, num_edges);

    std::cout << algo_prim(matrix_graph, inf);
}