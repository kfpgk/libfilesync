#include <tests/data/File/Changed.hpp>
#include <libfilesync/data/Data.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>

namespace filesync::integrationtest::data::file {

    Changed::Changed(const std::string& testName) :
        IntegrationTest(testName) {

    }

    void Changed::setup() {
        std::ofstream fileStream(fileName);
        if (!fileStream.is_open()) {
            throw FileSyncException("Cannot open local file for writing.",
                __FILE__, __LINE__);
        }
        fileStream << "Lorem ipsum dolor sit amet" << std::endl;
        file = std::make_unique<File>(fileName);
    }

    void Changed::perform() {
        evaluateFileChanged(false);
        modifyFile();
        evaluateFileChanged(true);
        evaluateFileChanged(true);
        file->resetChanged();
        evaluateFileChanged(false);
    }

    void Changed::modifyFile() {
        std::ofstream fileStream(fileName, std::ios_base::app);
        if (!fileStream.is_open()) {
            throw FileSyncException("Cannot open local file for writing.",
                __FILE__, __LINE__);
        }
        fileStream << "consetetur sadipscing elitr" << std::endl;
    }

    void Changed::evaluateFileChanged(bool expected) {
        if (file->hasChanged() != expected) {
            std::stringstream message;
            message << "File changed state did not match expected value '";
            message << std::boolalpha << expected << "'.";
            throw FileSyncException(message.str(), __FILE__, __LINE__);
        }
    }

}