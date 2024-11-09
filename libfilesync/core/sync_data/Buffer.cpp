#include <libfilesync/core/sync_data/Buffer.hpp>

namespace filesync::core::sync_data {

    void Buffer::store() {
        doStore();
    }

    bool Buffer::compareTo() {
        return doCompareTo();
    }

}