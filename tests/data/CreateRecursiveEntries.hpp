#ifndef LIBFILESYNC_TESTS_DATA_CREATE_RECURSIVE_ENTRIES_HPP
#define LIBFILESYNC_TESTS_DATA_CREATE_RECURSIVE_ENTRIES_HPP

#include <tests/template/IntegrationTest.hpp>
#include <libfilesync/data/Entry.hpp>

#include <string>
#include <memory>

namespace filesync::integrationtest::data {

    class CreateRecursiveEntries : public IntegrationTest {

        public:
            CreateRecursiveEntries(const std::string& testName);

        private:
            std::unique_ptr<filesync::data::Entry> entry;

            void setup() override;
            void perform() override;
            void evaluate() override;

    };

}

#endif