#include <iostream>
#include <vector>

enum Colours {WHITE, GRAY, BLACK};

void fill_matrix(std::vector<std::vector<int>>& matrix, int num_edges) {
    int vert1, vert2;

    for (int i = 0; i < num_edges; ++i) {
        std::cin >> vert1 >> vert2;

        matrix[vert1 - 1].push_back(vert2 - 1);
    }
}

void out_cycle(bool& was_cycle, int beg, const std::vector<int>& parents) {
    if (!was_cycle) {
        std::cout << "YES\n";

        std::vector<int> cycle;
        cycle.push_back(beg + 1);

        int cur = parents[beg];
        while (cur != beg && cur != -1) {
            cycle.push_back(cur + 1);
            cur = parents[cur];
        }

        for (int i = cycle.size() - 1; i >= 0; --i) {
            std::cout << cycle[i] << " ";
        }

        was_cycle = true;
    }
}

void dfs(bool& was_cycle, const std::vector<std::vector<int>>& matrix, std::vector<Colours>& colour,
         std::vector<int>& tin, std::vector<int>& tout, std::vector<int>& parents, int& timer,
         int vert, int parent_vert = -1) {
    tin[vert] = timer++;

    parents[vert] = parent_vert;

    colour[vert] = GRAY;

    for (int to: matrix[vert]) {
        if (colour[to] == GRAY) {
            parents[to] = vert;
            out_cycle(was_cycle, to, parents);
            return;
        }
        if (colour[to] == WHITE) {
            dfs(was_cycle, matrix, colour, tin, tout, parents, to, vert);
        }
    }

    colour[vert] = BLACK;

    tout[vert] = timer++;

}

int main() {
    int num_vert, num_edges;

    std::cin >> num_vert >> num_edges;

    std::vector<std::vector<int>> matrix(num_vert);
    std::vector<int> tin(num_vert);
    std::vector<int> tout(num_vert);
    int timer = 0;
    std::vector<Colours> colour(num_vert, WHITE);
    std::vector<int> parents(num_vert);

    fill_matrix(matrix, num_edges);

    bool was_cycle = false;

    for (int i = 0; i < num_vert; ++i) {
        if (colour[i] == WHITE) {
            dfs(was_cycle, matrix, colour, tin, tout, parents, timer, i);
        }
    }

    if (!was_cycle) {
        std::cout << "NO";
    }

    return 0;
}