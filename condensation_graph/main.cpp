#include <algorithm>
#include <iostream>
#include <vector>

void DFSColors(int v, std::vector<std::vector<int>>& graph,
               std::vector<char>& color, std::vector<int>& top_sort) {
  color[v] = 1;
  for (size_t i = 0; i < graph[v].size(); ++i) {
    if (color[graph[v][i]] == 0) {
      DFSColors(graph[v][i], graph, color, top_sort);
    }
  }
  top_sort.push_back(v);
  color[v] = 2;
}

void DFSUsed(int v, std::vector<std::vector<int>>& graph,
             std::vector<bool>& used, std::vector<int>& component) {
  used[v] = true;
  component.push_back(v);
  for (size_t i = 0; i < graph[v].size(); ++i) {
    if (!used[graph[v][i]]) {
      DFSUsed(graph[v][i], graph, used, component);
    }
  }
}

void FastInput() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}

void InputGraph(int m, std::vector<std::vector<int>>& graph) {
  for (int i = 0; i < m; ++i) {
    int a = 0, b = 0;
    std::cin >> a >> b;
    if (std::find(graph[a - 1].begin(), graph[a - 1].end(), b - 1) ==
        graph[a - 1].end()) {
      graph[a - 1].push_back(b - 1);
      graph[b - 1].push_back(a - 1);
    }
  }
}

void TopSort(std::vector<std::vector<int>>& graph, std::vector<char>& color,
             std::vector<int>& top_sort) {
  size_t size = color.size();
  for (size_t i = 0; i < size; ++i) {
    if (color[i] == 0) {
      DFSColors(int(i), graph, color, top_sort);
    }
  }
}

std::vector<std::vector<int>> Transpose(std::vector<std::vector<int>>& graph) {
  size_t size = graph.size();
  std::vector<std::vector<int>> graph_t(size);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < graph[i].size(); ++j) {
      graph_t[graph[i][j]].push_back(int(i));
    }
  }
  return graph_t;
}

std::vector<std::vector<int>> MakeComponents(
    std::vector<int>& top_sort, std::vector<std::vector<int>>& graph_t) {
  std::vector<std::vector<int>> cond_graph;
  std::vector<bool> used(top_sort.size(), false);
  for (int i = int(top_sort.size()) - 1; i >= 0; --i) {
    if (!used[top_sort[i]]) {
      std::vector<int> component;
      DFSUsed(top_sort[i], graph_t, used, component);
      cond_graph.push_back(component);
    }
  }
  return cond_graph;
}

void PrintComponents(std::vector<std::vector<int>>& cond_graph) {
  std::cout << cond_graph.size() << "\n";
  for (size_t i = 0; i < cond_graph.size(); ++i) {
    std::cout << cond_graph[i].size() << "\n";
    for (size_t j = 0; j < cond_graph[i].size(); ++j) {
      std::cout << cond_graph[i][j] + 1 << " ";
    }
    std::cout << "\n";
  }
}

int main() {
  FastInput();
  int n = 0, m = 0;
  std::cin >> n >> m;
  std::vector<std::vector<int>> graph(n);
  InputGraph(m, graph);
  std::vector<char> color(n);
  std::vector<int> top_sort;
  TopSort(graph, color, top_sort);
  std::vector<std::vector<int>> graph_t = Transpose(graph);
  std::vector<std::vector<int>> cond_graph = MakeComponents(top_sort, graph_t);
  PrintComponents(cond_graph);
  return 0;
}
