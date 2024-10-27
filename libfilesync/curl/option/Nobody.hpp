#ifndef LIBFILESYNC_CURL_OPTION_NOBODY_HPP
#define LIBFILESYNC_CURL_OPTION_NOBODY_HPP

#include <libfilesync/curl/option/ResettableOption.hpp>
#include <libfilesync/curl/option/UndoableOptionImpl.hpp>
#include <libfilesync/curl/interface/Easy.hpp>

namespace filesync::curl::option {

    /**
     * @brief Wrapper for CURLOPT_NOBODY
     * 
     * The option will be reset upon 
     * destruction of the object unless
     * persistent flag is set.
     * 
     * Patterns:
     *  - Command
     */
    class Nobody : public ResettableOption<bool>, public UndoableOptionImpl<bool>  {

        public:
            /**
             * @brief Constructs a persistent option, which
             * is not being reset upon object destruction.
             */
            explicit Nobody(interface::Easy& curlEasy,
                bool value);

            /**
             * @brief Constructs a volatile option, which is
             * being reset to "resetValue" upon object destruction.
             */
            Nobody(interface::Easy& curlEasy,
                bool value,
                bool resetValue);
            ~Nobody();

        private:
            bool value = false;

            [[nodiscard]] bool getValue() override;
            void setTo(bool value) override;

    };

}

#endif