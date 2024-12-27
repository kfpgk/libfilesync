#ifndef LIBFILESYNC_CURL_STORAGE_STORAGE_HPP
#define LIBFILESYNC_CURL_STORAGE_STORAGE_HPP

#include <libfilesync/curl/storage/FileStorage.hpp>
#include <libfilesync/curl/storage/MemoryStorage.hpp>

#include <filesystem>
#include <span>
#include <variant>

namespace filesync::curl::storage {

    using Storage = std::variant<FileStorage, MemoryStorage>;

    using DataReference = std::variant<std::filesystem::path, std::span<char>>;

}

#endif