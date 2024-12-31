#include <libfilesync/core/sync_data/buffer/visitor/IsEqualTo.hpp>
#include <libfilesync/data/Exception.hpp>

namespace filesync::core::sync_data::buffer::visitor {

    IsEqualTo::IsEqualTo(std::istream& in) :
        in{in} {

        if (!in.good()) {
            throw data::Exception("Input stream is in fail state. Cannot use for comparison.",
            __FILE__, __LINE__);
        }
    }

    bool IsEqualTo::operator()(const CharArrayMemoryBuffer& buffer) {
        return buffer.isEqualTo(in);
    }

    bool IsEqualTo::operator()(const FileBuffer& buffer) {
        return buffer.isEqualTo(in);
    }

    bool IsEqualTo::operator()(const ProtocolMemoryBuffer& buffer) {
        return buffer.isEqualTo(in);
    }

}