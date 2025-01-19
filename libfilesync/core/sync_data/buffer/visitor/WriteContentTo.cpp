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

    std::ostream& WriteContentTo::operator()(std::unique_ptr<CharArrayMemoryBuffer>& buffer) {
        return buffer->extractContentTo(out);
    }

    std::ostream& WriteContentTo::operator()(std::unique_ptr<FileBuffer>& buffer) {
        return buffer->extractContentTo(out);
    }

    std::ostream& WriteContentTo::operator()(std::unique_ptr<ProtocolMemoryBuffer>& buffer) {
        return buffer->extractContentTo(out);
    }

}