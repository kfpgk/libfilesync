add_subdirectory(conflict)
add_subdirectory(sync_data)

if(BUILD_UNIT_TESTS)
    enable_testing()
endif()

target_sources(libfilesync 
    PRIVATE
        FileSyncer.cpp
        BufferedSyncer.cpp
        OneWaySyncer.cpp
        UnbufferedSyncer.cpp
)