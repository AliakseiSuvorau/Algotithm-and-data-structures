#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
///                 REDUCTION RMQ PROBLEM TO LCA PROBLEM                     ///
///                             SPARSE TABLE                                 ///
///               O(NlogN) - PREPROCESSING, O(1) - REQUEST                   ///
////////////////////////////////////////////////////////////////////////////////

template < typename T>
class SparseTable {
  std::vector<std::vector<std::pair<size_t, T>>> table_;
  size_t size_ = 0;
  static int Log(int x);

 public:
  SparseTable() = default;
  explicit SparseTable(std::vector<T> numbers);
  ~SparseTable() = default;
  std::pair<size_t, T> GetMin(size_t l, size_t r);
};

template <typename T>
SparseTable<T>::SparseTable(std::vector<T> numbers) {
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
      std::pair<size_t, T> min_on_segment =
          table_[i - 1][j].second <= table_[i - 1][k].second ? table_[i - 1][j]
                                                             : table_[i - 1][k];
      table_[i][j] = min_on_segment;
    }
  }
}

template <typename T>
int SparseTable<T>::Log(int x) {
  int log = 0;
  while (x / 2 > 0) {
    x /= 2;
    ++log;
  }
  return log;
}

template <typename T>
std::pair<size_t, T> SparseTable<T>::GetMin(size_t l, size_t r) {
  int k = Log(int(r - l + 1));
  return table_[k][l].second < table_[k][r + 1 - (1 << k)].second
         ? table_[k][l]
         : table_[k][r + 1 - (1 << k)];
}

class Graph {
  size_t num_of_vertexes_ = 0;
  std::vector<std::vector<size_t>> graph_;
  std::vector<size_t> tin_;
  std::vector<size_t> depth_;
  std::vector<bool> used_;
  size_t timer_ = 0;
  SparseTable<size_t> st_;
  std::vector<size_t> euler_tour_tree_;

  void Dfs(size_t vertex, size_t depth = 0) {
    used_[vertex] = true;
    tin_[vertex] = timer_;
    for (auto& child : graph_[vertex]) {
      if (used_[child]) {
        continue;
      }
      depth_.push_back(depth);
      euler_tour_tree_.push_back(vertex);
      ++timer_;
      Dfs(child, depth + 1);
    }
    depth_.push_back(depth);
    euler_tour_tree_.push_back(vertex);
    ++timer_;
  }

 public:
  explicit Graph(size_t num_of_vertexes) : num_of_vertexes_(num_of_vertexes) {
    graph_.resize(num_of_vertexes_);
    used_.resize(num_of_vertexes_);
    tin_.resize(num_of_vertexes_);
    for (size_t i = 0; i < num_of_vertexes_ - 1; ++i) {
      size_t vertex = 0;
      std::cin >> vertex;
      graph_[vertex].push_back(i + 1);
      graph_[i + 1].push_back(vertex);
    }
  }

  void Preprocessing() {
    Dfs(0);
    st_ = SparseTable(depth_);
  }

  size_t Lca(size_t from, size_t into) {
    if (tin_[from] >= tin_[into]) {
      return euler_tour_tree_[st_.GetMin(tin_[into], tin_[from]).first];
    }
    return euler_tour_tree_[st_.GetMin(tin_[from], tin_[into]).first];
  }
};

void FastInput() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}

int main() {
  FastInput();
  size_t num_of_vertexes = 0;
  std::cin >> num_of_vertexes;
  size_t num_of_requests = 0;
  std::cin >> num_of_requests;
  Graph graph(num_of_vertexes);
  graph.Preprocessing();
  size_t first = 0;
  size_t second = 0;
  size_t x = 0;
  size_t y = 0;
  size_t z = 0;
  std::cin >> first >> second >> x >> y >> z;
  size_t ans = 0;
  uint64_t sum = 0;
  for (size_t i = 0; i < num_of_requests; ++i) {
    ans = graph.Lca((first + ans) % num_of_vertexes, second);
    sum += ans;
    first = (x * first + y * second + z) % num_of_vertexes;
    second = (x * second + y * first + z) % num_of_vertexes;
  }
  std::cout << sum;
  return 0;
}
