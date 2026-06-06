#include "lambda.h"

#include <algorithm>
#include <iostream>

void init_globals() {
    // keep only evens - vector by value
    by_value = +[](std::vector<int> vec) -> std::vector<int> {
        std::vector<int> even_values;
        for (std::size_t i = 0; i < vec.size(); ++i) {
            if ((vec[i] % 2) == 0) {
                even_values.push_back(vec[i]);
            }
        }
        return even_values;
    };

    // keep only evens - vector by ref
    by_ref = +[](std::vector<int>& vec) -> std::vector<int> {
        std::vector<int> even_values;
        for (std::size_t i = 0; i < vec.size(); ++i) {
            if ((vec[i] % 2) == 0) {
                even_values.push_back(vec[i]);
            }
        }
        return even_values;
    };

    // compare abs and count
    cmp_lambda = +[](int& a, int& b) -> bool {
        ++g_counter;
        int abs_a = (a < 0) ? -a : a;
        int abs_b = (b < 0) ? -b : b;
        return abs_a < abs_b;
    };
}

// sort and print
std::vector<int> ascending_sort(std::vector<int>& vec) {
    init_globals();
    g_counter = 0;
    std::sort(vec.begin(), vec.end(), cmp_lambda);
    std::cout << g_counter << std::endl;
    return vec;
}
