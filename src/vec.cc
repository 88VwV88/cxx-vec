#include <memory>

template<typename T, typename A=std::allocator<T>>
class Vec {
    T* start;
    T* finish;
    T* end;
    constexpr void alloc(const size_t N) const {
        return (N != 0)
            ? new T[N]
            : nullptr;
    }
public:
    constexpr Vec() const noexcept: {} 
    constexpr Vec(Vec& v) const 
    constexpr Vec(Vec&& v) const noexcept {
        self.start = v.start;
        self.finish = v.finish;
        self.end = v.end;
    }
};