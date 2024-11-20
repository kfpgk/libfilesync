#include <libfilesync/curl/option/Factory.hpp>

#include <memory>

namespace filesync::curl::option {

    Factory::Factory(wrapper::Easy& curlEasy) :
        curlEasy{curlEasy} {

    }

    std::unique_ptr<Collection> Factory::createCollection() const {
        return std::make_unique<Collection>(curlEasy);
    }

    std::unique_ptr<Nobody> Factory::createNobody() const {
        return std::make_unique<Nobody>(curlEasy, true);
    }

    std::unique_ptr<Nobody> Factory::createVolatileNobody() const {
        return std::make_unique<Nobody>(curlEasy, true, false);
    }

    std::unique_ptr<Quote> Factory::createQuote() const {

        return std::make_unique<Quote>(curlEasy, "");
    }

    std::unique_ptr<Quote> Factory::createQuote(
        const std::string& command) const {

        return std::make_unique<Quote>(curlEasy, command);
    }

    std::unique_ptr<Quote> Factory::createVolatileQuote() const {

        return std::make_unique<Quote>(curlEasy, "", nullptr);
    }

    std::unique_ptr<Quote> Factory::createVolatileQuote(
        const std::string& command) const {

        return std::make_unique<Quote>(curlEasy, command, nullptr);
    }

    std::unique_ptr<Upload> Factory::createUpload(
        bool value) const {
        
        return std::make_unique<Upload>(curlEasy, value);
    }

    std::unique_ptr<Upload> Factory::createVolatileUpload(
        bool value) const {
        
        return std::make_unique<Upload>(curlEasy, value, false);
    }

    std::unique_ptr<Verbose> Factory::createVerbose() const {
        return std::make_unique<Verbose>(curlEasy, true);
    }

    std::unique_ptr<Verbose> Factory::createVolatileVerbose() const {
        return std::make_unique<Verbose>(curlEasy, true, false);
    }

}