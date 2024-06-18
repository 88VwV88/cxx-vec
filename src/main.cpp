#include "vec.cpp"
#include <iostream>

int main() {
  // shrink_to_fit, resize, get_allocator, max_size
  Vec<int> v{};

  std::cout << "storage: " << v.capacity() + v.size() << '\n';
  v.insert(v.cbegin(), {1, 2, 3, 4, 5});
  v.insert(v.cbegin(), 5, 10);
  v.insert(v.cend(), 5, 11);
  v.resize(31);

  std::cout << "[";
  for (const auto &x: v)
    std::cout << x << ", ";
  std::cout << "\b\b]" << std::endl;
  std::cout << "storage: " << v.capacity() + v.size() << '\n';

  return 0;
}