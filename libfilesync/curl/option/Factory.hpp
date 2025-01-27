#ifndef LIBFILESYNC_CURL_OPTION_FACTORY_HPP
#define LIBFILESYNC_CURL_OPTION_FACTORY_HPP

#include <libfilesync/curl/wrapper/Easy.hpp>
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
            explicit Factory(wrapper::Easy& interface);

            template <typename... Args>
            [[nodiscard]] std::unique_ptr<Option> createGeneric(CURLoption curlOption, Args&&... args) const;

            [[nodiscard]] std::unique_ptr<Collection> createCollection() const;

            [[nodiscard]] std::unique_ptr<Nobody> createNobody() const;
            [[nodiscard]] std::unique_ptr<Nobody> createVolatileNobody() const;

            [[nodiscard]] std::unique_ptr<Quote> createQuote() const;
            [[nodiscard]] std::unique_ptr<Quote> createQuote(const std::string& command) const;
            [[nodiscard]] std::unique_ptr<Quote> createVolatileQuote() const;
            [[nodiscard]] std::unique_ptr<Quote> createVolatileQuote(const std::string& command) const;

            [[nodiscard]] std::unique_ptr<Upload> createUpload(bool value) const;
            [[nodiscard]] std::unique_ptr<Upload> createVolatileUpload(bool value) const;

            [[nodiscard]] std::unique_ptr<Verbose> createVerbose() const;
            [[nodiscard]] std::unique_ptr<Verbose> createVolatileVerbose() const;

        private:
            wrapper::Easy& interface;

    };

}

#include <libfilesync/curl/option/Factory.tpp>

#endif