#ifndef LIBFILESYNC_CURL_STORAGE_VISITOR_GET_REFERENCE_HPP
#define LIBFILESYNC_CURL_STORAGE_VISITOR_GET_REFERENCE_HPP

#include <libfilesync/curl/storage/Storage.hpp>
#include <libfilesync/curl/storage/FileStorage.hpp>
#include <libfilesync/curl/storage/MemoryStorage.hpp>

namespace filesync::curl::storage::visitor {

    /**
     * @brief Callable object to get reference to a storage's
     * data.
     * 
     * Patterns:
     *  - Visitor
     */
    class GetReference {

        public:
            [[nodiscard]] DataReference operator()(const FileStorage& storage);
            [[nodiscard]] DataReference operator()(MemoryStorage& storage);

    };

}

#endif