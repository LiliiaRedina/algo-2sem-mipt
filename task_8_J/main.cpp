#include <iostream>
#include <vector>

void fill_matrix(std::vector<std::vector<int>>& matrix) {
    int num_vert = matrix.size();
    int is_i_j;

    for (int i = 0; i < num_vert; ++i) {
        for (int j = 0; j < num_vert; ++j) {
            std::cin >> is_i_j;

            if (is_i_j == 0 && i != j) {
                matrix[i].push_back(j);
            }
        }
    }
}

void euler(int vert, const std::vector<std::vector<int>>& matrix,
           std::vector<int>& num_first_unused_edge,
           std::vector<std::vector<bool>>& used_edges, std::vector<int>& euler_cycle) {
    while (num_first_unused_edge[vert] < matrix[vert].size()) {
        int next_vert = matrix[vert][num_first_unused_edge[vert]];

        if (!used_edges[vert][next_vert]) {
            used_edges[vert][next_vert] = true;
            euler(next_vert, matrix, num_first_unused_edge, used_edges, euler_cycle);
        }

        ++num_first_unused_edge[vert];
    }

    euler_cycle.push_back(vert);
}

int main() {
    int num_vert, beg_vert;

    std::cin >> num_vert >> beg_vert;

    std::vector<std::vector<int>> matrix(num_vert);
    std::vector<std::vector<bool>> used_edges(num_vert, std::vector<bool>(num_vert, false));
    std::vector<int> num_first_unused_edge(num_vert, 0);
    std::vector<int> euler_cycle;

    fill_matrix(matrix);

    euler(beg_vert - 1, matrix, num_first_unused_edge, used_edges, euler_cycle);

    for (int i = euler_cycle.size() - 1; i >= 1; --i) {
        std::cout << euler_cycle[i] + 1 << " " << euler_cycle[i - 1] + 1 << "\n";
    }

    return 0;
}