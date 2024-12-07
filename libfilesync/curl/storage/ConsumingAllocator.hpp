#ifndef LIBFILESYNC_CURL_STORAGE_CONSUMING_ALLOCATOR_HPP
#define LIBFILESYNC_CURL_STORAGE_CONSUMING_ALLOCATOR_HPP

#include <cstddef>
#include <memory>

namespace filesync::curl::storage {

    template<typename T>
    class ConsumingAllocator : public std::allocator<char> {

        public:
            typedef std::size_t size_type;
            typedef T value_type;

            ConsumingAllocator(value_type* data, size_type size);
            ConsumingAllocator(const ConsumingAllocator& other) throw();

            template<typename U>
            ConsumingAllocator(const ConsumingAllocator<U>& other) throw();
            ConsumingAllocator& operator=(const ConsumingAllocator& other);

            template<typename U>
            ConsumingAllocator& operator=(const ConsumingAllocator<U>& other);
            
            ~ConsumingAllocator() = default;

            value_type* allocate(size_type n);
            void deallocate(value_type* ptr, size_type n);

            size_type max_size() const;
   
        private:
            char* data;
            std::size_t size;
    
    };

}

#include <libfilesync/curl/storage/ConsumingAllocator.tpp>

#endif