#include <iostream>
#include <queue>
#include <vector>

const int kInf = 999999999;

int FindMinCost(std::vector<std::vector<std::pair<int, int>>>& graph, int s,
                int t) {
  std::vector<int> dist(graph.size(), kInf);
  dist[s] = 0;
  std::priority_queue<std::pair<int, int>> min_queue;
  min_queue.emplace(0, s);
  while (!min_queue.empty()) {
    int min_dist = -min_queue.top().first;
    int min_index = min_queue.top().second;
    min_queue.pop();
    if (min_dist > dist[min_index]) {
      continue;
    }
    for (size_t i = 0; i < graph[min_index].size(); ++i) {
      if (min_dist + graph[min_index][i].second <
          dist[graph[min_index][i].first]) {
        dist[graph[min_index][i].first] = min_dist + graph[min_index][i].second;
        min_queue.emplace(-dist[graph[min_index][i].first],
                          graph[min_index][i].first);
      }
    }
  }
  return dist[t];
}

void FastInput() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}

void EnterGraph(int m, std::vector<std::vector<std::pair<int, int>>>& graph) {
  int from = -1, to = -1, weight = 0;
  for (int i = 0; i < m; ++i) {
    std::cin >> from >> to >> weight;
    graph[from - 1].emplace_back(to - 1, weight);
  }
}

int main() {
  FastInput();
  int n = 0, m = 0, s = -1, t = -1;
  std::cin >> n >> m >> s >> t;
  std::vector<std::vector<std::pair<int, int>>> graph(n);
  EnterGraph(m, graph);
  int cost = FindMinCost(graph, s - 1, t - 1);
  if (cost == kInf) {
    std::cout << -1;
  } else {
    std::cout << cost;
  }
  return 0;
}
