#ifndef LIBFILESYNC_PROTOCOL_MEMORY_HANDLE_OR_FILE_PATH_HPP
#define LIBFILESYNC_PROTOCOL_MEMORY_HANDLE_OR_FILE_PATH_HPP

#include <libfilesync/protocol/memory/Handle.hpp>

#include <filesystem>
#include <functional>
#include <memory>
#include <variant>

namespace filesync::protocol {

    using HandleOrFilePath = std::variant<
        std::reference_wrapper<const std::filesystem::path>, 
        std::reference_wrapper<std::unique_ptr<memory::Handle<char>>>
    >;

}

#endif