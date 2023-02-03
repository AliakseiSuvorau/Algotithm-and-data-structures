#include <algorithm>
#include <iostream>
#include <vector>

void DFS(int v, std::vector<std::vector<int>>& graph, std::vector<char>& color,
         bool& flag, std::vector<int>& top_sort) {
  color[v] = 1;
  for (size_t i = 0; i < graph[v].size(); ++i) {
    if (color[graph[v][i]] == 0) {
      DFS(graph[v][i], graph, color, flag, top_sort);
    }
    if (flag) {
      return;
    }
    if (color[graph[v][i]] == 1) {
      flag = true;
      return;
    }
  }
  top_sort.push_back(v);
  color[v] = 2;
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
    }
  }
}

int main() {
  FastInput();
  int n = 0, m = 0;
  std::cin >> n >> m;
  std::vector<std::vector<int>> graph(n);
  InputGraph(m, graph);
  std::vector<char> color(n);
  bool flag = false;
  std::vector<int> top_sort;
  for (int i = 0; i < n; ++i) {
    if (color[i] == 0 && !flag) {
      DFS(i, graph, color, flag, top_sort);
    }
  }
  if (flag) {
    std::cout << "-1";
    return 0;
  }
  for (int i = n - 1; i >= 0; --i) {
    std::cout << top_sort[i] + 1 << " ";
  }
  return 0;
}
