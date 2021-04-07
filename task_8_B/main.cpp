#include <iostream>
#include <vector>

void fill_matrix(std::vector<std::vector<int>>& matrix, int& root) {
    int parent;

    for (int i = 0; i < matrix.size(); ++i) {
        std::cin >> parent;
        if (parent) {
            matrix[parent - 1].push_back(i);
        } else {
            root = i;
        }
    }
}

void ask_requests(const std::vector<int>& tin, const std::vector<int>& tout) {
    int num_requests, vert1, vert2;

    std::cin >> num_requests;

    for (int i = 0; i < num_requests; ++i) {
        std::cin >> vert1 >> vert2;

        if (tin[vert1 - 1] < tin[vert2 - 1] && tout[vert1 - 1] > tout[vert2 - 1]) {
            std::cout << 1 << "\n";
        } else {
            std::cout << 0 << "\n";
        }
    }
}

void dfs(int vert, const std::vector<std::vector<int>>& matrix, std::vector<bool>& is_used,
         std::vector<int>& tin, std::vector<int>& tout, int& timer) {
    tin[vert] = timer++;

    is_used[vert] = true;

    for (int to: matrix[vert]) {
        if (!is_used[to]) {
            dfs(to, matrix, is_used, tin, tout, timer);
        }
    }

    tout[vert] = timer++;
}

int main() {
    int num_vert;

    std::cin >> num_vert;

    std::vector<std::vector<int>> matrix(num_vert);
    std::vector<int> tin(num_vert);
    std::vector<int> tout(num_vert);
    int timer = 0;
    std::vector<bool> is_used(num_vert, false);

    int root;

    fill_matrix(matrix, root);

    dfs(root, matrix, is_used, tin, tout, timer);

    ask_requests(tin, tout);

    return 0;
}