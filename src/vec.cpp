#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>

template <typename T, typename _Alloc = std::allocator<T>> class Vec {
private:
  typedef typename std::allocator_traits<_Alloc>::template rebind_alloc<T> _A;

  _A A{};
  typedef std::allocator_traits<_A> Tr;

  typedef Tr::pointer pointer;
  typedef Tr::value_type value_type;
  typedef Tr::difference_type difference_type;

  typedef value_type &reference;

  struct iterator {
    using iterator_category = std::random_access_iterator_tag;
    using value_type = value_type;
    using pointer = pointer;
    using reference = reference;
    using difference_type = difference_type;

    pointer ptr;

    constexpr iterator(pointer _ptr) : ptr(_ptr) {}
    constexpr bool operator!=(iterator &it) { return ptr != it.ptr; }
    constexpr reference operator*() { return *ptr; }
    constexpr iterator const &operator++() {
      ptr++;
      return *this;
    }
  };
  typedef const iterator const_iterator;

  pointer start;
  pointer finish;
  pointer store_end;

  constexpr pointer vec_alloc(size_t n) {
    return n != 0 ? A.allocate(n) : pointer();
  }
  constexpr void dealloc() {
    if (start != pointer()) {
      A.deallocate(start, size());
    }
    finish = store_end = pointer();
  }
  constexpr void range_check(size_t index) const {
    if (index >= size()) {
      throw std::out_of_range("Vec<> index out of bounds");
    }
  }

public:
  constexpr Vec() noexcept { start = finish = store_end = pointer(); }
  constexpr Vec(Vec &&v) noexcept
      : start(v.start), finish(v.finish), store_end(v.store_end) {
    v.start = v.finish = v.store_end = pointer();
  }
  constexpr ~Vec() noexcept { dealloc(); }

  constexpr iterator begin() { return start; }
  constexpr iterator end() { return finish; }
  constexpr const_iterator cbegin() const { return start; }
  constexpr const_iterator cend() const { return finish; }

  constexpr size_t size() const noexcept { return finish - start; }
  constexpr size_t capacity() const noexcept { return store_end - finish; }
  constexpr void operator=(Vec &&v) noexcept {
    start = v.start;
    finish = v.finish;
    store_end = v.store_end;

    v.start = v.finish = v.store_end = pointer();
  }
  constexpr reference operator[](const size_t index) const { return at(index); }

  constexpr void push(T value) noexcept {
    if (size() == 0) {
      start = finish = vec_alloc(1);
      store_end = start + 1;
    } else if (capacity() == 0) {
      Vec tmp{};
      tmp.start = vec_alloc(2 * size());
      tmp.finish = tmp.start + size();
      tmp.store_end = tmp.finish + size();

      std::copy(start, finish, tmp.start);
      dealloc();

      *this = std::move(tmp);
    }
    *finish = value;
    finish++;
  }
  constexpr void pop() noexcept {
    if (size() == 0)
      return;

    *finish-- = value_type();
  }
  constexpr reference at(size_t index) const {
    range_check(index);

    return *(start + index);
  }
};