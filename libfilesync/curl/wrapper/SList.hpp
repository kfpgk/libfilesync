#ifndef LIBFILESYNC_CURL_WRAPPER_SLIST_HPP
#define LIBFILESYNC_CURL_WRAPPER_SLIST_HPP

#include <libfilesync/curl/wrapper/Base.hpp>

#include <curl/curl.h>

#include <string>
#include <vector>

namespace filesync::curl::wrapper {

    /**
     * @brief Wrapper for curl_slist
     */
    class SList : public Base {

        public:
            SList() = default;
            SList(const std::string& content);
            ~SList();
            SList(const SList&);
            SList(SList&& rhs);
            SList& operator=(SList);
            friend void swap(SList& lhs, SList& rhs);

            [[nodiscard]] bool isEmpty() const;
            [[nodiscard]] curl_slist* getSListPointer() const;

            void append(const std::string& content);

        private:
            curl_slist* handle = NULL;
            std::vector<std::string> contents;

    };

}

#endif