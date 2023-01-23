#include <iostream>

class AvlTree {
  struct Node {
    int key = 0;
    int height = 1;
    Node* left = nullptr;
    Node* right = nullptr;
  };

  Node* root_ = nullptr;

 private:
  void DestroyTree(Node* cur);
  void PutHeight(Node* cur);
  Node* RotateRight(Node* cur);
  Node* RotateLeft(Node* cur);
  int FindDelta(Node* cur);
  Node* Balance(Node* cur);
  Node* FindMinGreaterThanCur(Node* cur, int key);
  Node* InsertSubTree(Node* cur, int key, int index);

 public:
  AvlTree() = default;
  void Insert(int key, int index);
  int GetNextVal(int key);
  ~AvlTree() { DestroyTree(root_); }
};

void AvlTree::DestroyTree(Node* cur) {
  if (cur == nullptr) {
    delete cur;
    return;
  }
  if (cur->left != nullptr) {
    DestroyTree(cur->left);
  }
  if (cur->right != nullptr) {
    DestroyTree(cur->right);
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

AvlTree::Node* AvlTree::FindMinGreaterThanCur(Node* cur, int key) {
  if (cur != nullptr && cur->key == key) {
    return cur;
  }
  Node* pr = nullptr;
  if (cur != root_ || (cur != nullptr && cur->key > key)) {
    while (cur->left != nullptr && cur->left->key >= key) {
      cur = cur->left;
    }
    if (cur != nullptr && cur->key == key) {
      return cur;
    }
    if (cur != nullptr && cur->left == nullptr && cur->key >= key) {
      return cur;
    }
    pr = cur;
    if (cur != nullptr && cur->left != nullptr) {
      cur = cur->left;
    }
  }
  while (cur != nullptr && cur->right != nullptr && cur->right->key <= key) {
    cur = cur->right;
  }
  if (cur != nullptr && cur->key == key) {
    return cur;
  }
  if (cur != nullptr && cur->right == nullptr) {
    if (pr != nullptr && pr->key >= key) {
      return pr;
    }
    if (pr == nullptr) {
      return nullptr;
    }
  }
  if (cur != nullptr) {
    cur = cur->right;
  }
  return FindMinGreaterThanCur(cur, key);
}

AvlTree::Node* AvlTree::InsertSubTree(Node* cur, int key, int index) {
  if (cur == nullptr) {
    Node* new_root = new Node;
    new_root->key = key;
    cur = new_root;
  }
  if (key < cur->key) {
    cur->left = InsertSubTree(cur->left, key, index);
  }
  if (key > cur->key) {
    cur->right = InsertSubTree(cur->right, key, index);
  }
  return Balance(cur);
}

void AvlTree::Insert(int key, int index) {
  root_ = InsertSubTree(root_, key, index);
}

int AvlTree::GetNextVal(int key) {
  Node* found = FindMinGreaterThanCur(root_, key);
  if (found == nullptr) {
    return -1;
  }
  return found->key;
}

