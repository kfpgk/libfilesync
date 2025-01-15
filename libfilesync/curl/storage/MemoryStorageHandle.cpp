#include <libfilesync/curl/storage/MemoryStorageHandle.hpp>
#include <libfilesync/curl/Exception.hpp>

namespace filesync::curl::storage {

    std::span<char> MemoryStorageHandle::data() {
        return storage->getDataReference();
    }

    MemoryStorageHandle::MemoryStorageHandle(
        [[maybe_unused]] ConstructorPermission permission,
        std::unique_ptr<MemoryStorage> storage) :
        
        storage{std::move(storage)} {

    }

    /*MemoryStorageHandle::MemoryStorageHandle(const MemoryStorageHandle& rhs) {
        if (rhs.storage) {
            //std::make_unique<MemoryStorage>(rhs.storage->getDataReference());
        }
    }*/

    /*MemoryStorageHandle& MemoryStorageHandle::operator=(const MemoryStorageHandle& rhs) {
        return *this;
    }*/

    std::unique_ptr<MemoryStorage> MemoryStorageHandle::extract() {
        if (!storage) {
            throw Exception("Trying to extract empty handle", __FILE__, __LINE__);
        }
        return std::move(storage);
    }

}