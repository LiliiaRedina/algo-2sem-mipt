#include <iostream>
#include <vector>

void fill_dp_prefix(const std::vector<int> &arr1, const std::vector<int> &arr2,
                 std::vector<std::vector<int>> &dp_prefix, const int &size1, const int &size2) {
    for (int i = 1; i < size1 + 1; i++) {
        for (int j = 1; j < size2 + 1; j++) {
            if (arr1[i - 1] == arr2[j - 1]) {
                dp_prefix[i][j] = dp_prefix[i - 1][j - 1] + 1;
            } else {
                dp_prefix[i][j] = std::max(dp_prefix[i - 1][j], dp_prefix[i][j - 1]);
            }
        }
    }
}

int main() {
    int size1, size2;

    std::cin >> size1;

    std::vector<int> arr1(size1);

    for (int i = 0; i < size1; i++) {
        std::cin >> arr1[i];
    }

    std::cin >> size2;

    std::vector<int> arr2(size2);

    for (int i = 0; i < size2; i++) {
        std::cin >> arr2[i];
    }

    std::vector<std::vector<int>> dp_prefix(size1 + 1, std::vector<int>(size2 + 1, 0));

    fill_dp_prefix(arr1, arr2, dp_prefix, size1, size2);

    std::cout << dp_prefix[size1][size2];

    return 0;
}
