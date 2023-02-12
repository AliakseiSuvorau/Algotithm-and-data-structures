// 82055691
#include <iostream>
#include <queue>
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
  virtual void TreeEdge(const Edge& edge) = 0;
};

template <typename Vertex, typename Edge>
class BfsAncestorVisitor : public Visitor<Vertex, Edge> {
 protected:
  std::unordered_map<Vertex, Vertex> ancestors_;

 public:
  void TreeEdge(const Edge& edge) final {
    ancestors_[edge.second] = edge.first;
  }
  std::unordered_map<Vertex, Vertex> GetAncestors() { return ancestors_; }
};

template <typename Vertex>
std::vector<Vertex> MakeTree(std::unordered_map<Vertex, Vertex>& ancestors,
                             Vertex& start, Vertex& finish) {
  if (ancestors.find(finish) == ancestors.end()) {
    return {};
  }
  std::stack<int> way;
  int current = finish;
  while (current != start) {
    way.push(current);
    current = ancestors[current];
  }
  way.push(start);
  std::vector<Vertex> tree;
  while (!way.empty()) {
    tree.push_back(way.top());
    way.pop();
  }
  return tree;
}

template <typename Vertex, typename Edge>
std::vector<Vertex> BFS(ListGraph<Vertex, Edge>& graph,
                        BfsAncestorVisitor<Vertex, Edge>& visitor,
                        Vertex& start, Vertex& finish) {
  std::unordered_map<Vertex, int> dist;
  std::queue<Vertex> q;
  q.push(start);
  dist[start] = 0;
  while (!q.empty()) {
    Vertex current = q.front();
    q.pop();
    for (auto& neighbour : graph.GetNeighbours(current)) {
      if (dist.find(neighbour) == dist.end() ||
          dist[neighbour] > dist[current] + 1) {
        q.push(neighbour);
        dist[neighbour] = dist[current] + 1;
        visitor.TreeEdge({current, neighbour});
      }
    }
  }
  std::unordered_map<Vertex, Vertex> ancestors = visitor.GetAncestors();
  return MakeTree(ancestors, start, finish);
}

void PrintAncestors(std::vector<int>& tree, int finish, int start) {
  if (start == finish) {
    std::cout << 0 << "\n" << start;
    return;
  }
  if (tree.empty()) {
    std::cout << "-1";
    return;
  }
  std::cout << tree.size() - 1 << "\n";
  for (auto& vertex : tree) {
    std::cout << vertex << " ";
  }
}

int main() {
  size_t num_of_vertexes = 0, num_of_edges = 0;
  std::cin >> num_of_vertexes >> num_of_edges;
  int start = -1, finish = -1;
  std::cin >> start >> finish;
  std::vector<std::pair<int, int>> edges(num_of_edges);
  for (size_t i = 0; i < num_of_edges; ++i) {
    int from = -1, to = -1;
    std::cin >> from >> to;
    edges.emplace_back(from, to);
  }
  ListGraph<int, std::pair<int, int>> graph(num_of_vertexes, edges);
  BfsAncestorVisitor<int, std::pair<int, int>> visitor;
  std::vector<int> ancestors_tree = BFS(graph, visitor, start, finish);
  PrintAncestors(ancestors_tree, finish, start);
  return 0;
}
