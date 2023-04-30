#include <iostream>
#include <vector>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
///                             Minimum span tree                            ///
////////////////////////////////////////////////////////////////////////////////

template <typename Vertex, typename Edge = std::pair<Vertex, Vertex>>
class DSU {
  std::vector<size_t> parent_;
  std::vector<size_t> depth_;

 public:
  DSU() = default;
  DSU(size_t size);
  DSU& operator=(DSU& other);
  void AddToSet(size_t parent, size_t child);
  void MakeSet(std::vector<std::pair<size_t, size_t>>& edges);
  size_t GetSetRoot(size_t member);
  void UniteSets(size_t first_member, size_t second_member);
  bool InOneSet(size_t one, size_t other);
};

DSU::DSU(size_t size) {
  parent_.resize(size);
  depth_.resize(size);
  for (size_t i = 0; i < size; ++i) {
    parent_[i] = i;
    depth_[i] = 1;
  }
}

void DSU::AddToSet(size_t parent, size_t child) {
  if(InOneSet(parent, child)) {
    return;
  }
  UniteSets(parent, child);
}

void DSU::MakeSet(std::vector<std::pair<size_t, size_t>>& edges) {
  for (auto& edge : edges) {
    AddToSet(edge.first, edge.second);
  }
}

size_t DSU::GetSetRoot(size_t member) {
  size_t vertex = member;
  while (parent_[vertex] != vertex) {
    vertex = parent_[vertex];
  }
  return vertex;
}

void DSU::UniteSets(size_t first_member, size_t second_member) {
  size_t first_root = GetSetRoot(first_member);
  size_t second_root = GetSetRoot(second_member);
  if (depth_[first_root] > depth_[second_root]) {
    parent_[second_root] = first_root;
  } else {
    if (depth_[first_root] < depth_[second_root]) {
      parent_[first_root] = second_root;
    } else {
      parent_[second_root] = first_root;
      ++depth_[first_root];
    }
  }
}

bool DSU::InOneSet(size_t one, size_t other) {
  return GetSetRoot(one) == GetSetRoot(other);
}

DSU& DSU::operator=(DSU& other) {
  parent_ = other.parent_;
  depth_ = other.depth_;
  return *this;
}

template <typename Vertex = size_t,
          typename Weight = int64_t,
          typename Edge = std::pair<std::pair<Vertex, Vertex>, Weight>,
          bool IsDirected = false>
class Graph {
  struct GraphEdge {
    Vertex into;
    Weight weight;
  };

  std::vector<std::vector<GraphEdge>> list_;
  std::vector<Edge> edges_;
  size_t num_of_vertexes_ = 0;
  size_t num_of_edges_ = 0;

 public:
  Graph() = default;
  Graph(size_t num_of_vertexes, std::vector<Edge> edges);
  Graph& operator=(Graph& other);
  std::vector<Edge> GetEdges();
  size_t Size();
};

template <typename Vertex, typename Weight, typename Edge, bool IsDirected>
size_t Graph<Vertex, Weight, Edge, IsDirected>::Size() {
  return num_of_vertexes_;
}

template <typename Vertex, typename Weight, typename Edge, bool IsDirected>
std::vector<Edge> Graph<Vertex, Weight, Edge, IsDirected>::GetEdges() {
  return edges_;
}

template <typename Vertex, typename Weight, typename Edge, bool IsDirected>
Graph<Vertex, Weight, Edge, IsDirected>& Graph<Vertex, Weight, Edge, IsDirected>::operator=(Graph& other) {
  num_of_vertexes_ = other.num_of_vertexes_;
  num_of_edges_ = other.num_of_edges_;
  list_ = other.list_;
  edges_ = other.edges_;
  return *this;
}

template <typename Vertex, typename Weight, typename Edge, bool IsDirected>
Graph<Vertex, Weight, Edge, IsDirected>::Graph(size_t num_of_vertexes, std::vector<Edge> edges)
          : num_of_vertexes_(num_of_vertexes), num_of_edges_(edges.size()), edges_(edges) {
  list_.resize(num_of_vertexes_);
  for (auto& edge : edges) {
    list_[edge.first.first].push_back({edge.first.second, edge.second});
    if (!IsDirected) {
      list_[edge.first.second].push_back({edge.first.first, edge.second});
    }
  }
}

template <typename Vertex = size_t,
          typename Weight = int64_t,
          typename Edge = std::pair<std::pair<Vertex, Vertex>, Weight>>
class MinSpanTree {
  DSU dsu_;
  std::vector<Edge> edges_;
  Weight min_weight_ = Weight();

 public:
  MinSpanTree(Graph<Vertex, Weight, Edge, false> graph);
  Weight GetWeight();
};

template <typename Vertex, typename Weight, typename Edge>
MinSpanTree<Vertex, Weight, Edge>::MinSpanTree(Graph<Vertex, Weight, Edge, false> graph) {
  edges_ = graph.GetEdges();
  std::sort(edges_.begin(),
            edges_.end(),
            [](Edge& one, Edge& another){ return one.second < another.second; });
  DSU dsu(graph.Size());
  for (auto& edge : edges_) {
    if (!dsu.InOneSet(edge.first.first, edge.first.second)) {
      min_weight_ += edge.second;
    }
    dsu.AddToSet(edge.first.first, edge.first.second);
  }
}

template <typename Vertex, typename Weight, typename Edge>
Weight MinSpanTree<Vertex, Weight, Edge>::GetWeight() {
  return min_weight_;
}

void FastInput() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}

int main() {
  FastInput();
  size_t num_of_vertexes = 0;
  size_t num_of_edges = 0;
  std::cin >> num_of_vertexes >> num_of_edges;
  std::vector<std::pair<std::pair<size_t, size_t>, int64_t>> edges;
  for (size_t i = 0; i < num_of_edges; ++i) {
    size_t from = 0;
    size_t into = 0;
    int64_t weight = 0;
    std::cin >> from >> into >> weight;
    --from;
    --into;
    edges.push_back({{from, into}, weight});
  }
  Graph graph(num_of_vertexes, edges);
  MinSpanTree mst(graph);
  std::cout << mst.GetWeight();
  return 0;
}
