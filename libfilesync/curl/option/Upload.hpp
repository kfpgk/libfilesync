#ifndef LIBFILESYNC_CURL_OPTION_UPLOAD_HPP
#define LIBFILESYNC_CURL_OPTION_UPLOAD_HPP

#include <libfilesync/curl/option/ResettableOption.hpp>
#include <libfilesync/curl/option/UndoableOptionImpl.hpp>
#include <libfilesync/curl/wrapper/Easy.hpp>

namespace filesync::curl::option {

    /**
     * @brief Wrapper for CURLOPT_UPLOAD
     * 
     * Patterns:
     *  - Command
     */
    class Upload : public ResettableOption<bool>, public UndoableOptionImpl<bool> {

        public:
            /**
             * @brief Constructs a persistent option, which
             * is not being reset upon object destruction.
             */
            Upload(wrapper::Easy& interface,
                bool value);

            /**
             * @brief Constructs a volatile option, which is
             * being reset to "resetValue" upon object destruction.
             */
            Upload(wrapper::Easy& interface,
                bool value,
                bool resetValue);

            ~Upload();

        private:
            bool value = false;

            [[nodiscard]] bool getValue() override;
            void setTo(bool value) override;

    };

}

#endif