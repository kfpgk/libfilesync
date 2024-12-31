#include <libfilesync/core/sync_data/buffer/visitor/Store.hpp>
#include <libfilesync/data/Exception.hpp>

namespace filesync::core::sync_data::buffer::visitor {

    Store::Store(BufferStoreType in) :
        in{std::move(in)} {

        if (std::holds_alternative<std::reference_wrapper<std::istream>>(in)) {
            if (!std::get<std::reference_wrapper<std::istream>>(in).get().good()) {
                throw data::Exception("Input stream is in fail state. Cannot store to buffer.",
                __FILE__, __LINE__);
            }
        }

    }

    void Store::operator()(CharArrayMemoryBuffer& buffer) {
        buffer.store(std::get<std::reference_wrapper<std::istream>>(in));
    }

    void Store::operator()(FileBuffer& buffer) {
        buffer.store(std::get<std::reference_wrapper<std::istream>>(in));
    }

    void Store::operator()(ProtocolMemoryBuffer& buffer) {
        buffer.store(std::move(std::get<std::unique_ptr<protocol::memory::Handle<char>>>(in)));
    }

}