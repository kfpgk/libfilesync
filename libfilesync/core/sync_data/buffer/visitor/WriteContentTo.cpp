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

    std::ostream& WriteContentTo::operator()(CharArrayMemoryBuffer& buffer) {
        return buffer.extractContentTo(out);
    }

    std::ostream& WriteContentTo::operator()(FileBuffer& buffer) {
        return buffer.extractContentTo(out);
    }

    std::ostream& WriteContentTo::operator()(ProtocolMemoryBuffer& buffer) {
        return buffer.extractContentTo(out);
    }

}