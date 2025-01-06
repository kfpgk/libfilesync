#include <libfilesync/curl/parser/NobodyStub.test.hpp>

namespace filesync::curl::parser::unit_test {

    void NobodyStub::doParse(const std::string& content) {
        size = 17;
    }

}