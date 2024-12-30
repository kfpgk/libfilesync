#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>
#include <libfilesync/data/Exception.hpp>

namespace filesync::core::sync_data::buffer {

    ProtocolMemoryBuffer::ProtocolMemoryBuffer(std::unique_ptr<protocol::memory::Handle<char>> data) :
        data{std::move(data)} {

    }

    std::unique_ptr<protocol::memory::Handle<char>> ProtocolMemoryBuffer::getHandle() {
        if (!data) {
            throw data::Exception("Cannot retrieve handle, because does not own a handle",
                __FILE__, __LINE__);
        }
        return std::move(data);
    }

    void ProtocolMemoryBuffer::store(std::unique_ptr<protocol::memory::Handle<char>> in) {
        data = std::move(in);
    }

    bool ProtocolMemoryBuffer::extractContentTo(std::ostream& out) {
        if (!data) {
            return false;
        }
        for (auto i : data->data()) {
            out.put(i);
        }
        return true;
    }

    bool ProtocolMemoryBuffer::isEqualTo(std::istream& in) const {

        if (!data || in.fail()) {
            return false;
        }

        if (getInStreamSize(in) != data->data().size()) {
            return false;
        }

        in.seekg(0, std::istream::beg);
        for (auto i : data->data()) {
            if (i != in.get()) {
                return false;
            }
        }
        return true;
    }

    std::streampos ProtocolMemoryBuffer::getInStreamSize(std::istream& in) const {
        std::streampos inSize = 0;
        in.seekg(0, std::istream::beg);
        inSize = in.tellg();
        in.seekg( 0, std::ios::end );
        inSize = in.tellg() - inSize;
        return inSize;
    }
}