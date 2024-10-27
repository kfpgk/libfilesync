#ifndef LIBFILESYNC_TESTS_CORE_FILE_SYNCER_HPP
#define LIBFILESYNC_TESTS_CORE_FILE_SYNCER_HPP

#include <tests/template/IntegrationTest.hpp>
#include <libfilesync/core/FileSyncer.hpp>
#include <libfilesync/protocol/ProtocolClient.hpp>
#include <libfilesync/data/Entry.hpp>

#include <string>
#include <memory>

namespace filesync::integrationtest::core {

    class ResolveConflict : public IntegrationTest {

        public:
            explicit ResolveConflict(const std::string& testName);
            ResolveConflict(const std::string& testName,
                std::unique_ptr<ProtocolClient> protocolClient);
            virtual ~ResolveConflict() = default;

        protected:
            std::unique_ptr<ProtocolClient> protocolClient = nullptr;
            std::shared_ptr<Entry> entry = nullptr;
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