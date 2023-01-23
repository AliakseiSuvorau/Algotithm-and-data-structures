#include <iostream>
#include <vector>

class DecTree {
 private:
  struct Node {
    int key = 0;
    int priority = 0;
    int index = 1;
    Node* pr = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
  };

  Node* root_ = nullptr;

 public:
  DecTree() = default;
  ~DecTree() { Destroy(root_); }
  void Insert(int key);
  void Delete(int key);
  bool Exists(int key);
  std::string Next(int key);
  std::string Prev(int key);
  std::string FindKthStat(int k);

 private:
  void Destroy(Node* root);
  static Node* Create(int key);
  static void SetIndex(Node* tree);
  std::pair<Node*, Node*> SplitSubTree(Node* cur, int key);
  std::pair<Node*, Node*> Split(int key);
  Node* InsertInSubTree(int key);
  Node* Merge(Node* tree1, Node* tree2);
  bool KeyExistsInSubTree(Node* cur, int key);
  Node* GetNextInSubTree(Node* cur, int key);
  Node* GetPrevInSubTree(Node* cur, int key);
  Node* FindKthStatInSubTree(Node* cur, int k);
};

void InsertRequest(DecTree& tree) {
  int key = 0;
  std::cin >> key;
  tree.Insert(key);
}

void DeleteRequest(DecTree& tree) {
  int key = 0;
  std::cin >> key;
  tree.Delete(key);
}

void ExistsRequest(DecTree& tree) {
  int key = 0;
  std::cin >> key;
  bool is_in = tree.Exists(key);
  if (is_in) {
    std::cout << "true\n";
  } else {
    std::cout << "false\n";
  }
}

void NextRequest(DecTree& tree) {
  int key = 0;
  std::cin >> key;
  std::string next = tree.Next(key);
  std::cout << next << "\n";
}

void PrevRequest(DecTree& tree) {
  int key = 0;
  std::cin >> key;
  std::string prev = tree.Prev(key);
  std::cout << prev << "\n";
}

void KthRequest(DecTree& tree) {
  int k = 0;
  std::cin >> k;
  std::string kth = tree.FindKthStat(k);
  std::cout << kth << "\n";
}

void SolveRequest(DecTree& tree, std::string& request) {
  if (request == "insert") {
    InsertRequest(tree);
  }
  if (request == "delete") {
    DeleteRequest(tree);
  }
  if (request == "exists") {
    ExistsRequest(tree);
  }
  if (request == "next") {
    NextRequest(tree);
  }
  if (request == "prev") {
    PrevRequest(tree);
  }
  if (request == "kth") {
    KthRequest(tree);
  }
}

void DecTree::Destroy(Node* root) {
  if (root == nullptr) {
    return;
  }
  if (root->left != nullptr) {
    Destroy(root->left);
  }
  if (root->right != nullptr) {
    Destroy(root->right);
  }
  delete root;
}

DecTree::Node* DecTree::Create(int key) {
  Node* new_node = new Node;
  const int kVarBillion = 1000000000;
  new_node->priority = 1 + rand() % kVarBillion;
  new_node->key = key;
  return new_node;
}

void DecTree::SetIndex(Node* tree) {
  tree->index = (tree->left != nullptr) ? tree->left->index + 1 : 1;
  tree->index += (tree->right != nullptr) ? tree->right->index : 0;
}

std::pair<DecTree::Node*, DecTree::Node*> DecTree::SplitSubTree(Node* cur,
                                                                int key) {
  if (cur == nullptr) {
    return {nullptr, nullptr};
  }
  if (cur->key <= key) {
    std::pair<Node*, Node*> pair_of_subtrees = SplitSubTree(cur->right, key);
    Node* left_subtree = cur;
    cur->right = pair_of_subtrees.first;
    if (cur->right != nullptr) {
      cur->right->pr = cur;
      SetIndex(cur->right);
    }
    Node* right_subtree = pair_of_subtrees.second;
    if (right_subtree != nullptr) {
      right_subtree->pr = nullptr;
      SetIndex(right_subtree);
    }
    return {left_subtree, right_subtree};
  }
  std::pair<Node*, Node*> pair_of_subtrees = SplitSubTree(cur->left, key);
  Node* left_subtree = pair_of_subtrees.first;
  if (left_subtree != nullptr) {
    left_subtree->pr = nullptr;
    SetIndex(left_subtree);
  }
  Node* right_subtree = cur;
  cur->left = pair_of_subtrees.second;
  if (cur->left != nullptr) {
    cur->left->pr = cur;
    SetIndex(cur->left);
  }
  return {left_subtree, right_subtree};
}

DecTree::Node* DecTree::InsertInSubTree(int key) {
  std::pair<Node*, Node*> splited = Split(key);
  if (splited.second != nullptr && splited.second->key == key) {
    return Merge(splited.first, splited.second);
  }
  Node* new_node = Create(key);
  return Merge(Merge(splited.first, new_node), splited.second);
}

DecTree::Node* DecTree::Merge(Node* tree1, Node* tree2) {
  if (tree1 == nullptr) {
    return tree2;
  }
  if (tree2 == nullptr) {
    return tree1;
  }
  if (tree1->priority > tree2->priority) {
    Node* right_subtree = Merge(tree1->right, tree2);
    tree1->right = right_subtree;
    if (right_subtree != nullptr) {
      right_subtree->pr = tree1;
      SetIndex(right_subtree);
    }
    SetIndex(tree1);
    tree1->pr = nullptr;
    return tree1;
  }
  Node* left_subtree = Merge(tree1, tree2->left);
  tree2->left = left_subtree;
  if (left_subtree != nullptr) {
    left_subtree->pr = tree2;
    SetIndex(left_subtree);
  }
  SetIndex(tree2);
  tree2->pr = nullptr;
  return tree2;
}

bool DecTree::KeyExistsInSubTree(Node* cur, int key) {
  if (cur == nullptr) {
    return false;
  }
  if (cur->key == key) {
    return true;
  }
  if (cur->key < key) {
    return KeyExistsInSubTree(cur->right, key);
  }
  return KeyExistsInSubTree(cur->left, key);
}

DecTree::Node* DecTree::GetNextInSubTree(Node* cur, int key) {
  if (cur == nullptr) {
    return nullptr;
  }
  if (cur->key == key) {
    if (cur->right == nullptr) {
      if (cur->pr == nullptr || cur->pr->key < cur->key) {
        return nullptr;
      }
      return cur->pr;
    }
    cur = cur->right;
    while (cur->left != nullptr) {
      cur = cur->left;
    }
    return cur;
  }
  if (cur->key < key) {
    Node* found = GetNextInSubTree(cur->right, key);
    if (found != nullptr && found->key >= key) {
      return found;
    }
    return nullptr;
  }
  if (cur->key > key && cur->left != nullptr) {
    Node* found = GetNextInSubTree(cur->left, key);
    if (found == nullptr) {
      return cur;
    }
    if (found->key >= key) {
      return found;
    }
    return nullptr;
  }
  return cur;
}

DecTree::Node* DecTree::GetPrevInSubTree(Node* cur, int key) {
  if (cur == nullptr) {
    return nullptr;
  }
  if (cur->key == key) {
    if (cur->left == nullptr) {
      if (cur->pr == nullptr || cur->pr->key > cur->key) {
        return nullptr;
      }
      return cur->pr;
    }
    cur = cur->left;
    while (cur->right != nullptr) {
      cur = cur->right;
    }
    return cur;
  }
  if (cur->key > key) {
    Node* found = GetPrevInSubTree(cur->left, key);
    if (found != nullptr && found->key <= key) {
      return found;
    }
    return nullptr;
  }
  if (cur->key < key && cur->right != nullptr) {
    Node* found = GetPrevInSubTree(cur->right, key);
    if (found == nullptr) {
      return cur;
    }
    if (found->key <= key) {
      return found;
    }
    return nullptr;
  }
  return cur;
}

DecTree::Node* DecTree::FindKthStatInSubTree(Node* cur, int k) {
  if (cur == nullptr) {
    return nullptr;
  }
  if (cur->right == nullptr) {
    if (cur->index == k + 1) {
      return cur;
    }
    if (cur->index > k + 1) {
      return FindKthStatInSubTree(cur->left, k);
    }
    return nullptr;
  }
  if (cur->index - cur->right->index == k + 1) {
    return cur;
  }
  if (cur->index - cur->right->index > k + 1) {
    return FindKthStatInSubTree(cur->left, k);
  }
  int left_index = (cur->left != nullptr) ? cur->left->index : 0;
  return FindKthStatInSubTree(cur->right, k - left_index - 1);
}

void DecTree::Insert(int key) {
  if (Exists(key)) {
    return;
  }
  root_ = InsertInSubTree(key);
}

void DecTree::Delete(int key) {
  if (!Exists(key)) {
    return;
  }
  std::pair<Node*, Node*> pair_of_trees = SplitSubTree(root_, key);
  std::pair<Node*, Node*> pair_of_left_subtrees =
      SplitSubTree(pair_of_trees.first, key - 1);
  Node* new_root = Merge(pair_of_left_subtrees.first, pair_of_trees.second);
  delete pair_of_left_subtrees.second;
  root_ = new_root;
}

bool DecTree::Exists(int key) { return KeyExistsInSubTree(root_, key); }

std::string DecTree::Next(int key) {
  Node* next = GetNextInSubTree(root_, key);
  if (next == nullptr) {
    return "none";
  }
  return std::to_string(next->key);
}

std::string DecTree::Prev(int key) {
  Node* prev = GetPrevInSubTree(root_, key);
  if (prev == nullptr) {
    return "none";
  }
  return std::to_string(prev->key);
}

std::string DecTree::FindKthStat(int k) {
  Node* kth = FindKthStatInSubTree(root_, k);
  if (kth == nullptr) {
    return "none";
  }
  return std::to_string(kth->key);
}

std::pair<DecTree::Node*, DecTree::Node*> DecTree::Split(int key) {
  return SplitSubTree(root_, key);
}

int main() {
  std::string request;
  DecTree tree;
  while (std::cin >> request) {
    SolveRequest(tree, request);
  }
  return 0;
}

