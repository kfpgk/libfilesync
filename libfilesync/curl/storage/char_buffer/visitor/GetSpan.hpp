#ifndef LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_VISITOR_GET_SPAN_HPP
#define LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_VISITOR_GET_SPAN_HPP

#include <libfilesync/curl/storage/char_buffer/ReadBuffer.hpp>
#include <libfilesync/curl/storage/char_buffer/ReadWriteBuffer.hpp>

#include <span>

namespace filesync::curl::storage::char_buffer::visitor {

    class GetSpan {

        public:
            std::span<char> operator()(ReadBuffer& buffer);
            std::span<char> operator()(ReadWriteBuffer& buffer);

    };

}

#endif