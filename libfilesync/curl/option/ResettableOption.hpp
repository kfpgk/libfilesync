#ifndef LIBFILESYNC_CURL_OPTION_RESETTABLE_OPTION_HPP
#define LIBFILESYNC_CURL_OPTION_RESETTABLE_OPTION_HPP

#include <libfilesync/curl/option/Option.hpp>

#include <optional>

namespace filesync::curl::option {

    /**
     * @brief CURLOPT that provides an interface
     * for resetting the option.
     */
    template<typename T> 
    class ResettableOption : virtual public Option {

        public:
            ResettableOption() = default;
            explicit ResettableOption(const T& resetValue);
            virtual ~ResettableOption() = default;

            void reset();

        private:
            std::optional<T> resetValue;

            virtual void setTo(T value) = 0;

    };

}

#include <libfilesync/curl/option/ResettableOption.tpp>

#endif