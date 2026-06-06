#include "sort.h"
#include <iostream>

//  CORRECT VERSION
void serialQuicksort(QSVector::iterator b, QSVector::iterator e)
{
    if (b >= e)
        return;

    // do shuffle
    int pivot_value = *b;
    QSVector::iterator i = b, j = e - 1;
    while (i != j)
    {
        while (i != j && pivot_value < *j)
            --j;
        while (i != j && *i <= pivot_value)
            ++i;
        std::iter_swap(i, j);
    }
    std::iter_swap(b, i);

    // recursive call
    serialQuicksort(b, i);
    serialQuicksort(i + 1, e);
}

void printVector(const QSVector &v)
{
    for (auto e : v)
    {
        std::cout << e << std::endl;
    }
    std::cout << std::endl;
}


std::vector<int> get_vector() {
    std::vector<int> numbers;
    int number;

    // Read integers one by one until EOF
    while (std::cin >> number) {
        numbers.push_back(number);
    }

    return numbers;
}

int main()
{
    auto v = get_vector();
    serialQuicksort(v.begin(), v.end());
    printVector(v);
}