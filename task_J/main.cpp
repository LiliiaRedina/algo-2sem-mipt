#include <iostream>
#include <vector>

void max_cost(const std::vector<int> &mass_cost, std::vector<std::vector<int>> &current_max_cost,
              const int &n, const int &weight) {
    for (int i = 1; i <= n; i++) {
        for (int current_weight = 1; current_weight <= weight; current_weight++) {
            if (current_weight >= mass_cost[i - 1] && mass_cost[i - 1] +
                        current_max_cost[i - 1][current_weight - mass_cost[i - 1]] >
                        current_max_cost[i - 1][current_weight]) {
                current_max_cost[i][current_weight] = mass_cost[i - 1] +
                        current_max_cost[i - 1][current_weight - mass_cost[i - 1]];
            } else {
                current_max_cost[i][current_weight] = current_max_cost[i - 1][current_weight];
            }
        }
    }
}

int main() {
    int weight, n;
    std::cin >> weight >> n;

    std::vector<int> mass_cost(n);

    for (int i = 0; i < n; i++) {
        std::cin >> mass_cost[i];
    }

    std::vector<std::vector<int>> current_max_cost(n + 1, std::vector<int>(weight + 1, 0));

    max_cost(mass_cost, current_max_cost, n, weight);

    std::cout << current_max_cost[n][weight];

    return 0;
}
