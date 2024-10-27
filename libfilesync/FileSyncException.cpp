#include <libfilesync/FileSyncException.hpp>

#include <iostream>
#include <ranges>
#include <sstream>

namespace filesync {

    FileSyncException::FileSyncException(const std::string& errorMessage) :
        std::runtime_error{errorMessage},
        errorMessage{errorMessage} {

    }

    FileSyncException::FileSyncException(const std::string& errorMessage,
        const std::string& context) :
            std::runtime_error{errorMessage},
            errorMessage{errorMessage} {

        std::stringstream contextStream;
        contextStream << context;
        this->contexts.push_back(contextStream.str());
    }

    FileSyncException::FileSyncException(const std::string& errorMessage,
        const char *file,
        int line) :
            std::runtime_error(errorMessage),
            errorMessage{errorMessage} {

        std::stringstream contextStream;
        contextStream << file << ":" << line;
        this->contexts.push_back(contextStream.str());
    }

    const char* FileSyncException::what() const noexcept {
        completeErrorMessageBuffer = getCompleteErrorMessage();
        return completeErrorMessageBuffer.c_str();
    }

    std::string FileSyncException::getCompleteErrorMessage() const {
        std::stringstream message;
        std::string indent = "";
        std::ranges::reverse_view contextsReverse{contexts};
        message << "---" << std::endl;
        for(const auto& nextContext : contextsReverse) {
            message << indent << nextContext << std::endl;
            indent += " ";
        }
        message << errorMessage << std::endl;
        message << "---" << std::endl;  
        return message.str();
    }

    std::string FileSyncException::getErrorMessage() const {
        return errorMessage;
    }

    void FileSyncException::setErrorMessage(const std::string& message) {
        errorMessage = message;
    }

    void FileSyncException::appendToErrorMessage(const std::string& message) {
        errorMessage += message;
    }

    void FileSyncException::addContext(const std::string& context) {
        this->contexts.push_back(context);
    }

    void FileSyncException::addContext(const char *file, int line) {
        std::stringstream context;
        context << file << ":" << line;
        this->contexts.push_back(context.str());       
    }

}