#ifndef LIBFILESYNC_TESTS_CORE_CONFLICT_RESOLVE_CONFLICT_HPP
#define LIBFILESYNC_TESTS_CORE_CONFLICT_RESOLVE_CONFLICT_HPP

#include <tests/template/IntegrationTest.hpp>
#include <libfilesync/protocol/Client.hpp>
#include <libfilesync/core/sync_data/Entry.hpp>

#include <string>
#include <memory>

namespace filesync::integration_test::core::conflict {

    class ResolveConflict : public IntegrationTest {

        public:
            explicit ResolveConflict(const std::string& testName);
            ResolveConflict(const std::string& testName,
                std::unique_ptr<protocol::Client> protocolClient);
            virtual ~ResolveConflict() = default;

        protected:
            std::unique_ptr<protocol::Client> protocolClient = nullptr;
            std::shared_ptr<filesync::core::sync_data::Entry> entry = nullptr;
            std::string localFileName = "testfile_local";
            std::string remoteFileName = "testfile_remote";
            std::string localFileContent = "Local file content";
            std::string remoteFileContent = "Remote file content";

        private:
            void setup() override;
            void perform() override = 0;
            void evaluate() override = 0;
            void cleanUp() override;

    };

}

#endif