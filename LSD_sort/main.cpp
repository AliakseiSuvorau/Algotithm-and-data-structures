#include <cstdint>
#include <iostream>
#include <vector>

uint64_t Digit(int number, uint64_t value) {
  return ((value >> (number * 8)) & 255llu);
}

void LSDSort(std::vector<uint64_t>& array) {
  for (int i = 0; i < 8; ++i) {
    std::vector<uint64_t> tmp_with_values(array.size());
    std::vector<uint64_t> tmp_with_order(256);
    for (unsigned int j = 0; j < array.size(); ++j) {
      uint64_t digit = Digit(i, array[j]);
      ++tmp_with_order[digit];
    }
    int count = 0;
    for (unsigned int j = 0; j < tmp_with_order.size();
         ++j) {  // сохраняем порядок
      int current = tmp_with_order[j];
      tmp_with_order[j] = count;
      count += current;
    }
    for (unsigned int j = 0; j < array.size(); ++j) {  // цифровая сортировка
      uint8_t digit = Digit(i, array[j]);
      tmp_with_values[tmp_with_order[digit]] = array[j];
      ++tmp_with_order[digit];
    }
    for (unsigned int j = 0; j < array.size(); ++j) {
      array[j] = tmp_with_values[j];
    }
  }
}

int main() {
  int size_of_input;
  std::cin >> size_of_input;
  std::vector<uint64_t> numbers(size_of_input);
  for (int i = 0; i < size_of_input; ++i) {
    std::cin >> numbers[i];
  }
  LSDSort(numbers);
  for (int i = 0; i < size_of_input; ++i) {
    std::cout << numbers[i] << "\n";
  }
  return 0;
}
