#include <cstdlib>

namespace filesync::curl::storage {

    template<typename T>
    ConsumingAllocator<T>::ConsumingAllocator(value_type* data, size_type size) :
        data{data},
        size{size} {

    }

    template<typename T>
    ConsumingAllocator<T>::ConsumingAllocator(const ConsumingAllocator& other) throw() :
        data{other.data},
        size{other.size} {

    }

    template<typename T>
    template<typename U>
    ConsumingAllocator<T>::ConsumingAllocator(const ConsumingAllocator<U>& other) throw() :
        data{other.data}, size{other.size} {

    }

    template<typename T>
    ConsumingAllocator<T>& ConsumingAllocator<T>::operator=(const ConsumingAllocator& other) {
        return *this; 
    }

    template<typename T>
    template<typename U>
    ConsumingAllocator<T>& ConsumingAllocator<T>::operator=(
        const ConsumingAllocator<U>& other) { 
            
        return *this; 
    }

    template<typename T>
    ConsumingAllocator<T>::value_type* ConsumingAllocator<T>::allocate(size_type n) {
        return data;
    }

    template<typename T>
    void ConsumingAllocator<T>::deallocate(value_type* ptr, size_type n) {
        std::free(ptr);
    }

    template<typename T>
    ConsumingAllocator<T>::size_type ConsumingAllocator<T>::max_size() const {
        return size;
    }

}