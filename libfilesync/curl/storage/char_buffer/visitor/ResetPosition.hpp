#ifndef LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_VISITOR_RESET_POSITION_HPP
#define LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_VISITOR_RESET_POSITION_HPP

#include <libfilesync/curl/storage/char_buffer/ReadBuffer.hpp>
#include <libfilesync/curl/storage/char_buffer/ReadWriteBuffer.hpp>
#include <libfilesync/curl/storage/char_buffer/CharBufferStub.test.hpp>

namespace filesync::curl::storage::char_buffer::visitor {

    class ResetPosition {

        public:
            void operator()(ReadBuffer& buffer);
            void operator()(ReadWriteBuffer& buffer);
            void operator()(unit_test::CharBufferStub& buffer);

    };

}

#endif