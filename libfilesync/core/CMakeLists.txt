add_subdirectory(conflict)
add_subdirectory(sync_data)

target_sources(libfilesync 
    PRIVATE
        FileSyncer.cpp
        BufferedSyncer.cpp
        OneWaySyncer.cpp
        UnbufferedSyncer.cpp
)