#include <tests/template/IntegrationTest.hpp>
#include <tests/config/config.h>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <iostream>
#include <sstream>
#include <exception>

using namespace filesync::utility;

namespace filesync::integrationtest {

    IntegrationTest::IntegrationTest(const std::string& name) :
        name{name},
        cwd{std::filesystem::current_path()} {

    }

    IntegrationTest::~IntegrationTest() {
    }

    bool IntegrationTest::run() {
        Logger::getInstance().log(LogDomain::TestResult, 
            std::string("Running integration test: '" + name + "'"));

        try {
            wrapSetup();
            if (testCases.empty()) {
                wrapPerform();
                wrapEvaluate();
            } else {
                runTestCases();
            }
            wrapCleanUp();
        } catch (const std::exception& e) {
            std::cerr << e.what();
            Logger::getInstance().log(LogDomain::TestResult, 
                std::string("Integration test '" + name + "' failed."));
            wrapCleanUp();
            if (exitOnFail) {
                exit(-1);
            }
            return false;        
        }

        Logger::getInstance().log(LogDomain::Info, 
            "--- test finished ---");
        Logger::getInstance().log(LogDomain::TestResult, 
            std::string("Integration test for '" + name + "' passed."));
        return true;
    }

    void IntegrationTest::setExitOnFail(bool value) {
        this->exitOnFail = value;
    }

    void IntegrationTest::setKeepSandBox(bool value) {
        this->keepSandBox = value;
    }

    void IntegrationTest::setKeepOldSandBox(bool value) {
        this->keepOldSandBox = value;
    }

    void IntegrationTest::setSkipCleanUp(bool value) {
        this->skipCleanUp = value;
    }

    void IntegrationTest::addTestCase(TestCase testCase) {

        Logger::getInstance().log(LogDomain::Info,
            "Adding testcase.");
        testCases.push_back(testCase);
    }

    void IntegrationTest::runTestCases() {
        int noOfTestCases = testCases.size();
        int count = 0;
        Logger::getInstance().log(LogDomain::Info,
            "Running " + std::to_string(noOfTestCases) + " testcase(s).");
        for (auto&& it = testCases.begin(); it != testCases.end(); it++) {
            count++;
            std::stringstream message;
            message << "--- Running testcase " << count << "/" << noOfTestCases;
            Logger::getInstance().log(LogDomain::Info, message.str());
            wrapPerform(*it);
            wrapEvaluate(*it);
            std::stringstream resultMessage;
            resultMessage << "Testcase '" << it->name << "' passed.";
            Logger::getInstance().log(LogDomain::TestResult, resultMessage.str());
        }
    }

    void IntegrationTest::wrapSetup() {
        Logger::getInstance().log(LogDomain::Info, 
            "--- setup ---");
        setupSandBox(INTEGRATION_SANDBOX_DIR);
        setup();
    }

    void IntegrationTest::wrapPerform() {
        Logger::getInstance().log(LogDomain::Info, 
            "--- perform ---");
        perform();
    }

    void IntegrationTest::wrapPerform(const TestCase& testCase) {
        Logger::getInstance().log(LogDomain::Info, 
            "--- perform test case '" + testCase.name + "' ---");
        testCase.perform();
    }

    void IntegrationTest::wrapEvaluate() {
        Logger::getInstance().log(LogDomain::Info, 
            "--- evaluate ---");
        evaluate();
    }

    void IntegrationTest::wrapEvaluate(const TestCase& testCase) {
        Logger::getInstance().log(LogDomain::Info, 
            "--- evaluate test case '" + testCase.name + "' ---");
        testCase.evaluate();
    }

    void IntegrationTest::wrapCleanUp() {
        if (!cleanUpCalled) {
            cleanUpCalled = true;
            Logger::getInstance().log(LogDomain::Info, 
                "--- clean up ---");        
            if (skipCleanUp) {
                Logger::getInstance().log(LogDomain::Info, 
                    " Skip clean up");
                std::filesystem::current_path(cwd);
            } else {
                cleanUp();
                std::filesystem::current_path(cwd);
                if (!keepSandBox) {
                    cleanUpSandBox(INTEGRATION_SANDBOX_DIR);
                }
            }
        }
    }

    void IntegrationTest::setupSandBox(const std::string& directory) {
        Logger::getInstance().log(LogDomain::Debug, 
            "Creating integration test sandbox '"
            + std::filesystem::absolute(directory).string()
            + "'");
        this->cwd = std::filesystem::current_path();
        if (!keepOldSandBox) {
            std::filesystem::remove_all(directory);
        }
        if (!std::filesystem::exists(directory)) {
            if (!std::filesystem::create_directory(directory)) {
                throw FileSyncException("Wasn't able to create directory '"
                    + directory + "'.");
            }
        }     
        std::filesystem::current_path(directory);
    }

    void IntegrationTest::cleanUpSandBox(const std::string& directory) {
        Logger::getInstance().log(LogDomain::Debug, 
            "Cleaning up integration test sandbox directory '"
            + directory + "'.");
        std::filesystem::remove_all(directory);
    }

    void IntegrationTest::setup() {
        Logger::getInstance().log(LogDomain::Debug, 
            "Custom setup() not implemented");
    }

    void IntegrationTest::perform() {
        Logger::getInstance().log(LogDomain::Debug, 
            "Custom perform() not implemented");
    }

    void IntegrationTest::evaluate() {
        Logger::getInstance().log(LogDomain::Debug, 
            "Custom evaluate() not implemented");
    }

    void IntegrationTest::cleanUp() {
        Logger::getInstance().log(LogDomain::Debug, 
            "Custom cleanUp() not implemented");
    }

}