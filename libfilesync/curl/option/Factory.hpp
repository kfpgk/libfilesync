#ifndef LIBFILESYNC_CURL_OPTION_FACTORY_HPP
#define LIBFILESYNC_CURL_OPTION_FACTORY_HPP

#include <libfilesync/curl/interface/Easy.hpp>
#include <libfilesync/curl/option/Option.hpp>
#include <libfilesync/curl/option/Collection.hpp>
#include <libfilesync/curl/option/Generic.hpp>
#include <libfilesync/curl/option/Nobody.hpp>
#include <libfilesync/curl/option/Upload.hpp>
#include <libfilesync/curl/option/Quote.hpp>
#include <libfilesync/curl/option/Verbose.hpp>

#include <curl/curl.h>

namespace filesync::curl::option {

    class Factory {

        public:
            explicit Factory(interface::Easy& curlEasy);

            template <typename... Args>
            std::unique_ptr<Option> createGeneric(CURLoption curlOption, Args&&... args) const;

            std::unique_ptr<Collection> createCollection() const;

            std::unique_ptr<Nobody> createNobody() const;
            std::unique_ptr<Nobody> createVolatileNobody() const;

            std::unique_ptr<Quote> createQuote() const;
            std::unique_ptr<Quote> createQuote(const std::string& command) const;
            std::unique_ptr<Quote> createVolatileQuote() const;
            std::unique_ptr<Quote> createVolatileQuote(const std::string& command) const;

            std::unique_ptr<Upload> createUpload(bool value) const;
            std::unique_ptr<Upload> createVolatileUpload(bool value) const;

            std::unique_ptr<Verbose> createVerbose() const;
            std::unique_ptr<Verbose> createVolatileVerbose() const;

        private:
            interface::Easy& curlEasy;

    };

}

#include <libfilesync/curl/option/Factory.tpp>

#endif