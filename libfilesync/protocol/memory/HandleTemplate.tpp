#include <libfilesync/protocol/memory/HandleTemplate.hpp>

namespace filesync::protocol::memory {

    template<typename ContainterType, typename ElementType>
    HandleTemplate<ContainterType, ElementType>::HandleTemplate(std::unique_ptr<ContainterType> dataPtr) :
        dataPtr{std::move(dataPtr)} {

    }

    template<typename ContainterType, typename ElementType>
    std::span<ElementType> HandleTemplate<ContainterType,ElementType>::doData() const {
        return dataPtr->data();
    }

}