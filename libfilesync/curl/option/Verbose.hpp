#ifndef LIBFILESYNC_CURL_OPTION_VERBOSE_HPP
#define LIBFILESYNC_CURL_OPTION_VERBOSE_HPP

#include <libfilesync/curl/option/ResettableOption.hpp>
#include <libfilesync/curl/option/UndoableOptionImpl.hpp>
#include <libfilesync/curl/interface/Easy.hpp>

namespace filesync::curl::option {

    /**
     * @brief Wrapper for CURLOPT_VERBOSE
     * 
     * Patterns:
     *  - Command
     */
    class Verbose : public ResettableOption<bool>, public UndoableOptionImpl<bool> {

        public:
            /**
             * @brief Constructs a persistent option, which
             * is not being reset upon object destruction.
             */
            explicit Verbose(interface::Easy& curlEasy,
                bool value);

            /**
             * @brief Constructs a volatile option, which is
             * being reset to "resetValue" upon object destruction.
             */
            Verbose(interface::Easy& curlEasy,
                bool value,
                bool resetValue);

            ~Verbose();

        private:
            bool value = false;

            [[nodiscard]] bool getValue() override;
            void setTo(bool value) override;
    };

}

#endif