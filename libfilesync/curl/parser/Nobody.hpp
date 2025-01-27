#ifndef LIBFILESYNC_CURL_PARSER_NOBODY_HPP
#define LIBFILESYNC_CURL_PARSER_NOBODY_HPP

#include <cstddef>
#include <string>

namespace filesync::curl::parser {

    /**
     * @brief curl CURL_OPT_NOBODY output parser
     * interface.
     * 
     * Patterns:
     * 
     *  - Non-virtual interface
     */
    class Nobody {

        public:
            void parse(const std::string& content);
            [[nodiscard]] std::size_t getFileSize() const;

        protected:
            std::size_t size = 0;

            std::string trim(const std::string& input);

        private:
            virtual void doParse(const std::string& content) = 0;

    };

}

#endif