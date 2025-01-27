#ifndef LIBFILESYNC_PROTOCOL_MEMORY_CURL_HANDLE_HPP
#define LIBFILESYNC_PROTOCOL_MEMORY_CURL_HANDLE_HPP

#include <libfilesync/protocol/memory/HandleTemplate.hpp>
#include <libfilesync/curl/storage/MemoryStorageHandle.hpp>

namespace filesync::protocol::memory {

    using CurlHandle = HandleTemplate<curl::storage::MemoryStorageHandle, char>; 

}

#endif