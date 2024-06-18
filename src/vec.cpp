#include <memory>
#include <utility>
#include <iterator>
#include <algorithm>

template <typename T, typename _Alloc = std::allocator<T>> class Vec {
private:
  typedef typename std::allocator_traits<_Alloc>::template rebind_alloc<T> _A;

  _A A{};
  typedef std::allocator_traits<_A> Tr;

  typedef Tr::pointer pointer;
  typedef Tr::size_type size_type;
  typedef Tr::value_type value_type;
  typedef Tr::allocator_type allocator_type;
  typedef Tr::difference_type difference_type;

  typedef value_type &reference;

  struct iterator {
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = value_type;
    using pointer = pointer;
    using reference = reference;
    using difference_type = difference_type;

    pointer ptr;

    constexpr iterator(pointer _ptr) : ptr(_ptr) {}
    constexpr reference operator*() { return *ptr; }
    constexpr pointer operator->() { return ptr; }
    constexpr iterator &operator++() {
      ptr++;
      return *this;
    }
    constexpr iterator &operator--() {
      --ptr;
      return *this;
    }
    constexpr iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    friend constexpr bool operator==(const iterator &a, const iterator &b) {
      return a.ptr == b.ptr;
    }
    friend constexpr bool operator!=(const iterator &a, const iterator &b) {
      return a.ptr != b.ptr;
    }
  };
  typedef const iterator const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef const reverse_iterator const_reverse_iterator;

  pointer start;
  pointer finish;
  pointer store_end;

  [[nodiscard]] constexpr pointer vec_alloc(size_type n) {
    return n != 0 ? A.allocate(n) : pointer();
  }
  constexpr void dealloc() {
    if (start != pointer()) {
      A.deallocate(start, size());
    }
    finish = store_end = pointer();
  }
  constexpr void range_check(const size_type index) const {
    if (start != pointer() && index >= size())
      throw std::out_of_range("Vec<> index out of bounds");
  }

public:
  constexpr Vec() noexcept { start = finish = store_end = pointer(); }
  constexpr Vec(Vec &&v) noexcept
      : start(v.start), finish(v.finish), store_end(v.store_end) {
    v.start = v.finish = v.store_end = pointer();
  }
  constexpr ~Vec() noexcept { dealloc(); }

  [[nodiscard]] constexpr iterator begin() { return start; }
  [[nodiscard]] constexpr iterator end() { return finish; }
  [[nodiscard]] constexpr const_iterator cbegin() const { return start; }
  [[nodiscard]] constexpr const_iterator cend() const { return finish; }
  [[nodiscard]] constexpr reverse_iterator rbegin() {
    return std::make_reverse_iterator(end());
  }
  [[nodiscard]] constexpr reverse_iterator rend() {
    return std::make_reverse_iterator(begin());
  }
  [[nodiscard]] constexpr const_reverse_iterator crbegin() { return rbegin(); }
  [[nodiscard]] constexpr const_reverse_iterator crend() { return rend(); }

  [[nodiscard]] constexpr size_type size() const noexcept {
    return finish - start;
  }
  [[nodiscard]] constexpr size_type capacity() const noexcept {
    return store_end - finish;
  }
  constexpr void operator=(Vec &&v) noexcept {
    start = v.start;
    finish = v.finish;
    store_end = v.store_end;

    v.start = v.finish = v.store_end = pointer();
  }
  [[nodiscard]] constexpr reference operator[](const size_type index) const {
    return at(index);
  }

  constexpr void push_back(value_type &&value) noexcept { emplace_back(value); }
  constexpr void push_back(const value_type &value) noexcept {
    emplace_back(value);
  }
  constexpr void pop_back() noexcept {
    if (size() == 0)
      return;

    *finish-- = value_type();
  }
  [[nodiscard]] constexpr reference at(size_type index) const {
    range_check(index);

    return *(start + index);
  }
  constexpr void assign(std::initializer_list<value_type> l) {
    (*this) = Vec<T>{};
    for (const auto &v : l)
      push_back(v);
  }
  [[nodiscard]] constexpr reference front() const { return *begin(); }
  [[nodiscard]] constexpr reference back() const { return *(finish - 1); }
  template <typename... Args>
    requires std::constructible_from<value_type, Args...>
  constexpr iterator emplace(const_iterator pos, Args &&...args) {
    iterator it = pos;
    if (capacity() == 0)
      push_back(value_type());

    size_type tsize = store_end - pos.ptr;
    printf("%ld\n", tsize + 1);

    pointer tmp = vec_alloc(tsize + 1);
    std::copy(pos.ptr, store_end, tmp + 1);
    std::construct_at(tmp, std::forward<Args>(args)...);

    std::copy(tmp, tmp + tsize + 1, it.ptr);
    finish++;

    return it;
  }
  template <typename... Args>
    requires std::constructible_from<value_type, Args...>
  constexpr void emplace_back(Args &&...args) {
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

    std::construct_at(finish, std::forward<Args>(args)...);
    finish++;
  }
  [[nodiscard]] constexpr pointer data() { return start; }
  constexpr void swap(Vec<T> &v) noexcept {
    v.start = std::exchange(start, v.start);
    v.finish = std::exchange(finish, v.finish);
    v.store_end = std::exchange(store_end, v.store_end);
  }
  constexpr void erase(const_iterator pos) {
    range_check(pos.ptr - start);
    for (auto &it = pos; it != end(); ++it) {
      *it = value_type();
    }
  }
  constexpr void erase(const_iterator first, const_iterator last) {
    range_check(first.ptr - start);
    range_check(last.ptr - start);
    static_assert(last.ptr - first.ptr > 0);
    for (auto &it = first; it != last; ++it) {
      *it = value_type();
    }
  }
  constexpr void clear() { start = finish = store_end = pointer(); }
  [[nodiscard]] constexpr bool empty() const { return size() == 0; }
  constexpr void insert(const_iterator pos, value_type &&x) {
    if (pos == end()) {
      push_back(x);
      return;
    }
    range_check(pos.ptr - start);
    emplace(pos, x);
  }
  constexpr void insert(const_iterator pos, const value_type &x) {
    if (pos == end()) {
      push_back(x);
      return;
    }
    emplace(pos, x);
  }
  constexpr void insert(const_iterator pos,
                        std::initializer_list<value_type> l) {
    if (pos == end()) {
      for (const auto &x : l)
        push_back(x);
      return;
    }
    range_check(pos.ptr - start);

    size_type tsize = size() + l.size();
    iterator it = pos;
    if (capacity() < l.size()) {
      pointer tmp = vec_alloc(tsize);
      std::copy(start, pos.ptr, tmp);
      it = tmp + (pos.ptr - start);
      std::copy(pos.ptr, finish, it.ptr + l.size());

      dealloc();
      start = tmp;
      finish = tmp + tsize;
      store_end = finish;
    }
    for (const auto &x : l) {
      *it++ = x;
    }
  }
  constexpr void insert(const_iterator pos, size_type n, value_type &&x) {
    if (pos == end()) {
      while (n-- > 0)
        push_back(x);
      return;
    }
    range_check(pos.ptr - start);

    size_type tsize = size() + n;
    iterator it = pos;
    if (capacity() < n) {
      pointer tmp = vec_alloc(tsize);
      std::copy(start, pos.ptr, tmp);
      it = tmp + (pos.ptr - start);
      std::copy(pos.ptr, finish, it.ptr + n);

      dealloc();
      start = tmp;
      finish = tmp + tsize;
      store_end = finish;
    }
    while (n-- > 0) {
      *it++ = x;
    }
  }
  constexpr void insert(const_iterator pos, size_type n, const value_type &x) {
    if (pos == end()) {
      while (n-- > 0)
        push_back(x);
      return;
    }
    range_check(pos.ptr - start);

    size_type tsize = size() + n;
    iterator it = pos;
    if (capacity() < n) {
      pointer tmp = vec_alloc(tsize);
      std::copy(start, pos.ptr, tmp);
      it = tmp + (pos.ptr - start);
      std::copy(pos.ptr, finish, it.ptr + n);

      dealloc();
      start = tmp;
      finish = tmp + tsize;
      store_end = finish;
    }
    while (n-- > 0) {
      *it++ = x;
    }
  }
  [[nodiscard]] constexpr allocator_type get_allocator() const { return {}; }
  constexpr void reserve(size_type n) noexcept {
    if (capacity() < n) {
      size_type tsize = size();
      pointer tmp = vec_alloc(tsize + n);
      std::copy(start, finish, tmp);

      dealloc();
      start = tmp;
      finish = tmp + tsize;
      store_end = finish + n;
    }
  }
  constexpr void resize(size_type n) noexcept {
    if (n == size())
      return;
    size_type tsize = size();
    
    pointer tmp = vec_alloc(n);
    std::copy(start, start + std::min(tsize, n), tmp);

    start = tmp;
    finish = tmp + std::min(tsize, n);
    store_end = tmp + n;
  }
  constexpr void shrink_to_fit() {
    store_end = finish;
  }
};