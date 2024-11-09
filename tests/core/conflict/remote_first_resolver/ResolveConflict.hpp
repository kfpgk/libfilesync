#ifndef LIBFILESYNC_TESTS_CORE_CONFLICT_REMOTE_FIRST_RESOLVER_RESOLVE_CONFLICT_HPP
#define LIBFILESYNC_TESTS_CORE_CONFLICT_REMOTE_FIRST_RESOLVER_RESOLVE_CONFLICT_HPP

#include <tests/core/conflict/ResolveConflict.hpp>

#include <string>
#include <memory>

namespace filesync::integrationtest::core::conflict::remote_first_resolver {

    class ResolveConflict : public conflict::ResolveConflict {

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