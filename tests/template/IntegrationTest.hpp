#ifndef LIBFILESYNC_INTEGRATION_TEST_HPP
#define LIBFILESYNC_INTEGRATION_TEST_HPP

#include <string>
#include <filesystem>
#include <vector>
#include <functional>

namespace filesync::integrationtest {

    /**
     * @brief A struct holding a testcase, which
     * consists of a perform() and evaluate()
     * function. 
     * 
     * Only needed if more than one
     * test case shall be included in an 
     * integration test object.
     */
    struct TestCase {
        std::string name;
        std::function<void(void)> perform;
        std::function<void(void)> evaluate;
    };

    /**
     * @brief Base class for integration tests
     * 
     * Provides a temporary sandbox for the 
     * derived class in setup() and deletes
     * the sandbox upon cleanUp() or destruction
     * of the object.
     * 
     * Patterns:
     *  - Template
     *  - Non virtual interface
     */
    class IntegrationTest {

        public:
            explicit IntegrationTest(const std::string& name);
            virtual ~IntegrationTest();
            IntegrationTest(const IntegrationTest&) = delete;
            IntegrationTest& operator==(const IntegrationTest& rhs) = delete;

            void setExitOnFail(bool value);
            void setKeepSandBox(bool value);
            void setKeepOldSandBox(bool value);
            void setSkipCleanUp(bool value);
            bool run();

        protected:
            void addTestCase(TestCase testCase);

        private:
            std::string name = "";
            bool exitOnFail = false;
            bool keepSandBox = false;
            bool keepOldSandBox = false;
            bool skipCleanUp = false;
            std::filesystem::path cwd;

            bool cleanUpCalled = false;

            std::vector<TestCase> testCases;

            void wrapSetup();
            void wrapPerform();
            void wrapPerform(const TestCase& testCase);
            void wrapEvaluate();
            void wrapEvaluate(const TestCase& testCase);
            void wrapCleanUp();

            virtual void setup();
            virtual void perform();
            virtual void evaluate();
            virtual void cleanUp();

            void setupSandBox(const std::string& directory);
            void cleanUpSandBox(const std::string& directory);
            void runTestCases();

    };

}

#endif