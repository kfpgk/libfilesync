#ifndef LIBFILESYNC_DATA_DATA_HPP
#define LIBFILESYNC_DATA_DATA_HPP

#include <filesystem>
#include <memory>

namespace filesync::data {

    enum class EntryType { none, file, dir };

    void createFile(const std::filesystem::path& path);

    bool haveEqualSize(
        const std::filesystem::path& lhs,
        const std::filesystem::path& rhs);

    bool areEqual(
        const std::filesystem::path& lhs,
        const std::filesystem::path& rhs);

}

#endif