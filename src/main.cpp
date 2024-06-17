#include "vec.cpp"
#include <iostream>

int main() {
  Vec<int> v{};
  for (int i=0; i<10; ++i) {
    v.push(i);
  }
  std::cout << "[";
  for (const auto x: v) {
    std::cout << x << ", ";
  } std::cout << "\b\b]" << std::endl;

  return 0;
}