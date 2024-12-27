#include <libfilesync/curl/storage/MemoryStorageHandle.hpp>
#include <libfilesync/curl/Exception.hpp>

namespace filesync::curl::storage {

    MemoryStorageHandle::MemoryStorageHandle(
        [[maybe_unused]] ConstructorPermission permission,
        std::unique_ptr<MemoryStorage> storage) :
        
        storage{std::move(storage)} {

    }

    std::unique_ptr<MemoryStorage> MemoryStorageHandle::extract() {
        if (!storage) {
            throw Exception("Trying to extract empty handle", __FILE__, __LINE__);
        }
        return std::move(storage);
    }

}