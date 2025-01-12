#include <libfilesync/curl/storage/visitor/GetReference.hpp>

namespace filesync::curl::storage::visitor {

    DataReference GetReference::operator()(const FileStorage& storage) {
        return storage.getPath();
    }

    DataReference GetReference::operator()(MemoryStorage& storage) {
        return storage.getDataReference();
    }

}