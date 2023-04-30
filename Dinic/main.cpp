#include <iostream>
#include <queue>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
///                              Dinic algorithm                             ///
////////////////////////////////////////////////////////////////////////////////

struct Edge {
  size_t into = 0;
  int64_t cap = 0;
  size_t reverse_id = 0;
};

const int64_t kInf = 99999999999;

class Graph {
  std::vector<std::vector<Edge>> graph_;
  size_t num_of_vertexes_ = 0;
  size_t num_of_edges_ = 0;
  size_t start_ = 0;
  size_t finish_ = 0;
  std::vector<size_t> layer_;
  std::vector<bool> used_;
  std::vector<std::pair<size_t, size_t>> edges_;
  std::vector<size_t> first_edge_;

  void DinicBfs();
  int64_t GetFlowDfs(size_t cur, int64_t flow);

 public:
  Graph();
  int64_t Dinic();
  void PrintFlows();
};

Graph::Graph() {
  std::cin >> num_of_vertexes_ >> num_of_edges_;
  graph_.resize(num_of_vertexes_);
  for (size_t i = 0; i < num_of_edges_; ++i) {
    size_t from = 0;
    size_t into = 0;
    int64_t cap = 0;
    std::cin >> from >> into >> cap;
    graph_[from - 1].push_back({into - 1, cap, graph_[into - 1].size()});
    graph_[into - 1].push_back({from - 1, 0, graph_[from - 1].size() - 1});
    edges_.emplace_back(into - 1, graph_[into - 1].size() - 1);
  }
  finish_ = num_of_vertexes_ - 1;
  layer_.resize(num_of_vertexes_, kInf);
  used_.resize(num_of_vertexes_, false);
  first_edge_.resize(num_of_vertexes_);
}

int64_t Graph::Dinic() {
  int64_t max_flow = 0;
  do {
    first_edge_.assign(num_of_vertexes_, 0);
    used_.assign(num_of_vertexes_, false);
    DinicBfs();
    int64_t add_flow = 0;
    do {
      add_flow = GetFlowDfs(start_, kInf);
      max_flow += add_flow;
    } while (add_flow > 0);
  } while (layer_[finish_] != kInf);
  return max_flow;
}

void Graph::DinicBfs() {
  for (size_t i = 0; i < num_of_vertexes_; ++i) {
    layer_[i] = kInf;
  }
  std::queue<size_t> queue;
  queue.push(start_);
  layer_[start_] = 0;
  while (!queue.empty()) {
    size_t cur = queue.front();
    queue.pop();
    for (auto& neighbour : graph_[cur]) {
      if (neighbour.cap > 0 && layer_[neighbour.into] == kInf) {
        layer_[neighbour.into] = layer_[cur] + 1;
        queue.push(neighbour.into);
      }
    }
  }
}

int64_t Graph::GetFlowDfs(size_t cur, int64_t flow) {
  if (cur == finish_) {
    return flow;
  }
  while (first_edge_[cur] < graph_[cur].size()) {
    auto& neighbour = graph_[cur][first_edge_[cur]];
    if (layer_[cur] + 1 != layer_[neighbour.into] || neighbour.cap == 0) {
      ++first_edge_[cur];
      continue;
    }
    int64_t add_flow =
        GetFlowDfs(neighbour.into, std::min(flow, neighbour.cap));
    if (add_flow == 0) {
      ++first_edge_[cur];
      continue;
    }
    graph_[cur][first_edge_[cur]].cap -= add_flow;
    graph_[neighbour.into][neighbour.reverse_id].cap += add_flow;
    return add_flow;
  }
  return 0;
}

void Graph::PrintFlows() {
  for (size_t i = 0; i < num_of_edges_; ++i) {
    std::cout << graph_[edges_[i].first][edges_[i].second].cap << "\n";
  }
}

void OptimizeInput() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}

int main() {
  OptimizeInput();
  Graph graph;
  std::cout << graph.Dinic() << "\n";
  graph.PrintFlows();
  return 0;
}
