#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>
#include <libfilesync/data/Exception.hpp>

namespace filesync::core::sync_data::buffer {

    ProtocolMemoryBuffer::ProtocolMemoryBuffer(
        std::unique_ptr<protocol::memory::Handle<char>> data) :
        data{std::move(data)} {

    }

    ProtocolMemoryBuffer::ProtocolMemoryBuffer(const ProtocolMemoryBuffer& rhs) {
        if (rhs.data) {
            //data = std::make_unique<protocol::memory::Handle<char>>();
        }
    }

    ProtocolMemoryBuffer::ProtocolMemoryBuffer(ProtocolMemoryBuffer&& rhs) noexcept {
        using std::swap;
        swap(*this, rhs);
    }

    ProtocolMemoryBuffer& ProtocolMemoryBuffer::operator=(ProtocolMemoryBuffer rhs) {
        using std::swap;
        swap(*this, rhs);
        return *this;
    }

    void swap(ProtocolMemoryBuffer& lhs, ProtocolMemoryBuffer& rhs) noexcept {
        using std::swap;
        swap(lhs.data, rhs.data);
    }

    std::unique_ptr<ProtocolMemoryBuffer> ProtocolMemoryBuffer::cloneType() const {
        return std::make_unique<ProtocolMemoryBuffer>();
    }

    std::unique_ptr<protocol::memory::Handle<char>>& ProtocolMemoryBuffer::getHandle() {
        return data;
    }

    void ProtocolMemoryBuffer::store(std::unique_ptr<protocol::memory::Handle<char>> in) {
        data = std::move(in);
    }

    std::ostream& ProtocolMemoryBuffer::extractContentTo(std::ostream& out) {
        if (!data) {
            return out;
        }
        for (auto i : data->data()) {
            out.put(i);
        }
        return out;
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