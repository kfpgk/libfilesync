#include <libfilesync/core/sync_data/buffer/visitor/WriteContentTo.hpp>
#include <libfilesync/data/Exception.hpp>

namespace filesync::core::sync_data::buffer::visitor {

    WriteContentTo::WriteContentTo(std::ostream& out) :
        out{out} {

        if (!out.good()) {
            throw data::Exception("Output stream is in fail state. Cannot write buffer content.",
            __FILE__, __LINE__);
        }
    }

    bool WriteContentTo::operator()(FileBuffer& buffer) {
        return buffer.extractContentTo(out);
    }

    bool WriteContentTo::operator()(MemoryBuffer& buffer) {
        return buffer.extractContentTo(out);
    }

}