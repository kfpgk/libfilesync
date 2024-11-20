#ifndef LIBFILESYNC_CURL_OPTION_OPTION_HPP
#define LIBFILESYNC_CURL_OPTION_OPTION_HPP

#include <libfilesync/curl/wrapper/Easy.hpp>

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
            Option(wrapper::Easy& interface);
            virtual ~Option() = default;
            void setEasy(wrapper::Easy& interface);

            void set();

        protected:
            std::reference_wrapper<wrapper::Easy> interface;

        private:
            virtual void doSet() = 0;

    };

}

#endif