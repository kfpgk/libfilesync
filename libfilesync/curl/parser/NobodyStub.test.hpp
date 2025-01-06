#ifndef LIBFILESYNC_CURL_PARSER_NOBODY_STUB_TEST_HPP
#define LIBFILESYNC_CURL_PARSER_NOBODY_STUB_TEST_HPP

#include <libfilesync/curl/parser/Nobody.hpp>

#include <string>

namespace filesync::curl::parser::unit_test {

    class NobodyStub : public Nobody {

        private:
            void doParse(const std::string& content) override;

    };

}

#endif