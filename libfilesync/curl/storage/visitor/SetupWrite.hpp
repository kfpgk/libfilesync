#ifndef LIBFILESYNC_CURL_STORAGE_VISITOR_SETUP_WRITE_HPP
#define LIBFILESYNC_CURL_STORAGE_VISITOR_SETUP_WRITE_HPP

#include <libfilesync/curl/storage/FileStorage.hpp>
#include <libfilesync/curl/storage/MemoryStorage.hpp>

#include <libfilesync/curl/option/Factory.hpp>

namespace filesync::curl::storage::visitor {

    /**
     * @brief Callable object to setup a write operation
     * to a storage.
     * 
     * Patterns:
     *  - Visitor
     */
    class SetupWrite {

        public:
            explicit SetupWrite(const option::Factory& optionFactory);

            void operator()(FileStorage& storage);
            void operator()(MemoryStorage& storage);

        private:
            const option::Factory& optionFactory;

    };

}

#endif