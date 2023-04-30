#include <iostream>
#include <unordered_map>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
///                           Минимальный разрез                             ///
////////////////////////////////////////////////////////////////////////////////

const int64_t kInf = 999999999;

class Graph {
  size_t num_of_vertexes_ = 0;
  size_t num_of_edges_ = 0;
  std::vector<std::vector<int64_t>> graph_;
  std::vector<bool> used_;
  size_t start_ = 0;
  size_t finish_ = 0;
  std::vector<std::pair<size_t, size_t>> edges_;

  int64_t Dfs(size_t vertex, int64_t min_flow);
  void PaintDfs(size_t vertex) {
    used_[vertex] = true;
    for (size_t i = 0; i < graph_[vertex].size(); ++i) {
      if (graph_[vertex][i] > 0 && !used_[i]) {
        PaintDfs(i);
      }
    }
  }

 public:
  Graph();
  int64_t FordFulkerson();
  std::vector<size_t> GetSection();
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
    edges_.emplace_back(from - 1, into - 1);
    graph_[from - 1][into - 1] += cap;
    graph_[into - 1][from - 1] += cap;
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
      int64_t add_flow = Dfs(i, std::min(min_flow, graph_[vertex][i]));
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

std::vector<size_t> Graph::GetSection() {
  std::vector<size_t> edges;
  for (size_t i = 0; i < num_of_vertexes_; ++i) {
    used_[i] = false;
  }
  PaintDfs(start_);
  for (size_t i = 0; i < edges_.size(); ++i) {
    if (used_[edges_[i].first] != used_[edges_[i].second]) {
      edges.push_back(i + 1);
    }
  }
  return edges;
}

int main() {
  Graph graph;
  int64_t size = graph.FordFulkerson();
  std::vector<size_t> edges = graph.GetSection();
  std::cout << edges.size() << " " << size << "\n";
  for (auto& num : edges) {
    std::cout << num << " ";
  }
  return 0;
}
