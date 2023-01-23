#include <iostream>
#include <string>

class AvlTree {
  struct Node {
    std::string first = "";
    std::string second = "";
    int height = 1;
    Node* left = nullptr;
    Node* right = nullptr;
  };

  Node* root_ = nullptr;

 private:
  void Destroy(Node* cur);
  static void PutHeight(Node* cur);
  Node* RotateRight(Node* cur);
  Node* RotateLeft(Node* cur);
  static int FindDelta(Node* cur);
  Node* Balance(Node* cur);
  Node* InsertSubTree(Node* cur, std::string& first, std::string& second);
  std::string FindSubTree(Node* cur, std::string& name);

 public:
  AvlTree() = default;
  std::string Find(std::string& name);
  void Insert(std::string& first, std::string& second);
  ~AvlTree() { Destroy(root_); }
};

std::string AnsToRequest(AvlTree& tree1, AvlTree& tree2, std::string request) {
  std::string attempt = tree1.Find(request);
  if (attempt == "name_not_found") {
    return tree2.Find(request);
  }
  return attempt;
}

void AvlTree::Destroy(Node* cur) {
  if (cur == nullptr) {
    delete cur;
    return;
  }
  if (cur->left != nullptr) {
    Destroy(cur->left);
  }
  if (cur->right != nullptr) {
    Destroy(cur->right);
  }
  delete cur;
}

void AvlTree::PutHeight(Node* cur) {
  if (cur == nullptr) {
    return;
  }
  if (cur->left != nullptr && cur->right != nullptr) {
    int h_l = cur->left->height;
    int h_r = cur->right->height;
    cur->height = (h_l > h_r) ? h_l + 1 : h_r + 1;
    return;
  }
  if (cur->left == nullptr && cur->right != nullptr) {
    cur->height = cur->right->height + 1;
    return;
  }
  if (cur->right == nullptr && cur->left != nullptr) {
    cur->height = cur->left->height + 1;
    return;
  }
  cur->height = 1;
}

AvlTree::Node* AvlTree::RotateRight(Node* cur) {
  if (cur == nullptr) {
    return cur;
  }
  Node* tmp = cur->left;
  cur->left = tmp->right;
  tmp->right = cur;
  PutHeight(cur);
  PutHeight(tmp);
  return tmp;
}

AvlTree::Node* AvlTree::RotateLeft(Node* cur) {
  if (cur == nullptr) {
    return cur;
  }
  Node* tmp = cur->right;
  cur->right = tmp->left;
  tmp->left = cur;
  PutHeight(cur);
  PutHeight(tmp);
  return tmp;
}

int AvlTree::FindDelta(Node* cur) {
  if (cur->left == nullptr && cur->right == nullptr) {
    return 0;
  }
  if (cur->left != nullptr && cur->right == nullptr) {
    return cur->left->height;
  }
  if (cur->left == nullptr && cur->right != nullptr) {
    return -1 * cur->right->height;
  }
  return cur->left->height - cur->right->height;
}

AvlTree::Node* AvlTree::Balance(Node* cur) {
  int delta = FindDelta(cur);
  if (delta == 2) {  // right rotate
    int delta_left = FindDelta(cur->left);
    if (delta_left < 0) {  // big right rotate
      cur->left = RotateLeft(cur->left);
    }
    return RotateRight(cur);
  }
  if (delta == -2) {  // left rotate
    int delta_right = FindDelta(cur->right);
    if (delta_right > 0) {  // big left rotate
      cur->right = RotateRight(cur->right);
    }
    return RotateLeft(cur);
  }
  PutHeight(cur);
  return cur;
}

AvlTree::Node* AvlTree::InsertSubTree(Node* cur, std::string& first,
                                      std::string& second) {
  if (cur == nullptr) {
    Node* new_root = new Node;
    new_root->first = first;
    new_root->second = second;
    cur = new_root;
  }
  if (first < cur->first) {
    cur->left = InsertSubTree(cur->left, first, second);
  }
  if (first > cur->first) {
    cur->right = InsertSubTree(cur->right, first, second);
  }
  return Balance(cur);
}

std::string AvlTree::FindSubTree(Node* cur, std::string& name) {
  if (cur == nullptr) {
    return "name_not_found";
  }
  if (name == cur->first) {
    return cur->second;
  }
  if (name < cur->first) {
    return FindSubTree(cur->left, name);
  }
  return FindSubTree(cur->right, name);
}

std::string AvlTree::Find(std::string& name) {
  return FindSubTree(root_, name);
}

void AvlTree::Insert(std::string& first, std::string& second) {
  root_ = InsertSubTree(root_, first, second);
}

int main() {
  AvlTree racers_name_tree;
  AvlTree cars_name_tree;
  int num_of_racers = 0;
  std::cin >> num_of_racers;
  for (int i = 0; i < num_of_racers; ++i) {
    std::string racer;
    std::string car;
    std::cin >> racer >> car;
    racers_name_tree.Insert(racer, car);
    cars_name_tree.Insert(car, racer);
  }
  int num_of_requests = 0;
  std::cin >> num_of_requests;
  for (int i = 0; i < num_of_requests; ++i) {
    std::string request;
    std::cin >> request;
    std::cout << AnsToRequest(racers_name_tree, cars_name_tree, request)
              << "\n";
  }
  return 0;
}

