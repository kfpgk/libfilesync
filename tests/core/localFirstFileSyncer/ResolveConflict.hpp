#ifndef LIBFILESYNC_TESTS_CORE_LOCAL_FIRST_FILE_SYNCER_RESOLVE_CONFLICT_HPP
#define LIBFILESYNC_TESTS_CORE_LOCAL_FIRST_FILE_SYNCER_RESOLVE_CONFLICT_HPP

#include <tests/core/ResolveConflict.hpp>

#include <string>
#include <memory>

namespace filesync::integrationtest::core::local_first_file_syncer {

    class ResolveConflict : public core::ResolveConflict {

        public:
            explicit ResolveConflict(const std::string& testName);
            ResolveConflict(
                const std::string& testName,
                std::unique_ptr<ProtocolClient> protocolClient);

        private:
            void perform() override;
            void evaluate() override;

    };

}

#endif