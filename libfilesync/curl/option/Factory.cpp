#include <libfilesync/curl/option/Factory.hpp>

#include <memory>

namespace filesync::curl::option {

    Factory::Factory(wrapper::Easy& interface) :
        interface{interface} {

    }

    std::unique_ptr<Collection> Factory::createCollection() const {
        return std::make_unique<Collection>(interface);
    }

    std::unique_ptr<Nobody> Factory::createNobody() const {
        return std::make_unique<Nobody>(interface, true);
    }

    std::unique_ptr<Nobody> Factory::createVolatileNobody() const {
        return std::make_unique<Nobody>(interface, true, false);
    }

    std::unique_ptr<Quote> Factory::createQuote() const {

        return std::make_unique<Quote>(interface, "");
    }

    std::unique_ptr<Quote> Factory::createQuote(
        const std::string& command) const {

        return std::make_unique<Quote>(interface, command);
    }

    std::unique_ptr<Quote> Factory::createVolatileQuote() const {

        return std::make_unique<Quote>(interface, "", nullptr);
    }

    std::unique_ptr<Quote> Factory::createVolatileQuote(
        const std::string& command) const {

        return std::make_unique<Quote>(interface, command, nullptr);
    }

    std::unique_ptr<Upload> Factory::createUpload(
        bool value) const {
        
        return std::make_unique<Upload>(interface, value);
    }

    std::unique_ptr<Upload> Factory::createVolatileUpload(
        bool value) const {
        
        return std::make_unique<Upload>(interface, value, false);
    }

    std::unique_ptr<Verbose> Factory::createVerbose() const {
        return std::make_unique<Verbose>(interface, true);
    }

    std::unique_ptr<Verbose> Factory::createVolatileVerbose() const {
        return std::make_unique<Verbose>(interface, true, false);
    }

}