#ifndef LIBFILESYNC_CURL_WRAPPER_BASE_HPP
#define LIBFILESYNC_CURL_WRAPPER_BASE_HPP

namespace filesync::curl::wrapper {

    /**
     * @brief Base class for all objects that
     * access libcurl directly.
     * 
     * Deriving from this ensures that global
     * initialization for libcurl is performed
     * if needed.
     */
    class Base {

        protected:
            Base();

    };

}

#endif