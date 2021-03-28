#include <iostream>
#include <vector>
#include <string>

void mult_matrix(std::vector<std::vector<int>>& matrix, std::vector<std::vector<int>> factor,
                 int size, int pow, int mod);

void log_2_pow_matrix(std::vector<std::vector<int>>& matrix, int pow, int size, int mod) {
    std::vector<std::vector<int>> ans(size, std::vector<int> (size, 0));

    for (int i = 0; i < size; ++i) {
        ans[i][i] = 1;
    }

    while(pow) {
        if (pow % 2 == 1) {
            mult_matrix(ans, matrix, size, 1 , mod);
            --pow;
        } else {
            mult_matrix(matrix, matrix, size, 1, mod);
            pow >>= 1;
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = ans[i][j];
        }
    }
}

void mult_matrix(std::vector<std::vector<int>>& matrix, std::vector<std::vector<int>> factor,
                 int size, int pow, int mod) {
    std::vector<std::vector<int>> ans(size, std::vector<int> (size, 0));

    if (pow > 1) {
        log_2_pow_matrix(factor, pow, size, mod);
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                ans[i][j] = (ans[i][j] + (matrix[i][k] * factor[k][j]) % mod) % mod;
            }
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = ans[i][j];
        }
    }
}

int log_10_pow_matrix(std::vector<int> current, std::vector<std::vector<int>> matrix,
                      std::string& pow, int size, int mod) {
    std::vector<std::vector<int>> ans(size, std::vector<int> (size, 0));

    for (int i = 0; i < size; ++i) {
        ans[i][i] = 1;
    }

    int size_pow = pow.size();

    for (int i = size_pow; i > 0; --i) {
        if (pow[i - 1] != '0') {
            mult_matrix(ans, matrix, size, pow[i - 1] - '0', mod);
            log_2_pow_matrix(matrix, 10, size, mod);
        } else if (i != 1){
            log_2_pow_matrix(matrix, 10, size, mod);
        }
    }

    int ret = 0;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            ret = (ret + (current[j] * ans[j][i]) % mod) % mod;
        }
    }

    return ret;
}

void string_minus_1(std::string& str) {
    int last_nonzero_digit = str.size() - 1;

    while (str[last_nonzero_digit] == '0') {
        --last_nonzero_digit;
    }

    str[last_nonzero_digit] -= 1;

    for (int i = last_nonzero_digit + 1; i < str.size(); ++i) {
        str[i] = '9';
    }
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
    int m, mod;
    std::string n;

    std::cin >> n >> m >> mod;

    string_minus_1(n);

    int mask_m = 1 << m;

    std::vector<std::vector<int>> matrix(mask_m, std::vector<int> (mask_m, 1));

    fill_matrix(matrix, m);

    std::vector<int> current(mask_m, 1);

    std::cout << log_10_pow_matrix(current, matrix, n, mask_m, mod) << '\n';

    return 0;
}