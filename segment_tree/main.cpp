#include <iostream>
#include <vector>

class SegTree {
  struct Node {
    int64_t sum = 0;
    size_t left = 0;
    size_t right = 0;
  };

  std::vector<Node> tree_;
  size_t shift_ = 0;

  static size_t FindDeg(size_t num) {
    size_t degree = 1;
    while (degree < num) {
      degree <<= 1;
    }
    return degree;
  }

 public:
  SegTree(std::vector<int>& input);
  int64_t SegmentSum(size_t cur, size_t l, size_t r);
  int64_t SegmentSumRequest(size_t l, size_t r);
  void UpdateValueRequest(size_t index, int64_t new_value);
  ~SegTree() = default;
};

SegTree::SegTree(std::vector<int>& input) {
  size_t deg_of_two = FindDeg(input.size());
  size_t size = 2 * deg_of_two - 1;
  tree_.resize(size);
  shift_ = deg_of_two - 1;
  for (size_t i = shift_; i < input.size() + shift_; ++i) {
    if (i % 2 == 1) {
      tree_[i] = {input[i - shift_], i - shift_, i - shift_};
    } else {
      tree_[i] = {-input[i - shift_], i - shift_, i - shift_};
    }
  }
  for (size_t i = input.size() + shift_; i < size; ++i) {
    tree_[i] = {0, i - shift_, i - shift_};
  }
  for (int i = int(deg_of_two - 2); i >= 0; --i) {
    tree_[i].sum = tree_[2 * i + 1].sum + tree_[2 * i + 2].sum;
    tree_[i].left = tree_[2 * i + 1].left;
    tree_[i].right = tree_[2 * i + 2].right;
  }
}

int64_t SegTree::SegmentSum(size_t cur, size_t l, size_t r) {
  if (tree_[cur].left > r || tree_[cur].right < l) {
    return 0;
  }
  if (tree_[cur].left >= l && tree_[cur].right <= r) {
    return tree_[cur].sum;
  }
  int64_t sum_of_subtrees = 0;
  sum_of_subtrees += SegmentSum(2 * cur + 1, l, r);
  sum_of_subtrees += SegmentSum(2 * cur + 2, l, r);
  return sum_of_subtrees;
}

int64_t SegTree::SegmentSumRequest(size_t l, size_t r) {
  if (l % 2 == 0) {
    return SegmentSum(0, l, r);
  }
  return -SegmentSum(0, l, r);
}

void SegTree::UpdateValueRequest(size_t index, int64_t new_value) {
  int64_t delta = 0;
  if (index % 2 == 1) {
    delta = -new_value - tree_[index + shift_].sum;
  } else {
    delta = new_value - tree_[index + shift_].sum;
  }
  size_t cur = index + shift_;
  while (cur > 0) {
    tree_[cur].sum += delta;
    cur = (cur - 1) / 2;
  }
  tree_[cur].sum += delta;
}

int main() {
  size_t size_of_input = 0;
  std::cin >> size_of_input;
  std::vector<int> input(size_of_input);
  for (size_t i = 0; i < size_of_input; ++i) {
    std::cin >> input[i];
  }
  SegTree tree(input);
  size_t num_of_requests = 0;
  std::cin >> num_of_requests;
  for (size_t i = 0; i < num_of_requests; ++i) {
    int request = 0;
    int first_param = 0;
    int second_param = 0;
    std::cin >> request >> first_param >> second_param;
    if (request != 0) {
      std::cout << tree.SegmentSumRequest(first_param - 1, second_param - 1)
                << "\n";
    } else {
      tree.UpdateValueRequest(first_param - 1, second_param);
    }
  }
  return 0;
}
