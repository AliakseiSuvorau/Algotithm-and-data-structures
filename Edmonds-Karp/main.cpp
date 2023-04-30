#include <iostream>
#include <queue>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
///                         Edmonds-Karp algorithm                           ///
////////////////////////////////////////////////////////////////////////////////

const int64_t kInf = 999999999;

class Graph {
  size_t num_of_vertexes_ = 0;
  size_t num_of_edges_ = 0;
  std::vector<std::vector<int64_t>> graph_;
  std::vector<int64_t> flow_;
  std::vector<size_t> anc_;
  size_t start_ = 0;
  size_t finish_ = 0;

  int64_t Bfs();
  void FixNet(int64_t flow) {
    size_t cur = finish_;
    while (cur != start_) {
      graph_[anc_[cur]][cur] -= flow;
      graph_[cur][anc_[cur]] += flow;
      cur = anc_[cur];
    }
  }

 public:
  Graph();
  int64_t EdmondsKarp();
};

Graph::Graph() {
  std::cin >> num_of_vertexes_ >> num_of_edges_;
  graph_.resize(num_of_vertexes_);
  for (size_t i = 0; i < num_of_vertexes_; ++i) {
    graph_[i].resize(num_of_vertexes_);
  }
  flow_.resize(num_of_vertexes_);
  anc_.resize(num_of_vertexes_);
  finish_ = num_of_vertexes_ - 1;
  size_t from = 0;
  size_t into = 0;
  int64_t cap = 0;
  for (size_t i = 0; i < num_of_edges_; ++i) {
    std::cin >> from >> into >> cap;
    graph_[from - 1][into - 1] += cap;
  }
}

int64_t Graph::Bfs() {
  std::queue<size_t> queue;
  flow_[start_] = kInf;
  queue.push(start_);
  while (!queue.empty()) {
    size_t vertex = queue.front();
    queue.pop();
    for (size_t i = 0; i < num_of_vertexes_; ++i) {
      if (graph_[vertex][i] == 0) {
        continue;
      }
      if (flow_[i] < std::min(flow_[vertex], graph_[vertex][i])) {
        flow_[i] = std::min(flow_[vertex], graph_[vertex][i]);
        anc_[i] = vertex;
        queue.push(i);
      }
    }
  }
  return flow_[finish_];
}

int64_t Graph::EdmondsKarp() {
  int64_t total_flow = 0;
  int64_t add_flow = 0;
  do {
    add_flow = Bfs();
    if (add_flow == 0) {
      break;
    }
    total_flow += add_flow;
    FixNet(add_flow);
    for (size_t i = 0; i < num_of_vertexes_; ++i) {
      flow_[i] = 0;
      anc_[i] = 0;
    }
  } while (add_flow > 0);
  return total_flow;
}

int main() {
  Graph graph;
  std::cout << graph.EdmondsKarp() << "\n";
  return 0;
}
