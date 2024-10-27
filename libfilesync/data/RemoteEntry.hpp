#ifndef LIBFILESYNC_REMOTE_ENTRY_HPP
#define LIBFILESYNC_REMOTE_ENTRY_HPP

#include <string>
#include <filesystem>

namespace filesync {

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