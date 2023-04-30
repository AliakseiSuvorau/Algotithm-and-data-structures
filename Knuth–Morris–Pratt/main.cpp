#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
///                     Knuth–Morris–Pratt algorithm                         ///
////////////////////////////////////////////////////////////////////////////////

class KnuthMorrisPrattAlgorithmHelper {
  std::vector<size_t> prefix_function_;
  std::string text_;

  void ProcessPrefixFunction(std::string& text);

 public:
  KnuthMorrisPrattAlgorithmHelper() = default;
  KnuthMorrisPrattAlgorithmHelper(std::string text) : text_(text) {}
  std::vector<size_t> GetEntries(std::string pattern, char delim = '#');
};

void KnuthMorrisPrattAlgorithmHelper::ProcessPrefixFunction(std::string& text) {
  prefix_function_.clear();
  prefix_function_.resize(text.length());
  for (size_t i = 1; i < text.length(); ++i) {
    size_t pref_idx = prefix_function_[i - 1];
    while (pref_idx > 0 && text[i] != text[pref_idx]) {
      pref_idx = prefix_function_[pref_idx - 1];
    }
    if (text[i] == text[pref_idx]) {
      prefix_function_[i] = pref_idx + 1;
    } else {
      prefix_function_[i] = pref_idx;
    }
  }
}

std::vector<size_t> KnuthMorrisPrattAlgorithmHelper::GetEntries(std::string pattern, char delim) {
  std::vector <size_t> entries;
  std::string text_for_search = pattern + delim + text_;
  ProcessPrefixFunction(text_for_search);
  size_t pattern_length = pattern.length();
  for (size_t i = pattern.length() + 1; i < text_for_search.length(); ++i) {
    if (prefix_function_[i] == pattern_length) {
      entries.push_back(i - 2 * pattern_length);
    }
  }
  return entries;
}

int main() {
  std::string text;
  std::cin >> text;
  KnuthMorrisPrattAlgorithmHelper helper(text);
  std::string pattern;
  std::cin >> pattern;
  std::vector<size_t> entry_indexes = helper.GetEntries(pattern);
  for (auto& entry : entry_indexes) {
    std::cout << entry << "\n";
  }
  return 0;
}
