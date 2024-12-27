#ifndef LIBFILESYNC_CURL_STORAGE_VISITOR_FLUSH_HPP
#define LIBFILESYNC_CURL_STORAGE_VISITOR_FLUSH_HPP

#include <libfilesync/curl/storage/FileStorage.hpp>
#include <libfilesync/curl/storage/MemoryStorage.hpp>

namespace filesync::curl::storage::visitor {

    /**
     * @brief Callable object to flush a storage.
     * 
     * Patterns:
     *  - Visitor
     */
    class Flush {
            void operator()(FileStorage& storage);
            void operator()(MemoryStorage& storage);

    };

}

#endif