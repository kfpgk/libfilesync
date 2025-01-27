#include <libfilesync/data/Data.hpp>
#include <libfilesync/data/Exception.hpp>

#include <string>
#include <fstream>

namespace filesync::data {

    void createFile(const std::filesystem::path& path) {
        if (std::filesystem::exists(path)) {
            throw Exception("Cannot create file '" + path.string() \
                + "' since it already exists.");
        }
        std::ofstream newFile(path);
        if (!newFile.is_open()) {
            throw Exception("Failed to create file '" + path.string() + "'");
        }
    }

    bool haveEqualSize(
        const std::filesystem::path& rhs,
        const std::filesystem::path& lhs) {

        return (std::filesystem::file_size(rhs) == std::filesystem::file_size(lhs));
    }

    bool areEqual(
        const std::filesystem::path& lhs,
        const std::filesystem::path& rhs) {

        std::ifstream lhsStream(lhs.string(), std::ifstream::binary | std::ifstream::ate);
        std::ifstream rhsStream(rhs.string(), std::ifstream::binary | std::ifstream::ate);

        if (lhsStream.fail() || rhsStream.fail()) {
            return false;
        }

        if (!haveEqualSize(lhs, rhs)) {
            return false;
        }

        lhsStream.seekg(0, std::ifstream::beg);
        rhsStream.seekg(0, std::ifstream::beg);
        return std::equal(
            std::istreambuf_iterator<char>(lhsStream.rdbuf()),
            std::istreambuf_iterator<char>(),
            std::istreambuf_iterator<char>(rhsStream.rdbuf()));

    }

}