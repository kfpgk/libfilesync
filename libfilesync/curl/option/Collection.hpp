#ifndef LIBFILESYNC_CURL_OPTION_COLLECTION_HPP
#define LIBFILESYNC_CURL_OPTION_COLLECTION_HPP

#include <libfilesync/curl/option/Option.hpp>
#include <libfilesync/curl/wrapper/Easy.hpp>

#include <memory>
#include <deque>

namespace filesync::curl::option {

    /**
     * @brief Collection of CURLOPTs
     * 
     * Patterns:
     *  - Composite of the composite pattern
     */
    class Collection : public Option {

        public:
            explicit Collection(wrapper::Easy& curlEasy);
            void add(std::shared_ptr<Option> option);
            void clear();

        private:
            std::deque<std::shared_ptr<Option>> options;

            virtual void doSet();

    };

}

#endif