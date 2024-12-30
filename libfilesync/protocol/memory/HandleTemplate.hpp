#ifndef LIBFILESYNC_PROTOCOL_MEMORY_HANDLE_TEMPLATE_HPP
#define LIBFILESYNC_PROTOCOL_MEMORY_HANDLE_TEMPLATE_HPP

#include <libfilesync/protocol/memory/Handle.hpp>

#include <memory>
#include <span>

namespace filesync::protocol::memory {

    template <typename ContainterType, typename ElementType>
    concept data_extractable_as_span = requires(ContainterType container, ElementType element) {
        { container.data() } -> std::convertible_to<std::span<const ElementType>>;
    };

    template<typename ContainterType, typename ElementType>
        requires data_extractable_as_span<ContainterType, ElementType>
    class HandleTemplate : public Handle<ElementType> {

        public:
            HandleTemplate(std::unique_ptr<ContainterType> dataPtr);
            std::span<ElementType> doData() override;

        private:
            std::unique_ptr<ContainterType> dataPtr;

    };

}

#include <libfilesync/protocol/memory/HandleTemplate.tpp>

#endif