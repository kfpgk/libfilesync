#ifndef LIBFILESYNC_CURL_OPTION_OPTION_HPP
#define LIBFILESYNC_CURL_OPTION_OPTION_HPP

#include <libfilesync/curl/interface/Easy.hpp>

#include <functional>

namespace filesync::curl::option {

    /**
     * @brief: Abstract wrapper for a CURLOPT
     * 
     * 
     * Patterns:
     *  - Command
     *  - Component & leaf of the composite pattern
     *  - Non virtual interface
     */
    class Option {

        public:
            Option(interface::Easy& curlEasy);
            virtual ~Option() = default;
            void setEasy(interface::Easy& curlEasy);

            void set();

        protected:
            std::reference_wrapper<interface::Easy> curlEasy;

        private:
            virtual void doSet() = 0;

    };

}

#endif