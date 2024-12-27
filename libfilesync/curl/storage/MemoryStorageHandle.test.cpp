#include <libfilesync/curl/storage/MemoryStorageHandle.test.hpp>
#include <libfilesync/curl/storage/MemoryStorageHandle.hpp>
#include <libfilesync/curl/storage/MemoryStorage.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <cassert>
#include <memory>

using namespace filesync::utility;
using namespace filesync::curl::storage::unit_test;

int main(int argc, char* argv[]) {

    MemoryStorageHandleTest test;

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

    void MemoryStorageHandleTest::construct_from_friend() {

        std::unique_ptr<MemoryStorage> storage = std::make_unique<MemoryStorage>();

        MemoryStorageHandle handle(
            MemoryStorageHandle::ConstructorPermission{0},
            std::move(storage));

    }

    void MemoryStorageHandleTest::construct_from_friend_with_make_unique() {

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

        std::unique_ptr<MemoryStorage> storage = std::make_unique<MemoryStorage>();

#       if 0

        MemoryStorageHandle handle(
            {0},
            std::move(storage));

#       endif

    }

    /**
     * @brief Try to construct an object using std::make_unique
     * within a non friend object. 
     * 
     * This does not compile, hence the #if 0. Experimental test.
     */
    void MemoryStorageHandleClassNonFriend::construct_from_non_friend_with_make_unique() {

        std::unique_ptr<MemoryStorage> storage = std::make_unique<MemoryStorage>();

#       if 0

        std::unique_ptr<MemoryStorageHandle> handle = 
            std::make_unique<MemoryStorageHandle>(
                {0},
                std::move(storage));

#       endif

    }  

}