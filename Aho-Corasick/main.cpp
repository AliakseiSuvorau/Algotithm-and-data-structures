#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
///                                   Trie                                   ///
////////////////////////////////////////////////////////////////////////////////

struct Node {
  std::unordered_map<char, size_t> to;
  bool is_term = false;
  ssize_t link = -1;
};

class Trie {
  std::vector<Node> trie_;
  void MakeRoot();

 public:
  Trie();
  void AddString(std::string& str);
  std::unordered_map<char, size_t>& operator[](size_t idx);
  size_t GetLink(size_t vertex);
  void SetLink(size_t vertex, size_t new_link);
  bool IsTerm(size_t vertex);
  size_t GetNumOfVertexes();
};

void Trie::AddString(std::string& str) {
    size_t vertex = 0;
    for (size_t i = 0; i < str.length(); ++i) {
      if (!trie_[vertex].to.contains(str[i])) {
        trie_.push_back(Node());
        trie_[vertex].to[str[i]] = trie_.size() - 1;
      }
      vertex = trie_[vertex].to[str[i]];
    }
    trie_[vertex].is_term = true;
}

void Trie::MakeRoot() { trie_.push_back(Node()); }

Trie::Trie() { MakeRoot(); }

std::unordered_map<char, size_t>& Trie::operator[](size_t idx) {
  return trie_[idx].to;
}

size_t Trie::GetLink(size_t vertex) { return trie_[vertex].link; }

void Trie::SetLink(size_t vertex, size_t new_link) {
  trie_[vertex].link = new_link;
  if (trie_[new_link].is_term) {
    trie_[vertex].is_term = true;
  }
}

bool Trie::IsTerm(size_t vertex) { return trie_[vertex].is_term; }

size_t Trie::GetNumOfVertexes() { return trie_.size(); }

////////////////////////////////////////////////////////////////////////////////
///                          Aho-Corasick algorithm                          ///
////////////////////////////////////////////////////////////////////////////////

class AhoCorasickAlgorithmHelper {
  Trie& trie_;
  std::vector<bool> used_;

  void ProcessRoot();
  bool Dfs(size_t vertex, std::vector<size_t>& queue);

 public:
  AhoCorasickAlgorithmHelper(Trie& trie);
  bool HasCycle();
};

AhoCorasickAlgorithmHelper::AhoCorasickAlgorithmHelper(Trie& trie) : trie_(trie) {
  // root
  ProcessRoot();
  // BFS
  std::queue<size_t> queue;
  queue.push(0);
  while (!queue.empty()) {
    size_t cur = queue.front();
    queue.pop();
    for (char symbol = '0'; symbol <= '1'; ++symbol) {
      size_t child = trie_[cur][symbol];
      // if there is an edge, there's no need for link
      if (trie_.GetLink(child) != -1) {
        continue;
      }
      // otherwise, setting a link
      if (cur == 0) {
        trie_.SetLink(child, 0);
      } else {
        trie_.SetLink(child, trie_[trie_.GetLink(cur)][symbol]);
      }
      // processing letters
      for (char letter = '0'; letter <= '1'; ++letter) {
        if (trie_[child].contains(letter)) {
          continue;
        }
        trie_[child][letter] = trie_[trie_.GetLink(child)][letter];
      }
      queue.push(child);
    }
  }
}

void AhoCorasickAlgorithmHelper::ProcessRoot() {
  trie_.SetLink(0, 0);
  for (char letter = '0'; letter <= '1'; ++letter) {
    if (trie_[0].contains(letter)) {
      continue;
    }
    trie_[0][letter] = 0;
  }
}

bool AhoCorasickAlgorithmHelper::HasCycle() {
  used_.resize(trie_.GetNumOfVertexes());
  std::vector<size_t> queue;
  return Dfs(0, queue);
}

bool AhoCorasickAlgorithmHelper::Dfs(size_t vertex,std::vector<size_t>& queue) {
  bool has_cycle = false;
  used_[vertex] = true;
  queue.push_back(vertex);
  for (char letter = '0'; letter <= '1'; ++letter) {
    size_t child = trie_[vertex][letter];
    if (trie_.IsTerm(child)) {
      continue;
    }
    if (!used_[child]) {
      has_cycle |= Dfs(child, queue);
    } else {
      if (std::find(queue.begin(), queue.end(), child) != queue.end()) {
        return true;
      }
      continue;
    }
  }
  queue.pop_back();
  return has_cycle;
}

int main() {
  size_t num_of_strings = 0;
  std::cin >> num_of_strings;
  Trie trie;
  std::string str;
  for (size_t i = 0; i < num_of_strings; ++i) {
    std::cin >> str;
    trie.AddString(str);
  }
  AhoCorasickAlgorithmHelper helper(trie);
  if (helper.HasCycle()) {
    std::cout << "TAK\n";
  } else {
    std::cout << "NIE\n";
  }
  return 0;
}
