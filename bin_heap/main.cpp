#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

struct Item {
  int64_t value;
  int index;
  int num_of_request;
};
class MyHeap {
  std::vector<Item> my_heap_;
  std::vector<int> requests_;
  int size_ = 0;

 public:
  MyHeap() = default;
  ~MyHeap() = default;

 private:
  void SwapElements(int first, int second) {
    Item tmp = my_heap_[first];
    my_heap_[first] = my_heap_[second];
    my_heap_[second] = tmp;
    requests_[my_heap_[first].num_of_request] = first;
    requests_[my_heap_[second].num_of_request] = second;
    my_heap_[first].index = first;
    my_heap_[second].index = second;
  }

  void SiftUp(int i) {
    if (i > 0 && my_heap_[i].value < my_heap_[(i - 1) / 2].value) {
      SwapElements(i, (i - 1) / 2);
      SiftUp((i - 1) / 2);
    }
  }

  void SiftDown(int i) {
    if (2 * i + 1 < size_) {
      if (2 * i + 2 == size_) {
        if (my_heap_[i].value > my_heap_[2 * i + 1].value) {
          SwapElements(i, 2 * i + 1);
        }
      } else {
        if (my_heap_[i].value > my_heap_[2 * i + 1].value &&
            my_heap_[2 * i + 1].value < my_heap_[2 * i + 2].value) {
          SwapElements(i, 2 * i + 1);
          SiftDown(2 * i + 1);
        } else {
          if (my_heap_[i].value > my_heap_[2 * i + 1].value) {
            SwapElements(i, 2 * i + 2);
            SiftDown(2 * i + 2);
          } else {
            if (my_heap_[i].value > my_heap_[2 * i + 2].value) {
              SwapElements(i, 2 * i + 2);
              SiftDown(2 * i + 2);
            }
          }
        }
      }
    }
  }

 public:
  void Insert(int64_t x) {
    my_heap_.push_back({x, size_, int(requests_.size())});
    ++size_;
    requests_.push_back(size_ - 1);
    SiftUp(size_ - 1);
  }

  int64_t GetMin() {
    requests_.push_back(-1);
    return my_heap_[0].value;
  }

  void ExtractMin() {
    requests_.push_back(-1);
    SwapElements(0, size_ - 1);
    my_heap_.pop_back();
    --size_;
    SiftDown(0);
  }

  void DecreaseKey(int64_t index, int64_t delta) {
    requests_.push_back(-1);
    my_heap_[requests_[index - 1]].value -= delta;
    SiftUp(my_heap_[requests_[index - 1]].index);
  }
};

void Optimize() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}

int main() {
  Optimize();
  int number_of_requests;
  MyHeap heap;
  std::cin >> number_of_requests;
  for (int i = 0; i < number_of_requests; ++i) {
    std::string request;
    std::cin >> request;
    if (request == "insert") {
      int64_t number;
      std::cin >> number;
      heap.Insert(number);
      continue;
    }
    if (request == "decreaseKey") {
      int64_t delta;
      int index;
      std::cin >> index >> delta;
      heap.DecreaseKey(index, delta);
      continue;
    }
    if (request == "getMin") {
      int64_t heap_min = heap.GetMin();
      std::cout << heap_min << "\n";
      continue;
    }
    heap.ExtractMin();
  }
  return 0;
}
