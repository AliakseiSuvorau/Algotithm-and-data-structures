#include <algorithm>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename Vertex = int, typename Edge = std::pair<Vertex, Vertex>>
class Graph {
 protected:
  size_t vertex_num_ = 0;
  size_t edge_num_ = 0;

 public:
  using VertexType = Vertex;
  using EdgeType = Edge;
  Graph(size_t vertex_num, size_t edge_num)
      : vertex_num_(vertex_num), edge_num_(edge_num) {}
  size_t GetNumOfVertex() const { return vertex_num_; }
  size_t GetNumOfEdges() const { return edge_num_; }
  virtual std::vector<Vertex> GetNeighbours(const Vertex& vertex) = 0;
};

template <typename Vertex, typename Edge>
class ListGraph : public Graph<Vertex, Edge> {
 private:
  std::unordered_map<Vertex, std::vector<Vertex>> adjacency_list_;

 public:
  ListGraph(size_t vertex_num, std::vector<Edge> edges)
      : Graph<Vertex, Edge>(vertex_num, edges.size()) {
    for (auto& edge : edges) {
      adjacency_list_[edge.first].push_back(edge.second);
      adjacency_list_[edge.second].push_back(edge.first);
    }
  }
  std::vector<Vertex> GetNeighbours(const Vertex& vertex) final {
    return adjacency_list_[vertex];
  }
  typename std::vector<Vertex>::iterator GetIterNeighbours(Vertex vertex) {
    return adjacency_list_[vertex].begin();
  }
  std::unordered_map<Vertex, std::vector<Vertex>>* GetList() {
    return &adjacency_list_;
  }
  typename std::unordered_map<Vertex, std::vector<Vertex>>::iterator
  GetVertexBeginIter() {
    return adjacency_list_.begin();
  }
  typename std::unordered_map<Vertex, std::vector<Vertex>>::iterator
  GetVertexEndIter() {
    return adjacency_list_.end();
  }
};

template <typename Vertex, typename Edge>
class MatrixGraph : public Graph<Vertex, Edge> {
 private:
  std::unordered_map<Vertex, std::unordered_set<Vertex>> adjacency_matrix_;

 public:
  MatrixGraph(size_t vertex_num, std::vector<Edge> edges)
      : Graph<Vertex, Edge>(vertex_num, edges.size()) {
    for (auto& edge : edges) {
      adjacency_matrix_[edge.first].insert(edge.second);
      adjacency_matrix_[edge.second].insert(edge.first);
    }
  }
  std::vector<Vertex> GetNeighbours(const Vertex& vertex) final {
    std::vector<Vertex> neighbours;
    for (auto& u : adjacency_matrix_[vertex]) {
      if (adjacency_matrix_[vertex].find(u) !=
          adjacency_matrix_[vertex].end()) {
        neighbours.push_back(u);
      }
    }
    return neighbours;
  }
  typename std::unordered_set<Vertex>::iterator GetIterNeighbours(
      Vertex vertex) {
    return adjacency_matrix_[vertex].begin();
  }
};

template <typename Vertex, typename Edge>
class Visitor {
 public:
  Visitor() = default;
  ~Visitor() = default;
  virtual void DFS(Vertex, Vertex) = 0;
  virtual void FindBridges(
      std::vector<Edge>& edges,
      std::unordered_map<Vertex, std::unordered_map<Vertex, int>>&
          times_edge) = 0;
};

template <typename Vertex, typename Edge>
class BridgeVisitor : public Visitor<Vertex, Edge> {
 protected:
  std::vector<size_t> bridges_;
  ListGraph<Vertex, Edge>* graph_;
  std::unordered_set<Vertex> used_;
  size_t timer_ = 0;
  std::unordered_map<Vertex, size_t> tin_;
  std::unordered_map<Vertex, size_t> ret_;

  void DFS(Vertex vertex, Vertex parent) final {
    if (WasVisited(vertex)) {
      return;
    }
    used_.insert(vertex);
    tin_[vertex] = timer_++;
    ret_[vertex] = tin_[vertex];
    std::vector<Vertex> list = (*graph_).GetNeighbours(vertex);
    for (size_t i = 0; i < list.size(); ++i) {
      Vertex neighbour = list[i];
      if (neighbour == parent) {
        continue;
      }
      if (WasVisited(neighbour)) {
        ret_[vertex] = std::min(ret_[vertex], tin_[neighbour]);
      } else {
        DFS(neighbour, vertex);
        ret_[vertex] = std::min(ret_[vertex], ret_[neighbour]);
      }
    }
  }
  bool WasVisited(Vertex vertex) { return used_.find(vertex) != used_.end(); }
  bool IsMultiple(
      Edge& edge,
      std::unordered_map<Vertex, std::unordered_map<Vertex, int>>& times_edge) {
    return times_edge[edge.first][edge.second] +
               times_edge[edge.second][edge.first] >
           1;
  }

 public:
  explicit BridgeVisitor(ListGraph<Vertex, Edge>* graph) { graph_ = graph; }
  void FindBridges(std::vector<Edge>& edges,
                   std::unordered_map<Vertex, std::unordered_map<Vertex, int>>&
                       times_edge) final {
    std::unordered_map<Vertex, std::vector<Vertex>>* list_ptr =
        (*graph_).GetList();
    for (auto& vertex : *list_ptr) {
      if (!WasVisited(vertex.first)) {
        DFS(vertex.first, -1);
      }
    }
    for (size_t i = 0; i < edges.size(); ++i) {
      if (ret_[edges[i].first] > tin_[edges[i].second] ||
          ret_[edges[i].second] > tin_[edges[i].first]) {
        if (!IsMultiple(edges[i], times_edge)) {
          bridges_.push_back(i + 1);
        }
      }
    }
  }
  std::vector<size_t> GetBridges() { return bridges_; }
};

void FastInput() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}

int main() {
  FastInput();
  using Edge = std::pair<int, int>;
  size_t num_of_vertexes = 0, num_of_edges = 0;
  std::cin >> num_of_vertexes >> num_of_edges;
  std::vector<Edge> edges;
  std::unordered_map<int, std::unordered_map<int, int>> times_edge;
  for (size_t i = 0; i < num_of_edges; ++i) {
    int from = -1, to = -1;
    std::cin >> from >> to;
    edges.emplace_back(from, to);
    ++times_edge[from][to];
  }
  ListGraph<int, Edge> graph(num_of_vertexes, edges);
  BridgeVisitor<int, Edge> visitor(&graph);
  visitor.FindBridges(edges, times_edge);
  std::vector<size_t> bridges = visitor.GetBridges();
  std::cout << bridges.size() << "\n";
  for (auto& num : bridges) {
    std::cout << num << " ";
  }
  return 0;
}
