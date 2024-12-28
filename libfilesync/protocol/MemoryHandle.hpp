#ifndef LIBFILESYNC_PROTOCOL_MEMORY_HANDLE_HPP
#define LIBFILESYNC_PROTOCOL_MEMORY_HANDLE_HPP

#include <memory>
#include <span>

namespace filesync::protocol {

    template<typename ElementType>
    class MemoryHandle {

        public:
            std::span<ElementType> data() const {
                return doData();
            }

        private:
            virtual std::span<ElementType> doData() const = 0;

    };

    template <typename ContainterType, typename ElementType>
    concept data_extractable_as_span = requires(ContainterType container, ElementType element) {
        { container.data() } -> std::convertible_to<std::span<ElementType>>;
    };

    template<typename ContainterType, typename ElementType>
        requires data_extractable_as_span<ContainterType, ElementType>
    class MemoryHandleT : public MemoryHandle<ElementType> {

        public:
            MemoryHandleT(std::unique_ptr<ContainterType> dataPtr);
            std::span<ElementType> doData() const override;

        private:
            std::unique_ptr<ContainterType> dataPtr;

    };

}

#include <libfilesync/protocol/MemoryHandle.tpp>

#endif