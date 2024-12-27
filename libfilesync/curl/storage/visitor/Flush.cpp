#include <libfilesync/curl/storage/visitor/Flush.hpp>

namespace filesync::curl::storage::visitor {

    void Flush::operator()(FileStorage& storage) {
        storage.flush();
    }

    void Flush::operator()(MemoryStorage& storage) {
        storage.flush();
    }

}