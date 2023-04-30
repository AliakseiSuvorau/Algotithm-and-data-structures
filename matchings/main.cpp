#include <iostream>
#include <queue>
#include <vector>

struct Flow {
  int64_t cap = 0;
  int64_t flow = 0;
};

const int64_t kInf = 999999999;

class Graph {
  size_t num_of_vertexes_ = 0;
  std::vector<std::vector<Flow>> graph_;
  std::vector<int64_t> flow_;
  std::vector<size_t> anc_;
  size_t start_ = 0;
  size_t finish_ = 0;
  size_t first_part_ = 0;
  size_t second_part_ = 0;

  int64_t Bfs();
  void FixNet(int64_t flow) {
    size_t cur = finish_;
    while (cur != start_) {
      graph_[anc_[cur]][cur].flow += flow;
      graph_[cur][anc_[cur]].flow -= flow;
      cur = anc_[cur];
    }
  }

 public:
  Graph();
  int64_t EdmondsKarp();
  void AddStartAndEnd() {
    num_of_vertexes_ += 2;
    graph_.resize(num_of_vertexes_);
    for (size_t i = 0; i < num_of_vertexes_; ++i) {
      graph_[i].resize(num_of_vertexes_);
    }
    start_ = num_of_vertexes_ - 2;
    finish_ = num_of_vertexes_ - 1;
    flow_.resize(num_of_vertexes_);
    anc_.resize(num_of_vertexes_);
    for (size_t i = 0; i < first_part_; ++i) {
      graph_[start_][i].cap = 1;
      graph_[i][start_].cap = 1;
      graph_[i][start_].flow = 1;
    }
    for (size_t i = first_part_; i < num_of_vertexes_ - 2; ++i) {
      graph_[i][finish_].cap = 1;
      graph_[finish_][i].cap = 1;
      graph_[finish_][i].flow = 1;
    }
  }

  void PrintWays() {
    for (size_t i = 0; i < first_part_; ++i) {
      for (size_t j = first_part_; j < num_of_vertexes_ - 2; ++j) {
        if (graph_[i][j].flow == 1) {
          std::cout << i + 1 << " " << j - first_part_ + 1 << "\n";
        }
      }
    }
  }
};

Graph::Graph() {
  std::cin >> first_part_ >> second_part_;
  num_of_vertexes_ = first_part_ + second_part_;
  graph_.resize(num_of_vertexes_);
  for (size_t i = 0; i < num_of_vertexes_; ++i) {
    graph_[i].resize(num_of_vertexes_);
  }
  size_t into = 0;
  for (size_t from = 0; from < first_part_; ++from) {
    std::cin >> into;
    while (into != 0) {
      graph_[from][first_part_ + into - 1].cap = 1;
      graph_[first_part_ + into - 1][from].cap = 1;
      graph_[first_part_ + into - 1][from].flow = 1;
      std::cin >> into;
    }
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
      if (graph_[vertex][i].cap == 0) {
        continue;
      }
      if (flow_[i] < std::min(flow_[vertex],
                              graph_[vertex][i].cap - graph_[vertex][i].flow)) {
        flow_[i] = std::min(flow_[vertex],
                            graph_[vertex][i].cap - graph_[vertex][i].flow);
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
  graph.AddStartAndEnd();
  std::cout << graph.EdmondsKarp() << "\n";
  graph.PrintWays();
  return 0;
}
