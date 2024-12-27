#ifndef LIBFILESYNC_CURL_STORAGE_MEMORY_STORAGE_HANDLE_HPP
#define LIBFILESYNC_CURL_STORAGE_MEMORY_STORAGE_HANDLE_HPP

#include <libfilesync/curl/storage/MemoryStorage.hpp>

#include <memory>

namespace filesync::curl {
    class ProtocolClient;
}

namespace filesync::curl::storage {

    namespace unit_test {
        class MemoryStorageHandleTest;
    }

    class MemoryStorageHandle {

        private:
            /**
             * @brief Object to block constructor calls
             * to MemoryStorageHandle from non-friend 
             * classes.
             */
            struct ConstructorPermission {
                /**
                 * @brief Constructor for permission object
                 * 
                 * This object must not be default constructable.
                 * If this was the case, a non friend member could
                 * call MemoryStorageHandle constructor using default
                 * construction '{}' for ConstructorPermission Object:
                 *  - MemoryStorageHandle({}, ...)
                 */
                explicit ConstructorPermission([[maybe_unused]] int i) {};
            };

        public:
            /**
             * @brief Constructor that should only be callable
             * from friend classes (equivalent to private).
             * 
             * We need this to be able to construct object using
             * std::make_unique.
             */
            MemoryStorageHandle(ConstructorPermission token,
                std::unique_ptr<MemoryStorage> storage);

        private:
            std::unique_ptr<MemoryStorage> storage;

            std::unique_ptr<MemoryStorage> extract();

        friend class filesync::curl::ProtocolClient;

        friend class unit_test::MemoryStorageHandleTest;

    };

}

#endif