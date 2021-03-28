#include <iostream>
#include <vector>

void mult_matrix(std::vector<std::vector<int>>& matrix, std::vector<std::vector<int>>& factor, int size) {
    std::vector<std::vector<int>> ans(size, std::vector<int> (size, 0));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                ans[i][j] += matrix[i][k] * factor[k][j];
            }
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = ans[i][j];
        }
    }
}

int log_pow_matrix(std::vector<int>& current, std::vector<std::vector<int>>& matrix, int pow, int size) {
    std::vector<std::vector<int>> ans(size, std::vector<int> (size, 0));

    for (int i = 0; i < size; ++i) {
        ans[i][i] = 1;
    }

    while(pow) {
        if (pow % 2 == 1) {
            mult_matrix(ans, matrix, size);
            --pow;
        } else {
            mult_matrix(matrix, matrix, size);
            pow >>= 1;
        }
    }

    int sum_current = 0;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            sum_current += current[j] * ans[j][i];
        }
    }

    return sum_current;
}

void fill_matrix(std::vector<std::vector<int>>& matrix, int num_bits) {
    int same_prev_bits, mask1_copy, mask2_copy;

    for (int mask1 = 0; mask1 < matrix.size(); ++mask1) {
        for (int mask2 = 0; mask2 < matrix.size(); ++mask2) {
            mask1_copy = mask1; mask2_copy = mask2; same_prev_bits = -1;

            for (int cur_bit_chek = num_bits; cur_bit_chek > 0; --cur_bit_chek) {
                if ((mask1_copy & 1) == (mask2_copy & 1)) {
                    if ((mask1_copy & 1) && same_prev_bits != 1) {
                        same_prev_bits = 1;
                    } else if (!(mask1_copy & 1) && same_prev_bits != 0) {
                        same_prev_bits = 0;
                    } else {
                        matrix[mask1][mask2] = 0;
                        break;
                    }
                } else {
                    same_prev_bits = -1;
                }
                mask1_copy >>= 1; mask2_copy >>= 1;
            }
        }
    }
}

int main() {
    int m, n;

    std::cin >> m >> n;

    if (m > n) {
        std::swap(m, n);
    }

    int mask_m = (1 << m);

    std::vector<std::vector<int>> matrix(mask_m, std::vector<int> (mask_m, 1));

    fill_matrix(matrix, m);

    std::vector<int> current(mask_m, 1);

    std::cout << log_pow_matrix(current, matrix, n - 1, mask_m) << '\n';

    return 0;
}