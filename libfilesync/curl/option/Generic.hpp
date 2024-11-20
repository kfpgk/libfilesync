#ifndef LIBFILESYNC_CURL_OPTION_GENERIC_HPP
#define LIBFILESYNC_CURL_OPTION_GENERIC_HPP

#include <libfilesync/curl/option/Option.hpp>
#include <libfilesync/curl/wrapper/Easy.hpp>

#include <curl/curl.h>

#include <tuple>
		
namespace filesync::curl::option {

    /**
     * @brief Generic CURLOPT class usable for every
     * CURLOPT option.
     * 
     * Use this if there is no specific CURLOPT class
     * available.
     */
    template<typename... Args> 
    class Generic : public Option {

        public:
            Generic(wrapper::Easy& curlEasy,
                CURLoption curlOption, Args&&... args);

        private:
            CURLoption curlOption;
            /**
             * @brief Storage for variadic template arguments
             * 
             * Decay the types, because we cannot store some
             * const literal types (e.g. const char[N]) in 
             * a member. 
             */
            std::tuple<std::decay_t<Args>...> args;

            void doSet() override;

    };

}

#include <libfilesync/curl/option/Generic.tpp>

#endif