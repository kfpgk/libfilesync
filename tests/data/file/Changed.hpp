#ifndef LIBFILESYNC_TESTS_DATA_FILE_CHANGED_HPP
#define LIBFILESYNC_TESTS_DATA_FILE_CHANGED_HPP

#include <tests/template/IntegrationTest.hpp>
#include <libfilesync/data/File.hpp>

#include <string>
#include <memory>

namespace filesync::integrationtest::data::file {

    class Changed : public IntegrationTest {

        public:
            explicit Changed(const std::string& testName);

        private:
            std::unique_ptr<File> file;
            std::string fileName = "file1";

            void setup() override;
            void perform() override;

            void modifyFile();
            void evaluateFileChanged(bool expected);

    };

}

#endif