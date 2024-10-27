#ifndef LIBFILESYNC_CURL_OPTION_QUOTE_HPP
#define LIBFILESYNC_CURL_OPTION_QUOTE_HPP

#include <libfilesync/curl/option/ResettableOption.hpp>
#include <libfilesync/curl/option/UndoableOptionImpl.hpp>
#include <libfilesync/curl/interface/Easy.hpp>
#include <libfilesync/curl/SList.hpp>

#include <string>

namespace filesync::curl::option {

    /**
     * @brief Wrapper for CURLOPT_QUOTE
     * 
     * Patterns:
     *  - Command
     */
    class Quote : public ResettableOption<SList*>, public UndoableOptionImpl<SList*> {

        public:

            /**
             * @brief Constructs a persistent option, which
             * is not being reset upon object destruction.
             */
            explicit Quote(interface::Easy& curlEasy,
                const std::string& command);

            /**
             * @brief Constructs a volatile option, which is
             * being reset to "resetValue" upon object destruction.
             */
            Quote(interface::Easy& curlEasy,               
                const std::string& command,
                SList* resetValue);
                
            ~Quote();

            void addCommand(const std::string& command);

        private:
            SList* commands;

            [[nodiscard]] SList* getValue() override;
            void setTo(SList* value) override;

    };

}

#endif