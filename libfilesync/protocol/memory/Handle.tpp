#include <libfilesync/protocol/memory/Handle.hpp>

namespace filesync::protocol::memory {

    template<typename ElementType>
    std::span<ElementType> Handle<ElementType>::data() {
        return doData();
    }

}