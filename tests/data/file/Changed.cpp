#include <tests/data/File/Changed.hpp>
#include <libfilesync/data/Data.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <chrono>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <thread>

namespace filesync::integration_test::data::file {

    Changed::Changed(const std::string& testName) :
        IntegrationTest(testName) {

        TestCase initialState{
            .name = "Tests initial changed() state of file",
            .perform = std::bind(&Changed::performInitialState, this),
            .evaluate = std::bind(&Changed::evaluateInitialState, this)
        };
        addTestCase(initialState);

        TestCase modifyFile{
            .name = "Tests changed() state after file modification",
            .perform = std::bind(&Changed::performModifyFile, this),
            .evaluate = std::bind(&Changed::evaluateModifyFile, this)
        };
        addTestCase(modifyFile);

        TestCase readChangedStateTwice{
            .name = "Tests changed() state when reading twice without",
            .perform = std::bind(&Changed::performReadChangedStateTwice, this),
            .evaluate = std::bind(&Changed::evaluateReadChangedStateTwice, this)
        };
        addTestCase(readChangedStateTwice);

        TestCase resetChanged{
            .name = "Tests resetting of changed() state",
            .perform = std::bind(&Changed::performResetChanged, this),
            .evaluate = std::bind(&Changed::evaluateResetChanged, this)
        };
        addTestCase(resetChanged);
    }

    void Changed::setup() {
        std::ofstream fileStream(fileName);
        if (!fileStream.is_open()) {
            throw FileSyncException("Cannot open local file for writing.",
                __FILE__, __LINE__);
        }
        fileStream << "Lorem ipsum dolor sit amet" << std::endl;
        fileStream.close();
        
        using namespace std::chrono;
        std::this_thread::sleep_for(10ms);

        file = std::make_unique<filesync::data::File>(fileName);
    }

    void Changed::performInitialState() {
        /* Do nothing */
    }

    void Changed::evaluateInitialState() {
        evaluateFileChanged(false);
    }

    void Changed::performModifyFile() {
        modifyFile(fileName);
    }

    void Changed::evaluateModifyFile() {
        evaluateFileChanged(true);
    }

    void Changed::performReadChangedStateTwice() {
        /* Do nothing */
    }

    void Changed::evaluateReadChangedStateTwice() {
        evaluateFileChanged(true);
    }

    void Changed::performResetChanged() {
        file->resetChanged();
    }

    void Changed::evaluateResetChanged() {
        evaluateFileChanged(false);
    }

    void Changed::modifyFile(const std::string& fileName) {
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