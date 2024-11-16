#ifndef LIBFILESYNC_CORE_SYNC_DATA_REMOTE_ENTRY_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_REMOTE_ENTRY_HPP

#include <string>
#include <filesystem>
#include <optional>
#include <cstddef>

namespace filesync::core::sync_data {

    class RemoteEntry {

        public:
            explicit RemoteEntry(std::string path);
            [[nodiscard]] std::string getPath() const;
            [[nodiscard]] std::string getLocation() const;
            [[nodiscard]] std::string getFileName() const;

        private:
            std::filesystem::path path;

    };

}

#endif