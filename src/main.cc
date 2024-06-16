#include <iostream>
#include "vec.cc"


int main() {
    Vec<int> v{};
    for (int i=0; i<100; ++i) {
        printf("capacity: %ld, size: %ld storage: %ld\n", 
            v.capacity(), 
            v.size(), 
            v.capacity() + v.size());
        v.push(i);
    }
    printf("capacity: %ld, size: %ld, storage: %ld\n", 
        v.capacity(), 
        v.size(), 
        v.capacity() + v.size());
    
    return 0;
}