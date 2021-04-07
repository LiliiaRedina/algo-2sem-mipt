#include <iostream>
#include <vector>
#include <algorithm>

void fill_matrix(std::vector<std::vector<int>>& matrix, int num_edges) {
    int vert1, vert2;

    for (int i = 0; i < num_edges; ++i) {
        std::cin >> vert1 >> vert2;

        matrix[vert1 - 1].push_back(vert2 - 1);
        matrix[vert2 - 1].push_back(vert1 - 1);
    }
}

void dfs(const std::vector<std::vector<int>>& matrix, std::vector<int>& matrix_point,
         std::vector<bool>& is_used, std::vector<int>& tin, std::vector<int>& ret, int& timer,
         int vert, int parent_vert = -1) {
    tin[vert] = timer++;
    ret[vert] = tin[vert];

    int num_children = 0;

    is_used[vert] = true;

    for (int to: matrix[vert]) {
        if (to != parent_vert && is_used[to]) {
            ret[vert] = std::min(ret[vert], tin[to]);
        } else if (!is_used[to]) {
            ++num_children;
            dfs(matrix, matrix_point, is_used, tin, ret, timer, to, vert);

            if ((parent_vert != -1 && ret[to] >= tin[vert]) || (parent_vert == -1 && num_children > 1)) {
                if (std::find(matrix_point.begin(), matrix_point.end(), vert) == matrix_point.end()) {
                    matrix_point.push_back(vert);
                }
            }

            ret[vert] = std::min(ret[vert], ret[to]);
        }
    }
}

int main() {
    int num_vert, num_edges;

    std::cin >> num_vert >> num_edges;

    std::vector<std::vector<int>> matrix(num_vert);
    std::vector<int> matrix_point;
    std::vector<int> tin(num_vert);
    int timer = 0;
    std::vector<bool> is_used(num_vert, false);
    std::vector<int> ret(num_vert);

    fill_matrix(matrix, num_edges);

    for (int i = 0; i < num_vert; ++i) {
        if (!is_used[i]) {
            dfs(matrix, matrix_point, is_used, tin, ret, timer, i);
        }
    }

    std::cout << matrix_point.size() << "\n";

    std::sort(matrix_point.begin(), matrix_point.end());

    for (int to: matrix_point) {
        std::cout << to + 1 << " ";
    }

    return 0;
}