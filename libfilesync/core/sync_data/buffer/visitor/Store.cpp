#include <libfilesync/core/sync_data/buffer/visitor/Store.hpp>
#include <libfilesync/data/Exception.hpp>

namespace filesync::core::sync_data::buffer::visitor {

    Store::Store(std::istream& in) :
        in{in} {

        if (!in.good()) {
            throw data::Exception("Input stream is in fail state. Cannot store to buffer.",
            __FILE__, __LINE__);
        }
    }

    void Store::operator()(FileBuffer& buffer) {
        buffer.store(in);
    }

    void Store::operator()(MemoryBuffer& buffer) {
        buffer.store(in);
    }

}