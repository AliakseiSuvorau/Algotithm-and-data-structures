#include <algorithm>
#include <iostream>
#include <vector>

class SparseTable {
  std::vector<std::vector<std::pair<size_t, int>>> table_;
  size_t size_ = 0;
  static int Log(int x);

 public:
  SparseTable(std::vector<int> numbers);
  ~SparseTable() = default;
  std::pair<size_t, int> GetMin(size_t l, size_t r);
};

SparseTable::SparseTable(std::vector<int> numbers) {
  size_ = numbers.size();
  int log = Log(int(size_));
  table_.resize(log + 1);
  for (int i = 0; i <= log; ++i) {
    table_[i].resize(size_);
  }
  for (size_t i = 0; i < size_; ++i) {
    table_[0][i] = {i, numbers[i]};
  }
  for (int i = 1; i <= log; ++i) {
    for (size_t j = 0; j + (1 << i) <= size_; ++j) {
      size_t k = j + (1 << (i - 1));
      std::pair<size_t, int> min_on_segment =
          table_[i - 1][j].second <= table_[i - 1][k].second ? table_[i - 1][j]
                                                             : table_[i - 1][k];
      table_[i][j] = min_on_segment;
    }
  }
}

int SparseTable::Log(int x) {
  int log = 0;
  while (x / 2 > 0) {
    x /= 2;
    ++log;
  }
  return log;
}

std::pair<size_t, int> SparseTable::GetMin(size_t l, size_t r) {
  int k = Log(int(r - l + 1));
  return table_[k][l].second < table_[k][r + 1 - (1 << k)].second
             ? table_[k][l]
             : table_[k][r + 1 - (1 << k)];
}

int FindSecondStat(SparseTable& table, size_t l, size_t r) {
  size_t min_index = table.GetMin(l, r).first;
  if (min_index == l) {
    return table.GetMin(min_index + 1, r).second;
  }
  if (min_index == r) {
    return table.GetMin(l, min_index - 1).second;
  }
  int second_stat1 = table.GetMin(l, min_index - 1).second;
  int second_stat2 = table.GetMin(min_index + 1, r).second;
  return std::min(second_stat1, second_stat2);
}

int main() {
  int size_of_input = 0;
  int number_of_requests = 0;
  std::cin >> size_of_input >> number_of_requests;
  std::vector<int> input(size_of_input);
  for (int i = 0; i < size_of_input; ++i) {
    std::cin >> input[i];
  }
  SparseTable table(input);
  for (int i = 0; i < number_of_requests; ++i) {
    size_t left_border = 0;
    size_t right_border = 0;
    std::cin >> left_border >> right_border;
    std::cout << FindSecondStat(table, left_border - 1, right_border - 1)
              << "\n";
  }
  return 0;
}
