#ifndef LIBFILESYNC_CURL_INTERFACE_UNIT_TEST_EASY_STUB_HPP
#define LIBFILESYNC_CURL_INTERFACE_UNIT_TEST_EASY_STUB_HPP

#include <libfilesync/curl/interface/Easy.hpp>

namespace filesync::curl::interface::unittest {

    class EasyStub : public Easy {

        private:
            void run() const override;

    };

}

#endif