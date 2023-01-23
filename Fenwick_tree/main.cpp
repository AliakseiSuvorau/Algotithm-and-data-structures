// 75085433
#include <iostream>
#include <vector>

class FenTree {
  std::vector<std::vector<std::vector<int64_t>>> tree_;
  int64_t GetSum(size_t x, size_t y, size_t z);

 public:
  explicit FenTree(size_t size);
  ~FenTree() = default;
  void Update(size_t x, size_t y, size_t z, int64_t delta_key);
  int64_t GetNumOfStars(size_t x1, size_t y1, size_t z1, size_t x2, size_t y2,
                        size_t z2);
};

FenTree::FenTree(size_t size) {
  tree_.resize(size);
  for (size_t i = 0; i < size; ++i) {
    tree_[i].resize(size);
  }
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      tree_[i][j].resize(size, 0);
    }
  }
}

void FenTree::Update(size_t x, size_t y, size_t z, int64_t delta_key) {
  for (size_t i = x; i < tree_.size(); i = i | (i + 1)) {
    for (size_t j = y; j < tree_.size(); j = j | (j + 1)) {
      for (size_t k = z; k < tree_.size(); k = k | (k + 1)) {
        tree_[i][j][k] += delta_key;
      }
    }
  }
}

int64_t FenTree::GetSum(size_t x, size_t y, size_t z) {
  int64_t sum = 0;
  for (size_t i = x; i < tree_.size(); i = (i & (i + 1)) - 1) {
    for (size_t j = y; j < tree_.size(); j = (j & (j + 1)) - 1) {
      for (size_t k = z; k < tree_.size(); k = (k & (k + 1)) - 1) {
        sum += tree_[i][j][k];
      }
    }
  }
  return sum;
}

int64_t FenTree::GetNumOfStars(size_t x1, size_t y1, size_t z1, size_t x2,
                               size_t y2, size_t z2) {
  int64_t a = GetSum(x2, y2, z2);
  if (x1 != 0) {
    a -= GetSum(x1 - 1, y2, z2);
  }
  if (y1 != 0) {
    a -= GetSum(x2, y1 - 1, z2);
  }
  if (z1 != 0) {
    a -= GetSum(x2, y2, z1 - 1);
  }
  if (y1 * z1 != 0) {
    a += GetSum(x2, y1 - 1, z1 - 1);
  }
  if (x1 * z1 != 0) {
    a += GetSum(x1 - 1, y2, z1 - 1);
  }
  if (x1 * y1 != 0) {
    a += GetSum(x1 - 1, y1 - 1, z2);
  }
  if (x1 * y1 * z1 != 0) {
    a -= GetSum(x1 - 1, y1 - 1, z1 - 1);
  }
  return a;
}

int64_t CountRequest(FenTree& tree) {
  size_t x1 = 0;
  size_t x2 = 0;
  size_t y1 = 0;
  size_t y2 = 0;
  size_t z1 = 0;
  size_t z2 = 0;
  std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
  return tree.GetNumOfStars(x1, y1, z1, x2, y2, z2);
}

void UpdateRequest(FenTree& tree) {
  size_t x = 0;
  size_t y = 0;
  size_t z = 0;
  int64_t delta = 0;
  std::cin >> x >> y >> z >> delta;
  tree.Update(x, y, z, delta);
}

int main() {
  size_t size_of_tree = 0;
  std::cin >> size_of_tree;
  FenTree tree(size_of_tree);
  int num_of_request = 0;
  while (std::cin >> num_of_request) {
    if (num_of_request == 3) {
      break;
    }
    if (num_of_request == 2) {
      std::cout << CountRequest(tree) << "\n";
    }
    if (num_of_request == 1) {
      UpdateRequest(tree);
    }
  }
  return 0;
}
