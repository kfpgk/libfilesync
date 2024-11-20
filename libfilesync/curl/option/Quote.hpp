#ifndef LIBFILESYNC_CURL_OPTION_QUOTE_HPP
#define LIBFILESYNC_CURL_OPTION_QUOTE_HPP

#include <libfilesync/curl/option/ResettableOption.hpp>
#include <libfilesync/curl/option/UndoableOptionImpl.hpp>
#include <libfilesync/curl/wrapper/Easy.hpp>
#include <libfilesync/curl/wrapper/SList.hpp>

#include <string>

namespace filesync::curl::option {

    /**
     * @brief Wrapper for CURLOPT_QUOTE
     * 
     * Patterns:
     *  - Command
     */
    class Quote : public ResettableOption<wrapper::SList*>, public UndoableOptionImpl<wrapper::SList*> {

        public:

            /**
             * @brief Constructs a persistent option, which
             * is not being reset upon object destruction.
             */
            explicit Quote(wrapper::Easy& curlEasy,
                const std::string& command);

            /**
             * @brief Constructs a volatile option, which is
             * being reset to "resetValue" upon object destruction.
             */
            Quote(wrapper::Easy& curlEasy,               
                const std::string& command,
                wrapper::SList* resetValue);
                
            ~Quote();

            void addCommand(const std::string& command);

        private:
            wrapper::SList* commands;

            [[nodiscard]] wrapper::SList* getValue() override;
            void setTo(wrapper::SList* value) override;

    };

}

#endif