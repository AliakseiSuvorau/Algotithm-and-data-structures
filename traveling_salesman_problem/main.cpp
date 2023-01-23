#include <iostream>
#include <vector>

struct Node {
  int val = 10000000;
  int pr = -1;
};

int main() {
  int n = 0;
  std::cin >> n;
  std::vector<std::vector<int>> a(n, std::vector<int>(n));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cin >> a[i][j];
    }
  }
  std::vector<std::vector<Node>> dp(1 << n, std::vector<Node>(n));
  const int kInf = 10000000;
  int m = n;
  n = 1 << n;
  // база
  for (int i = 0; i < m; ++i) {
    dp[1 << i][i].val = 0;
    dp[0][i].val = 0;
  }
  // переход
  for (int mask = 0; mask < n; ++mask) {
    for (int i = 0; i < m; ++i) {
      if ((mask & (1 << i)) == 0) {
        for (int j = 0; j < m; ++j) {
          if (((1 << j) & mask) != 0) {
            if (dp[mask | (1 << i)][i].val > dp[mask][j].val + a[j][i]) {
              dp[mask | (1 << i)][i].val = dp[mask][j].val + a[j][i];
              dp[mask | (1 << i)][i].pr = j;
            }
          }
        }
      }
    }
  }
  std::vector<int> way;
  int min_index = 0;
  int count = kInf;
  for (int i = 0; i < m; ++i) {
    if (dp[n - 1][i].val < count) {
      count = dp[n - 1][i].val;
      min_index = i;
    }
  }
  int cur = min_index;
  int i = n - 1;
  while (dp[i][cur].pr != -1) {
    way.push_back(cur + 1);
    int tmp = i;
    i = i & (~(1 << cur));
    cur = dp[tmp][cur].pr;
  }
  way.push_back(cur + 1);
  std::cout << dp[n - 1][min_index].val << "\n";
  for (int j = int(way.size()) - 1; j >= 0; --j) {
    std::cout << way[j] << " ";
  }
  return 0;
}
