#ifndef LIBFILESYNC_PROTOCOL_MEMORY_HANDLE_HPP
#define LIBFILESYNC_PROTOCOL_MEMORY_HANDLE_HPP

#include <span>

namespace filesync::protocol::memory {

    template<typename ElementType>
    class Handle {

        public:
            std::span<ElementType> data() const;

        private:
            virtual std::span<ElementType> doData() const = 0;

    };

}

#include <libfilesync/protocol/memory/Handle.tpp>

#endif