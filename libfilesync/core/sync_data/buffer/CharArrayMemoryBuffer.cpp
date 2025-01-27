#include <libfilesync/core/sync_data/buffer/CharArrayMemoryBuffer.hpp>
#include <libfilesync/data/Exception.hpp>

namespace filesync::core::sync_data::buffer {

    std::span<char> CharArrayMemoryBuffer::getData() {
        return data;
    }

    std::unique_ptr<CharArrayMemoryBuffer> CharArrayMemoryBuffer::cloneType() const {
        return std::make_unique<CharArrayMemoryBuffer>();
    }

    void CharArrayMemoryBuffer::store(std::istream& in) {
        if (in.fail()) {
            throw data::Exception("Cannot store. Input stream is in fail state.",
                __FILE__, __LINE__);
        }
        data = {std::istreambuf_iterator<char>(in), {}};          
    }

    std::ostream& CharArrayMemoryBuffer::extractContentTo(std::ostream& out) {
        for (auto i : data) {
            out.put(i);
        }
        return out;
    }

    bool CharArrayMemoryBuffer::isEqualTo(std::istream& in) const {

        if (in.fail()) {
            return false;
        }

        if (getInStreamSize(in) != data.size()) {
            return false;
        }

        in.seekg(0, std::istream::beg);
        for (auto i : data) {
            if (i != in.get()) {
                return false;
            }
        }
        return true;
    }

    std::streampos CharArrayMemoryBuffer::getInStreamSize(std::istream& in) const {
        std::streampos inSize = 0;
        in.seekg(0, std::istream::beg);
        inSize = in.tellg();
        in.seekg( 0, std::ios::end );
        inSize = in.tellg() - inSize;
        return inSize;
    }
}