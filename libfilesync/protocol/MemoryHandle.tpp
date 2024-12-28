#include <libfilesync/protocol/MemoryHandle.hpp>

namespace filesync::protocol {

    template<typename ContainterType, typename ElementType>
    MemoryHandleT<ContainterType, ElementType>::MemoryHandleT(std::unique_ptr<ContainterType> dataPtr) :
        dataPtr{std::move(dataPtr)} {

    }

    template<typename ContainterType, typename ElementType>
    std::span<ElementType> MemoryHandleT<ContainterType,ElementType>::doData() const {
        return dataPtr->data();
    }

}