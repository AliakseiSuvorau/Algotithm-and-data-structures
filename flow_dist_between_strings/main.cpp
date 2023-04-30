#include <iostream>
#include <queue>
#include <vector>

struct Edge {
  size_t into = 0;
  size_t cap = 0;
  size_t reverse_id = 0;
};

class Graph {
  const size_t kInf = 999999999;
  size_t num_of_vertexes_ = 0;
  std::vector<std::vector<Edge>> graph_;
  std::vector<bool> used_;
  size_t start_ = 0;
  size_t finish_ = 0;
  std::string first_;
  std::string second_;
  size_t q_f_ = 0;
  size_t q_s_ = 0;
  size_t dist_ = 0;
  std::vector<size_t> first_edge_;
  std::vector<size_t> layer_;

  void DinicBfs();
  size_t GetFlowDfs(size_t cur, size_t flow);

  void GetStrings() { std::cin >> first_ >> second_; }

  static size_t Count(std::string& str, char sym) {
    size_t cnt = 0;
    for (auto& symbol : str) {
      if (sym == symbol) {
        ++cnt;
      }
    }
    return cnt;
  }

  void CountQs() {
    q_f_ = Count(first_, '?');
    q_s_ = Count(second_, '?');
  }

  void AddEdge(size_t from, size_t into, size_t cap) {
    graph_[from].push_back({into, cap, graph_[into].size()});
    graph_[into].push_back({from, 0, graph_[from].size() - 1});
  }

  void BothQ(size_t& f_q_num, size_t& s_q_num) {
    AddEdge(f_q_num, s_q_num, 1);
    AddEdge(s_q_num, f_q_num, 1);
    ++f_q_num;
    ++s_q_num;
  }

  void FirstQ(size_t& f_q_num, size_t idx) {
    if (second_[idx] == '0') {
      AddEdge(0, f_q_num, 1);
    } else {
      AddEdge(f_q_num, num_of_vertexes_ - 1, 1);
    }
    ++f_q_num;
  }

  void SecondQ(size_t& s_q_num, size_t idx) {
    if (first_[idx] == '0') {
      AddEdge(0, s_q_num, 1);
    } else {
      AddEdge(s_q_num, num_of_vertexes_ - 1, 1);
    }
    ++s_q_num;
  }

  void ProcessEdges(size_t from, size_t f_q_num) {
    size_t s_q_num = 1 + q_f_;
    for (size_t i = from, j = 0; j < second_.length(); ++i, ++j) {
      if (first_[i] == '?' && second_[j] == '?') {
        BothQ(f_q_num, s_q_num);
        continue;
      }
      if (first_[i] == '?') {
        FirstQ(f_q_num, j);
        continue;
      }
      if (second_[j] == '?') {
        SecondQ(s_q_num, i);
      }
    }
  }

  void PaintDfs(size_t vertex) {
    used_[vertex] = true;
    for (auto& edge : graph_[vertex]) {
      if (used_[edge.into] || edge.cap == 0) {
        continue;
      }
      PaintDfs(edge.into);
    }
  }

  size_t GetCringe(size_t from) {
    size_t delta = 0;
    for (size_t i = from, j = 0; j < second_.length(); ++i, ++j) {
      if (first_[i] != second_[j]) {
        ++delta;
      }
    }
    return delta;
  }

 public:
  Graph();
  size_t Dinic();

  void BuildMinSplit() {
    Dinic();
    used_.assign(num_of_vertexes_, false);
    PaintDfs(0);
  }

  size_t GetDist() {
    for (size_t i = 0; i <= first_.length() - second_.length(); ++i) {
      dist_ += GetCringe(i);
    }
    return dist_;
  }

  void MakeFirstSrt() {
    size_t f_q_num = 1;
    for (size_t i = 0; i < first_.length(); ++i) {
      if (first_[i] != '?') {
        continue;
      }
      if (used_[f_q_num]) {
        first_[i] = '0';
      } else {
        first_[i] = '1';
      }
      ++f_q_num;
    }
  }

  void MakeSecondStr() {
    size_t q_s_num = 1 + q_f_;
    for (size_t i = 0; i < second_.length(); ++i) {
      if (second_[i] != '?') {
        continue;
      }
      if (used_[q_s_num]) {
        second_[i] = '0';
      } else {
        second_[i] = '1';
      }
      ++q_s_num;
    }
  }

  void MakeStr() {
    MakeFirstSrt();
    MakeSecondStr();
  }

  void PrintStr() { std::cout << first_ << "\n" << second_; }
};

Graph::Graph() {
  GetStrings();
  CountQs();
  num_of_vertexes_ = q_f_ + q_s_ + 2;
  finish_ = num_of_vertexes_ - 1;
  used_.resize(num_of_vertexes_);
  graph_.resize(num_of_vertexes_);
  layer_.resize(num_of_vertexes_, kInf);
  first_edge_.resize(num_of_vertexes_);
  size_t f_q_num = 1;
  for (size_t i = 0; i <= first_.length() - second_.length(); ++i) {
    ProcessEdges(i, f_q_num);
    if (first_[i] == '?') {
      ++f_q_num;
    }
  }
}

size_t Graph::Dinic() {
  size_t max_flow = 0;
  do {
    first_edge_.assign(num_of_vertexes_, 0);
    used_.assign(num_of_vertexes_, false);
    DinicBfs();
    size_t add_flow = 0;
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

size_t Graph::GetFlowDfs(size_t cur, size_t flow) {
  if (cur == finish_) {
    return flow;
  }
  while (first_edge_[cur] < graph_[cur].size()) {
    auto& neighbour = graph_[cur][first_edge_[cur]];
    if (layer_[cur] + 1 != layer_[neighbour.into] || neighbour.cap == 0) {
      ++first_edge_[cur];
      continue;
    }
    size_t add_flow = GetFlowDfs(neighbour.into, std::min(flow, neighbour.cap));
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

int main() {
  Graph graph;
  graph.BuildMinSplit();
  graph.MakeStr();
  std::cout << graph.GetDist() << "\n";
  graph.PrintStr();
  return 0;
}
