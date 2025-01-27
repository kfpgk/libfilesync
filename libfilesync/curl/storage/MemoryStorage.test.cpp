#include <libfilesync/curl/storage/MemoryStorage.test.hpp>
#include <libfilesync/curl/storage/MemoryStorage.hpp>
#include <libfilesync/curl/storage/char_buffer/ReadWriteBuffer.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <cassert>
#include <string>
#include <variant>
#include <vector>

using namespace filesync::utility;

int main(int argc, char* argv[]) {

    filesync::curl::storage::unit_test::MemoryStorageTest test;

    test.test_swap_one_empty();
    test.test_swap_read_write_vs_read_only();
    test.copy_construction();
    test.move_construction();
    test.copy_assignment();
    test.move_assignment();

    Logger::getInstance().log(LogDomain::TestResult,
        "curl::storage::char_buffer::Owner: passed", __FILE__, __LINE__);
    return 0;
}

namespace filesync::curl::storage::unit_test {

    void MemoryStorageTest::test_swap_one_empty() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running test_swap_one_empty()", __FILE__, __LINE__);

        MemoryStorage storage1(100);

        std::vector<char> data{'t','e','s','t',' ','c','o','n','t','e','n','t'};
        MemoryStorage storage2(data);

        using std::swap;
        swap(storage1, storage2);

        assert(storage1.getDataReference().data() == data.data());
        assert(storage1.getDataReference().size() == data.size());
        assert(storage2.getDataReference().empty());                                                                                          }

    void MemoryStorageTest::test_swap_read_write_vs_read_only() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running test_swap_read_write_vs_read_only()", __FILE__, __LINE__);

        MemoryStorage storage1(100);
        std::string initialStorage1Data("Test Content storage1");
        memoryStorageWriteCallback(initialStorage1Data.data(), 1, initialStorage1Data.size(), 
            static_cast<void*>(&std::get<char_buffer::ReadWriteBuffer>(storage1.data)));

        std::vector<char> data{'t','e','s','t',' ','c','o','n','t','e','n','t'};
        MemoryStorage storage2(data);

        using std::swap;
        swap(storage1, storage2);

        std::string storage2Data(storage2.getDataReference().data(), storage2.getDataReference().size());

        assert(storage2Data == initialStorage1Data);
        assert(storage1.getDataReference().data() == data.data());
        assert(storage1.getDataReference().size() == data.size());
    }
    

    void MemoryStorageTest::copy_construction() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running copy_construction()", __FILE__, __LINE__);

        MemoryStorage storage1(100);
        std::string initialStorage1Data("Test Content storage1");
        memoryStorageWriteCallback(initialStorage1Data.data(), 1, initialStorage1Data.size(), 
            static_cast<void*>(&std::get<char_buffer::ReadWriteBuffer>(storage1.data)));

        MemoryStorage storage2(storage1);

        std::string storage1Data(storage1.getDataReference().data(), storage1.getDataReference().size());
        std::string storage2Data(storage2.getDataReference().data(), storage2.getDataReference().size());
        
        assert(storage1Data == initialStorage1Data);
        assert(storage2Data == initialStorage1Data);
    }

    void MemoryStorageTest::move_construction() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running move_construction()", __FILE__, __LINE__);
        
        MemoryStorage storage1(100);
        std::string initialStorage1Data("Test Content storage1");
        memoryStorageWriteCallback(initialStorage1Data.data(), 1, initialStorage1Data.size(), 
            static_cast<void*>(&std::get<char_buffer::ReadWriteBuffer>(storage1.data)));

        MemoryStorage storage2(std::move(storage1));

        std::string storage1Data(storage1.getDataReference().data(), storage1.getDataReference().size());
        std::string storage2Data(storage2.getDataReference().data(), storage2.getDataReference().size());

        assert(storage1Data.empty());
        assert(storage2Data == initialStorage1Data);
    }

    void MemoryStorageTest::copy_assignment() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running copy_assignment()", __FILE__, __LINE__);

        MemoryStorage storage1(100);
        std::string initialStorage1Data("Test Content storage1");
        memoryStorageWriteCallback(initialStorage1Data.data(), 1, initialStorage1Data.size(), 
            static_cast<void*>(&std::get<char_buffer::ReadWriteBuffer>(storage1.data)));

        MemoryStorage storage2(100);
        std::string initialStorage2Data("Test Content storage2");
        memoryStorageWriteCallback(initialStorage2Data.data(), 1, initialStorage2Data.size(), 
            static_cast<void*>(&std::get<char_buffer::ReadWriteBuffer>(storage2.data)));

        storage2 = storage1;

        std::string storage1Data(storage1.getDataReference().data(), storage1.getDataReference().size());
        std::string storage2Data(storage2.getDataReference().data(), storage2.getDataReference().size());
        
        assert(storage1Data == initialStorage1Data);
        assert(storage2Data == initialStorage1Data);
    }

    void MemoryStorageTest::move_assignment() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running move_assignment()", __FILE__, __LINE__);

        MemoryStorage storage1(100);
        std::string initialStorage1Data("Test Content storage1");
        memoryStorageWriteCallback(initialStorage1Data.data(), 1, initialStorage1Data.size(), 
            static_cast<void*>(&std::get<char_buffer::ReadWriteBuffer>(storage1.data)));

        MemoryStorage storage2(100);
        std::string initialStorage2Data("Test Content storage2");
        memoryStorageWriteCallback(initialStorage2Data.data(), 1, initialStorage2Data.size(), 
            static_cast<void*>(&std::get<char_buffer::ReadWriteBuffer>(storage2.data)));

        storage2 = std::move(storage1);

        std::string storage1Data(storage1.getDataReference().data(), storage1.getDataReference().size());
        std::string storage2Data(storage2.getDataReference().data(), storage2.getDataReference().size());

        assert(storage1Data.empty());
        assert(storage2Data == initialStorage1Data);
    }

}