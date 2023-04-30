#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
///                         Ford–Fulkerson algorithm                         ///
////////////////////////////////////////////////////////////////////////////////

const int64_t kInf = 999999999;

class Graph {
  size_t num_of_vertexes_ = 0;
  size_t num_of_edges_ = 0;
  std::vector<std::vector<int64_t>> graph_;
  std::vector<bool> used_;
  size_t start_ = 0;
  size_t finish_ = 0;

  int64_t Dfs(size_t vertex, int64_t min_flow);

 public:
  Graph();
  int64_t FordFulkerson();
};

Graph::Graph() {
  std::cin >> num_of_vertexes_ >> num_of_edges_;
  graph_.resize(num_of_vertexes_);
  for (size_t i = 0; i < num_of_vertexes_; ++i) {
    graph_[i].resize(num_of_vertexes_);
  }
  used_.resize(num_of_vertexes_);
  finish_ = num_of_vertexes_ - 1;
  size_t from = 0;
  size_t into = 0;
  int64_t cap = 0;
  for (size_t i = 0; i < num_of_edges_; ++i) {
    std::cin >> from >> into >> cap;
    graph_[from - 1][into - 1] += cap;
  }
}

int64_t Graph::Dfs(size_t vertex, int64_t min_flow) {
  if (vertex == finish_) {
    return min_flow;
  }
  used_[vertex] = true;
  for (size_t i = 0; i < graph_[vertex].size(); ++i) {
    if (used_[i]) {
      continue;
    }
    if (graph_[vertex][i] > 0) {
      // пытаемся протолкнуть поток дальше
      int64_t add_flow = Dfs(i, std::min(min_flow, graph_[vertex][i]));
      // увеличиваем поток у себя и перестраиваем сеть
      if (add_flow > 0) {
        graph_[vertex][i] -= add_flow;
        graph_[i][vertex] += add_flow;
        return add_flow;
      }
    }
  }
  return 0;
}

int64_t Graph::FordFulkerson() {
  int64_t total_flow = 0;
  int64_t add_flow = 0;
  do {
    add_flow = Dfs(start_, kInf);
    total_flow += add_flow;
    for (size_t i = 0; i < num_of_vertexes_; ++i) {
      used_[i] = false;
    }
  } while (add_flow > 0);
  return total_flow;
}

int main() {
  Graph graph;
  std::cout << graph.FordFulkerson() << "\n";
  return 0;
}
