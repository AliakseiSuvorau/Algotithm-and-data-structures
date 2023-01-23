#include <iostream>
#include <vector>

int QuickSelect(int begin, int end) { return begin + (rand() % (end - begin)); }

void MergeVectors(std::vector<int>& less, std::vector<int>& equal,
                  std::vector<int>& greater, std::vector<int>& sorted,
                  int begin) {
  for (unsigned int i = 0; i < less.size(); ++i) {
    sorted[begin + i] = less[i];
  }
  unsigned int less_size = less.size();
  for (unsigned int i = 0; i < equal.size(); ++i) {
    sorted[begin + less_size + i] = equal[i];
  }
  unsigned int equal_size = equal.size();
  for (unsigned int i = 0; i < greater.size(); ++i) {
    sorted[begin + less_size + equal_size + i] = greater[i];
  }
}

void QuickSort(std::vector<int>& numbers, int begin, int end);

std::pair<int, int> Partition(std::vector<int>& numbers, int pivot, int begin,
                              int end) {
  std::vector<int> less;
  std::vector<int> equal;
  std::vector<int> greater;
  for (int i = begin; i <= end; ++i) {
    if (numbers[i] < pivot) {
      less.push_back(numbers[i]);
    } else {
      if (numbers[i] == pivot) {
        equal.push_back(numbers[i]);
      } else {
        greater.push_back(numbers[i]);
      }
    }
  }
  MergeVectors(less, equal, greater, numbers, begin);
  return std::make_pair((int)less.size() + begin,
                        (int)(less.size() + equal.size()) + begin);
}

void QuickSort(std::vector<int>& numbers, int begin, int end) {
  if (begin < end) {
    int pivot_index = QuickSelect(begin, end);
    std::pair pair = Partition(numbers, numbers[pivot_index], begin, end);
    QuickSort(numbers, begin, pair.first - 1);
    QuickSort(numbers, pair.second, end);
  }
}

int main() {
  int size_of_input;
  std::cin >> size_of_input;
  std::vector<int> numbers;
  for (int i = 0; i < size_of_input; ++i) {
    int value;
    std::cin >> value;
    numbers.push_back(value);
  }
  QuickSort(numbers, 0, (int)numbers.size() - 1);
  for (unsigned int i = 0; i < numbers.size(); ++i) {
    std::cout << numbers[i] << " ";
  }
  return 0;
}
