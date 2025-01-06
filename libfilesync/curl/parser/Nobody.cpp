#include <libfilesync/curl/parser/Nobody.hpp>

#include <regex>

namespace filesync::curl::parser {

    void Nobody::parse(const std::string& content) {
        doParse(content);
    }

    std::size_t Nobody::getFileSize() const {
        return size;
    }

    std::string Nobody::trim(const std::string& input) {
        return std::regex_replace(input, std::regex("(^[ \n\r]+)|([ \n\r]+$)"),"");
    }

}