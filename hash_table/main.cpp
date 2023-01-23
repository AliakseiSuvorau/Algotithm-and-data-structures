#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

template <typename T>
class HashTable {
  std::vector<std::list<T>> hash_table_;
  size_t capacity_ = 0;
  const double kMaxFactor = 1.;
  size_t num_of_elements_ = 0;

  bool CheckLoad();
  void Extend();

 public:
  HashTable() = default;
  ~HashTable() = default;
  void Append(T value);
  void Delete(T value);
  bool IsIn(T value);
};

template <typename T>
bool HashTable<T>::CheckLoad() {
  if (capacity_ == 0) {
    return true;
  }
  double load_factor = 1. * num_of_elements_ / capacity_;
  return load_factor > kMaxFactor;
}

template <typename T>
void HashTable<T>::Extend() {
  if (capacity_ == 0) {
    capacity_ = 1;
  } else {
    capacity_ *= 2;
  }
  std::vector<std::list<T>> new_hash_table(capacity_);
  for (size_t i = 0; i < hash_table_.size(); ++i) {
    for (auto it = hash_table_[i].begin(); it != hash_table_[i].end(); ++it) {
      size_t hash = std::hash<T>{}(*it) % capacity_;
      new_hash_table[hash].push_back(*it);
    }
  }
  hash_table_ = new_hash_table;
}

template <typename T>
void HashTable<T>::Append(T value) {
  if (CheckLoad()) {
    Extend();
  }
  if (IsIn(value)) {
    return;
  }
  size_t hash = std::hash<T>{}(value) % capacity_;
  hash_table_[hash].push_back(value);
  ++num_of_elements_;
}

template <typename T>
bool HashTable<T>::IsIn(T value) {
  if (capacity_ == 0) {
    return false;
  }
  size_t hash = std::hash<T>{}(value) % capacity_;
  return std::find(hash_table_[hash].begin(), hash_table_[hash].end(), value) !=
         hash_table_[hash].end();
}

template <typename T>
void HashTable<T>::Delete(T value) {
  if (IsIn(value)) {
    size_t hash = std::hash<T>{}(value) % capacity_;
    hash_table_[hash].erase(
        std::find(hash_table_[hash].begin(), hash_table_[hash].end(), value));
  }
}

template <typename T>
void WorkOutRequest(HashTable<T>& hash_table) {
  char type_of_request = 0;
  T value;
  std::cin >> type_of_request >> value;
  switch (type_of_request) {
    case '+':
      hash_table.Append(value);
      break;
    case '?':
      if (hash_table.IsIn(value)) {
        std::cout << "YES\n";
      } else {
        std::cout << "NO\n";
      }
      break;
    case '-':
      hash_table.Delete(value);
  }
}

int main() {
  int num_of_request = 0;
  std::cin >> num_of_request;
  HashTable<int> hash_table;
  for (int i = 0; i < num_of_request; ++i) {
    WorkOutRequest(hash_table);
  }
  return 0;
}
