#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

class DfsVisitor {
  size_t num_of_vertexes_ = 0;
  size_t num_of_edges_ = 0;
  std::vector<std::vector<int>> graph_;
  std::vector<bool> used_;
  size_t timer_ = 0;
  std::vector<size_t> tin_;
  std::vector<size_t> ret_;
  std::vector<std::vector<int>> wood_edges_;
  std::vector<int> roots_;

  void EnterGraph() {
    for (int i = 0; i < int(num_of_edges_); ++i) {
      int from = -1, to = -1;
      std::cin >> from >> to;
      graph_[from - 1].push_back(to - 1);
      graph_[to - 1].push_back(from - 1);
    }
  }

  void DFS(int vertex, int parent = -1) {
    if (used_[vertex]) {
      return;
    }
    if (parent == -1) {
      roots_.push_back(vertex);
    } else {
      wood_edges_[parent].push_back(vertex);
    }
    used_[vertex] = true;
    tin_[vertex] = ret_[vertex] = timer_++;
    for (auto& neighbour : graph_[vertex]) {
      if (neighbour == parent) {
        continue;
      }
      if (used_[neighbour]) {
        ret_[vertex] = std::min(tin_[neighbour], ret_[vertex]);
      } else {
        DFS(neighbour, vertex);
        ret_[vertex] = std::min(ret_[vertex], ret_[neighbour]);
      }
    }
  }

  bool IsRoot(int vertex) {
    return std::find(roots_.begin(), roots_.end(), vertex) != roots_.end();
  }

  void CollectPoints(std::set<int>& points) {
    for (int i = 0; i < int(num_of_vertexes_); ++i) {
      if (IsRoot(i)) {
        if (wood_edges_[i].size() >= 2) {
          points.insert(i);
        }
      } else {
        for (auto& neighbour : wood_edges_[i]) {
          if (ret_[neighbour] >= tin_[i]) {
            points.insert(i);
          }
        }
      }
    }
  }

 public:
  DfsVisitor() {
    std::cin >> num_of_vertexes_ >> num_of_edges_;
    graph_.resize(num_of_vertexes_);
    EnterGraph();
    used_.resize(num_of_vertexes_);
    ret_.resize(num_of_vertexes_);
    tin_.resize(num_of_vertexes_);
    wood_edges_.resize(num_of_vertexes_);
  }

  std::set<int> FindArticulationPoints() {
    for (int i = 0; i < int(num_of_vertexes_); ++i) {
      if (!used_[i]) {
        DFS(i);
      }
    }
    std::set<int> points;
    CollectPoints(points);
    return points;
  }
};

int main() {
  DfsVisitor visitor;
  std::set<int> articulation_points = visitor.FindArticulationPoints();
  std::cout << articulation_points.size() << "\n";
  for (auto& point : articulation_points) {
    std::cout << point + 1 << "\n";
  }
  return 0;
}
