#include <iostream>
#include <vector>

struct Field {
  int size_of_subs = 0;
  size_t i_pr = 0;
  size_t j_pr = 0;
};

std::vector<int> ReadSubsequence() {
  int size_of_subsequence = 0;
  std::cin >> size_of_subsequence;
  std::vector<int> first_subsequence(size_of_subsequence);
  for (int i = 0; i < size_of_subsequence; ++i) {
    std::cin >> first_subsequence[i];
  }
  return first_subsequence;
}

void FillTable(std::vector<std::vector<Field>>& table,
               std::vector<int>& first_subs, std::vector<int>& second_subs) {
  size_t vert_size = first_subs.size() + 1;
  size_t hor_size = second_subs.size() + 1;
  for (size_t i = 1; i < vert_size; ++i) {
    for (size_t j = 1; j < hor_size; ++j) {
      if (first_subs[i - 1] == second_subs[j - 1]) {
        table[i][j].size_of_subs = table[i - 1][j - 1].size_of_subs + 1;
        table[i][j].i_pr = i - 1;
        table[i][j].j_pr = j - 1;
      } else {
        table[i][j].size_of_subs =
            (table[i - 1][j].size_of_subs > table[i][j - 1].size_of_subs)
                ? table[i - 1][j].size_of_subs
                : table[i][j - 1].size_of_subs;
        table[i][j].i_pr =
            (table[i - 1][j].size_of_subs > table[i][j - 1].size_of_subs)
                ? i - 1
                : i;
        table[i][j].j_pr =
            (table[i - 1][j].size_of_subs > table[i][j - 1].size_of_subs)
                ? j
                : j - 1;
      }
    }
  }
}

std::vector<int> FindMaxSubsequence(std::vector<std::vector<Field>>& table,
                                    std::vector<int>& first_subs) {
  std::vector<int> max_subsequence;
  size_t i = table.size() - 1;
  size_t j = table[0].size() - 1;
  while (i > 0 && j > 0) {
    size_t i_pr = table[i][j].i_pr;
    size_t j_pr = table[i][j].j_pr;
    if (table[i][j].size_of_subs > table[i_pr][j_pr].size_of_subs) {
      max_subsequence.push_back(first_subs[i - 1]);
    }
    i = i_pr;
    j = j_pr;
  }
  return max_subsequence;
}

int main() {
  std::vector<int> first_subs = ReadSubsequence();
  std::vector<int> second_subs = ReadSubsequence();
  std::vector<std::vector<Field>> length_of_max_subs(
      first_subs.size() + 1, std::vector<Field>(second_subs.size() + 1));
  FillTable(length_of_max_subs, first_subs, second_subs);
  std::vector<int> max_subsequence =
      FindMaxSubsequence(length_of_max_subs, first_subs);
  for (int i = int(max_subsequence.size()) - 1; i >= 0; --i) {
    std::cout << max_subsequence[i] << " ";
  }
  return 0;
}
