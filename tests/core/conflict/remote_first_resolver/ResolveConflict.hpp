#ifndef LIBFILESYNC_TESTS_CORE_CONFLICT_REMOTE_FIRST_RESOLVER_RESOLVE_CONFLICT_HPP
#define LIBFILESYNC_TESTS_CORE_CONFLICT_REMOTE_FIRST_RESOLVER_RESOLVE_CONFLICT_HPP

#include <tests/core/conflict/ResolveConflict.hpp>
#include <libfilesync/protocol/Client.hpp>

#include <string>
#include <memory>

namespace filesync::integration_test::core::conflict::remote_first_resolver {

    class ResolveConflict : public conflict::ResolveConflict {

        public:
            explicit ResolveConflict(const std::string& testName);
            ResolveConflict(
                const std::string& testName,
                std::unique_ptr<protocol::Client> protocolClient);

        private:
            void perform() override;
            void evaluate() override;

    };

}

#endif