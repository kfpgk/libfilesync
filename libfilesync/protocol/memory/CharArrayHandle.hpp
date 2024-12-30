#ifndef LIBFILESYNC_PROTOCOL_MEMORY_CHAR_ARRAY_HANDLE_HPP
#define LIBFILESYNC_PROTOCOL_MEMORY_CHAR_ARRAY_HANDLE_HPP

#include <libfilesync/protocol/memory/HandleTemplate.hpp>
#include <libfilesync/protocol/memory/CharArray.hpp>

#include <vector>

namespace filesync::protocol::memory {

    using CharArrayHandle = HandleTemplate<CharArray, char>;
}

#endif