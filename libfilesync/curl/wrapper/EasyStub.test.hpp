#ifndef LIBFILESYNC_CURL_WRAPPER_UNIT_TEST_EASY_STUB_HPP
#define LIBFILESYNC_CURL_WRAPPER_UNIT_TEST_EASY_STUB_HPP

#include <libfilesync/curl/wrapper/Easy.hpp>

namespace filesync::curl::wrapper::unit_test {

    class EasyStub : public Easy {

        private:
            void perform() const override;

    };

}

#endif