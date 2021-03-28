#include <iostream>
#include <vector>

void mult_matrix(std::vector<std::vector<long long>>& matrix,
                 std::vector<std::vector<long long>>& factor, int mod) {
    std::vector<std::vector<long long>> ans(5, std::vector<long long> (5, 0));

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            for (int k = 0; k < 5; ++k) {
                ans[i][j] = (ans[i][j] + (matrix[i][k] * factor[k][j]) % mod) % mod;
            }
        }
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            matrix[i][j] = ans[i][j];
        }
    }
}

int log_pow_matrix(std::vector<long long> current, std::vector<std::vector<long long>> matrix,
                   long long pow, int mod) {
    std::vector<std::vector<long long>> ans(5, std::vector<long long> (5, 0));

    for (int i = 0; i < 5; ++i) {
        ans[i][i] = 1;
    }

    while(pow) {
        if (pow % 2 == 1) {
            mult_matrix(ans, matrix, mod);
            --pow;
        } else {
            mult_matrix(matrix, matrix, mod);
            pow >>= 1ll;
        }
    }

    int sum_current = 0;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            sum_current = (sum_current + (current[j] * ans[j][i]) % mod) % mod;
        }
    }

    return sum_current;
}

int main() {
    const int mod = 999999937;

    long long size;

    std::cin >> size;

    std::vector<std::vector<long long>> matrix(5, std::vector<long long> (5, 1));

    matrix[2][3] = 0;
    matrix[2][4] = 0;
    matrix[4][3] = 0;
    matrix[4][4] = 0;

    std::vector<long long> current(5, 1);

    while(size) {
        std::cout << log_pow_matrix(current, matrix, size - 1, mod) << '\n';

        std::cin >> size;
    }

    return 0;
}