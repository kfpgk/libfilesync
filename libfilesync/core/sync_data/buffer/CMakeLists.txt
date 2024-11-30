add_subdirectory(visitor)

target_sources(libfilesync
    PRIVATE
        FileBuffer.cpp
        MemoryBuffer.cpp
)