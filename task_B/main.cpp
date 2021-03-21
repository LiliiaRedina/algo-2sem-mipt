#include <iostream>
#include <vector>

int find_max_len_correct_lnis(const int& elem, const std::vector<int>& arr,
               const std::vector<int>& length_max_elem_index, int max_lnis) {
    int left = 0;
    int right = max_lnis + 1;
    int new_bord;

    while (right - left > 1) {
        new_bord = (left + right) / 2;
        if (arr[length_max_elem_index[new_bord]] < elem) {
            right = new_bord;
        } else {
            left = new_bord;
        }
    }

    return left;
}

int find_lnis(const std::vector<int>& arr, std::vector<int>& length_max_elem_index,
              std::vector<int>& arr_index_prev_elem, int& index_last_elem) {
    int max_length_for_elem;

    int max_lnis = 0;

    for (int i = 0; i < arr.size(); i++) {
        max_length_for_elem = find_max_len_correct_lnis(arr[i], arr, length_max_elem_index, max_lnis);

        length_max_elem_index[max_length_for_elem + 1] = i;

        arr_index_prev_elem[i] = length_max_elem_index[max_length_for_elem];

        if (max_length_for_elem + 1 > max_lnis) {
            max_lnis = max_length_for_elem + 1;
            index_last_elem = i;
        }
    }

    return max_lnis;
}

int main() {
    int n;
    std::cin >> n;

    std::vector<int> arr(n);
    std::vector<int> length_max_elem_index(n + 1, -1);
    std::vector<int> arr_index_prev_elem(n);

    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    int index_last_elem = -1;

    std::cout << find_lnis(arr, length_max_elem_index, arr_index_prev_elem,
                           index_last_elem) << '\n';

    std::vector<int> seq_indexes_lnis;
    int current_index = index_last_elem;

    while (current_index != -1) {
        seq_indexes_lnis.push_back(current_index);

        current_index = arr_index_prev_elem[current_index];
    }

    for (int i = seq_indexes_lnis.size() - 1; i >= 0; --i) {
        std::cout << seq_indexes_lnis[i] + 1 << " ";
    }

    return 0;
}
