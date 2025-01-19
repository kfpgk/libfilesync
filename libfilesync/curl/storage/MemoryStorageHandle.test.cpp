#include <libfilesync/curl/storage/MemoryStorageHandle.test.hpp>
#include <libfilesync/curl/storage/MemoryStorage.hpp>
#include <libfilesync/curl/storage/char_buffer/CharBufferStub.test.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <cassert>

using namespace filesync::utility;
using namespace filesync::curl::storage::unit_test;

int main(int argc, char* argv[]) {

    MemoryStorageHandleTest test;

    test.test_swap();
    test.copy_construction();
    test.move_construction();
    test.copy_assignment();
    test.move_assignment();

    test.construct_from_friend();
    test.construct_from_friend_with_make_unique();

    MemoryStorageHandleClassNonFriend nonFriendTest;

    nonFriendTest.construct_from_non_friend();
    nonFriendTest.construct_from_non_friend_with_make_unique();

    Logger::getInstance().log(LogDomain::TestResult,
        "curl::storage::MemoryStorageHandle: passed", __FILE__, __LINE__);
    return 0;
}

namespace filesync::curl::storage::unit_test {

    void MemoryStorageHandleTest::test_swap() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running test_swap()", __FILE__, __LINE__);

        std::string data1("Data1");
        std::unique_ptr<MemoryStorageHandle> handle1 =
            constructHandle(data1);

        std::string data2("Data2");
        std::unique_ptr<MemoryStorageHandle> handle2 =
            constructHandle(data2);

        using std::swap;
        swap(*handle1, *handle2);

        std::string handle1Data(handle1->data().data());
        std::string handle2Data(handle2->data().data());
        assert(handle1Data == data2);
        assert(handle2Data == data1);
    }

    void MemoryStorageHandleTest::copy_construction() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running copy_construction()", __FILE__, __LINE__);

        std::string data1("Data1");
        std::unique_ptr<MemoryStorageHandle> handle1 =
            constructHandle(data1);

        MemoryStorageHandle handle2(*handle1);

        std::string handle1Data(handle1->data().data());
        std::string handle2Data(handle2.data().data());
        assert(handle1Data == data1);
        assert(handle2Data == data1);
    }

    void MemoryStorageHandleTest::move_construction() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running move_construction()", __FILE__, __LINE__);

        std::string data1("Data1");
        std::unique_ptr<MemoryStorageHandle> handle1 =
            constructHandle(data1);

        MemoryStorageHandle handle2(std::move(*handle1));

        std::string handle2Data(handle2.data().data());
        assert(!handle1->storage);
        assert(handle2Data == data1);
    }

    void MemoryStorageHandleTest::copy_assignment() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running copy_assignment()", __FILE__, __LINE__);

        std::string data1("Data1");
        std::unique_ptr<MemoryStorageHandle> handle1 =
            constructHandle(data1);

        std::string data2("Data2");
        std::unique_ptr<MemoryStorageHandle> handle2 =
            constructHandle(data2);

        *handle2 = *handle1;

        std::string handle1Data(handle1->data().data());
        std::string handle2Data(handle2->data().data());
        assert(handle1Data == data1);
        assert(handle2Data == data1);
    }

    void MemoryStorageHandleTest::move_assignment() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running move_assignment()", __FILE__, __LINE__);

        std::string data1("Data1");
        std::unique_ptr<MemoryStorageHandle> handle1 =
            constructHandle(data1);

        std::string data2("Data2");
        std::unique_ptr<MemoryStorageHandle> handle2 =
            constructHandle(data2);

        *handle2 = std::move(*handle1);

        std::string handle2Data(handle2->data().data());
        assert(!handle1->storage);
        assert(handle2Data == data1);
    }

    void MemoryStorageHandleTest::construct_from_friend() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running construct_from_friend()", __FILE__, __LINE__);

        std::unique_ptr<MemoryStorage> storage = std::make_unique<MemoryStorage>();

        MemoryStorageHandle handle(
            MemoryStorageHandle::ConstructorPermission{0},
            std::move(storage));

    }

    void MemoryStorageHandleTest::construct_from_friend_with_make_unique() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running construct_from_friend_with_make_unique()", __FILE__, __LINE__);

        std::unique_ptr<MemoryStorage> storage = std::make_unique<MemoryStorage>();

        std::unique_ptr<MemoryStorageHandle> handle = 
            std::make_unique<MemoryStorageHandle>(
                MemoryStorageHandle::ConstructorPermission{0},
                std::move(storage));

    }

    /**
     * @brief Try to construct an object within a non friend
     * object. 
     * 
     * This does not compile, hence the #if 0. Experimental
     * test.
     */
    void MemoryStorageHandleClassNonFriend::construct_from_non_friend() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running construct_from_non_friend()", __FILE__, __LINE__);

        std::unique_ptr<MemoryStorage> storage = std::make_unique<MemoryStorage>();

#       if 0

        MemoryStorageHandle handle(
            {0},
            std::move(storage));

#       endif

    }

    /**
     * @brief Try to construct an object using `std::make_unique`
     * within a non friend object. 
     * 
     * This does not compile, hence the #if 0. Experimental test.
     */
    void MemoryStorageHandleClassNonFriend::construct_from_non_friend_with_make_unique() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running construct_from_non_friend_with_make_unique()", __FILE__, __LINE__);

        std::unique_ptr<MemoryStorage> storage = std::make_unique<MemoryStorage>();

#       if 0

        std::unique_ptr<MemoryStorageHandle> handle = 
            std::make_unique<MemoryStorageHandle>(
                {0},
                std::move(storage));

#       endif

    }  

    std::unique_ptr<MemoryStorageHandle> MemoryStorageHandleTest::constructHandle(
        const std::string& data) {

        /**
         * We construct MemoryStorage with `new`, because constructor taking
         * `CharBufferStub` is private. `MemoryStorageHandleTest` is friend,
         * but `std::make_unique` is not.
         */
        return std::make_unique<MemoryStorageHandle>(
            MemoryStorageHandle::ConstructorPermission{0},
            std::unique_ptr<MemoryStorage>(
                new MemoryStorage(char_buffer::unit_test::CharBufferStub(data))));
    }

}