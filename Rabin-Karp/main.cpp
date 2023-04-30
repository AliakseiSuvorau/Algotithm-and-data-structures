#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

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
  Trie(std::string& str);
  void AddString(std::string& str);
  std::unordered_map<char, size_t>& operator[](size_t idx);
  size_t GetLink(size_t vertex);
  void SetLink(size_t vertex, size_t new_link);
  bool IsTerm(size_t vertex);
};

Trie::Trie(std::string& str) {
  MakeRoot();
  AddString(str);
}

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
}

bool Trie::IsTerm(size_t vertex) { return trie_[vertex].is_term; }

////////////////////////////////////////////////////////////////////////////////
///                          Aho-Corasick algorithm                          ///
////////////////////////////////////////////////////////////////////////////////

class AhoCorasickAlgorithmHelper {
  Trie& trie_;

  void ProcessRoot();

 public:
  AhoCorasickAlgorithmHelper(Trie& trie);
  size_t CountEntries(std::string& text);
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
    for (char symbol = 'a'; symbol <= 'z'; ++symbol) {
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
      for (char letter = 'a'; letter <= 'z'; ++letter) {
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
  for (char letter = 'a'; letter <= 'z'; ++letter) {
    if (trie_[0].contains(letter)) {
      continue;
    }
    trie_[0][letter] = 0;
  }
}

size_t AhoCorasickAlgorithmHelper::CountEntries(std::string& text) {
  size_t num_of_entries = 0;
  size_t cur = 0;
  for (char letter : text) {
    if (trie_.IsTerm(cur)) {
      ++num_of_entries;
    }
    cur = trie_[cur][letter];
  }
  if (trie_.IsTerm(cur)) {
    ++num_of_entries;
  }
  return num_of_entries;
}

void CreateTrie(std::string& pattern, Trie& trie) {
  std::string cyclic_pattern = pattern + pattern;
  cyclic_pattern.pop_back();
  for (size_t i = 0; i < pattern.length(); ++i) {
    std::string shift = cyclic_pattern.substr(i, pattern.length());
    trie.AddString(shift);
  }
}

int main() {
  std::string pattern;
  std::cin >> pattern;
  Trie trie;
  CreateTrie(pattern, trie);
  AhoCorasickAlgorithmHelper helper(trie);
  std::string text;
  std::cin >> text;
  std::cout << helper.CountEntries(text);
  return 0;
}
