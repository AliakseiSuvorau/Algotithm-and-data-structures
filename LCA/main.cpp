#include <cmath>
#include <iostream>
#include <vector>

///////////////////////////////////
///             LCA             ///
///////////////////////////////////

class Graph {
  size_t num_of_vertexes_ = 0;
  size_t degree_ = 0;
  std::vector<std::vector<size_t>> graph_;
  std::vector<size_t> parent_;
  std::vector<size_t> depth_;
  std::vector<bool> used_;
  std::vector<std::vector<size_t>> binary_lift_;

  void Dfs(size_t vertex, size_t depth = 0) {
    used_[vertex] = true;
    depth_[vertex] = depth;
    for (auto& child : graph_[vertex]) {
      if (used_[child]) {
        continue;
      }
      parent_[child] = vertex;
      Dfs(child, depth + 1);
    }
  }

  size_t Lca(size_t from, size_t into) {
    if (depth_[from] > depth_[into]) {
      std::swap(from, into);
    }
    for (int i = static_cast<int>(degree_ - 1); i >= 0; --i) {
      if (depth_[binary_lift_[into][i]] >= depth_[from]) {
        into = binary_lift_[into][i];
      }
    }
    // now vertexes are on the same depth
    if (from == into) {
      return from;
    }
    for (int i = static_cast<int>(degree_ - 1); i >=0; --i) {
      if (binary_lift_[from][i] != binary_lift_[into][i]) {
        from = binary_lift_[from][i];
        into = binary_lift_[into][i];
      }
    }
    return parent_[from];
  }

 public:
  Graph() {
    std::cin >> num_of_vertexes_;
    graph_.resize(num_of_vertexes_);
    parent_.resize(num_of_vertexes_);
    depth_.resize(num_of_vertexes_);
    used_.resize(num_of_vertexes_);
    for (size_t i = 0; i < num_of_vertexes_ - 1; ++i) {
      size_t from = 0;
      size_t into = 0;
      std::cin >> from >> into;
      graph_[from - 1].push_back(into - 1);
      graph_[into - 1].push_back(from - 1);
    }
  }

  void CountArrayOfBinaryLift() {
    parent_[0] = 0;
    Dfs(0);
    binary_lift_.resize(num_of_vertexes_);
    degree_ = std::ceil(std::log2(num_of_vertexes_));
    for (size_t i = 0; i < num_of_vertexes_; ++i) {
      binary_lift_[i].resize(degree_);
    }
    for (size_t i = 0; i < degree_; ++i) {
      for (size_t j = 0; j < num_of_vertexes_; ++j) {
        if (i == 0) {
          binary_lift_[j][i] = parent_[j];
        } else {
          binary_lift_[j][i] = binary_lift_[binary_lift_[j][i - 1]][i - 1];
        }
      }
    }
  }

  size_t FindShortestPath(size_t from, size_t into) {
    size_t lca = Lca(from, into);
    return depth_[from] + depth_[into] - 2 * depth_[lca];
  }
};

int main() {
  Graph graph;
  size_t num_of_requests = 0;
  std::cin >> num_of_requests;
  graph.CountArrayOfBinaryLift();
  for (size_t i = 0; i < num_of_requests; ++i) {
    size_t first_vertex = 0;
    size_t second_vertex = 0;
    std::cin >> first_vertex >> second_vertex;
    std::cout << graph.FindShortestPath(first_vertex - 1, second_vertex - 1) << "\n";
  }
  return 0;
}
