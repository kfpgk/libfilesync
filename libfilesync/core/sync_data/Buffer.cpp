#include <libfilesync/core/sync_data/Buffer.hpp>
#include <libfilesync/data/Exception.hpp>

namespace filesync::core::sync_data {

    void Buffer::store(std::istream& in) {
        if (!in.good()) {
            throw data::Exception("Input stream is in fail state. Cannot store to buffer.",
            __FILE__, __LINE__);
        }
        doStore(in);
    }

    bool Buffer::writeContentTo(std::ostream& out) {
        if (!out.good()) {
            throw data::Exception("Output stream is in fail state. Cannot write from buffer.",
            __FILE__, __LINE__);
        }
        return doWriteContentTo(out);
    }

    bool Buffer::isEqualTo(std::istream& in) {
        if (!in.good()) {
            throw data::Exception("Input stream is in fail state. Cannot compare to buffer.",
            __FILE__, __LINE__);
        }
        return doIsEqualTo(in);
    }

}