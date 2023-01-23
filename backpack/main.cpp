#include <iostream>
#include <vector>

struct Node {
  int v = 0;
  int pr_i = -1;
  int pr_j = -1;
};

bool Find(std::vector<int>& v, int a) {
  for (size_t i = 0; i < v.size(); ++i) {
    if (a == v[i]) {
      return true;
    }
  }
  return false;
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;
  std::vector<int> mass(n);
  std::vector<int> price(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> mass[i];
  }
  for (int i = 0; i < n; ++i) {
    std::cin >> price[i];
  }
  std::vector<std::vector<Node>> dp(n + 1, std::vector<Node>(m + 1));
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      dp[i][j].v = dp[i - 1][j].v;
      dp[i][j].pr_i = i - 1;
      dp[i][j].pr_j = j;
      if (j >= mass[i - 1] &&
          dp[i][j].v < dp[i - 1][j - mass[i - 1]].v + price[i - 1]) {
        dp[i][j].v = dp[i - 1][j - mass[i - 1]].v + price[i - 1];
        dp[i][j].pr_i = i - 1;
        dp[i][j].pr_j = j - mass[i - 1];
      }
    }
  }
  std::vector<int> ans;
  int i = n;
  int j = m;
  int pr_i = dp[i][j].pr_i;
  int pr_j = dp[i][j].pr_j;
  while (!(pr_i == -1 || pr_j == -1)) {
    if (dp[i][j].v != dp[pr_i][pr_j].v) {
      if (!Find(ans, i)) {
        ans.push_back(i);
      }
    }
    std::swap(i, pr_i);
    std::swap(j, pr_j);
    pr_i = dp[i][j].pr_i;
    pr_j = dp[i][j].pr_j;
  }

  for (size_t l = 0; l < ans.size(); ++l) {
    std::cout << ans[l] << "\n";
  }
  return 0;
}
