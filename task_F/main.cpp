#include <iostream>
#include <vector>

void fill_dp(const std::vector<std::vector<int>>& matrix, std::vector<std::vector<int>>& dp_mask_and_beg,
             std::vector<std::vector<int>>& prev_vertex_in_path) {
    for (int i = 0; i < matrix.size(); ++i) {
        dp_mask_and_beg[1 << i][i] = 0;
    }

    for (int mask = 0; mask < dp_mask_and_beg.size(); ++mask) {
        for (int i = 0; i < matrix.size(); ++i) {
            if ((mask >> i) & 1) {
                for (int beg = 0; beg < matrix.size(); ++beg) {
                    if (i != beg && ((mask >> beg) & 1) &&
                        dp_mask_and_beg[mask][i] > dp_mask_and_beg[mask - (1 << i)][beg] + matrix[i][beg]) {
                        dp_mask_and_beg[mask][i] = dp_mask_and_beg[mask - (1 << i)][beg] + matrix[i][beg];
                        prev_vertex_in_path[mask][i] = beg;
                    }
                }
            }
        }
    }
}

void find_min_dist_and_beg(const std::vector<std::vector<int>>& dp_mask_and_beg, int& min_dist, int& min_beg) {
    min_dist = 1e9, min_beg = 0;
    int n = dp_mask_and_beg[0].size();
    int mask_n = dp_mask_and_beg.size();

    for (int i = 0; i < n; ++i) {
        if (min_dist > dp_mask_and_beg[mask_n - 1][i]) {
            min_dist = dp_mask_and_beg[mask_n - 1][i];
            min_beg = i;
        }
    }
}

void out_path(const std::vector<std::vector<int>>& prev_vertex_in_path, int min_beg) {
    int current_beg = min_beg;
    int prev_beg;
    int mask = prev_vertex_in_path.size() - 1;
    while (current_beg > -1) {
        std::cout << current_beg + 1 << " ";

        prev_beg = current_beg;
        current_beg = prev_vertex_in_path[mask][current_beg];
        mask -= (1 << prev_beg);
    }
}

int main() {
    int n;

    std::cin >> n;

    std::vector<std::vector<int>> matrix(n, std::vector<int> (n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> matrix[i][j];
        }
    }

    int mask_n = (1 << n);

    std::vector<std::vector<int>> dp_mask_and_beg(mask_n, std::vector<int> (n, 1e9));
    std::vector<std::vector<int>> prev_vertex_in_path(mask_n, std::vector<int> (n, -1));

    fill_dp(matrix, dp_mask_and_beg, prev_vertex_in_path);

    int min_dist, min_beg;

    find_min_dist_and_beg(dp_mask_and_beg, min_dist, min_beg);

    std::cout << min_dist << "\n";

    out_path(prev_vertex_in_path, min_beg);

    return 0;
}