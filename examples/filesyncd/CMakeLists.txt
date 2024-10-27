add_executable(filesyncd
    main.cpp
)
target_link_libraries(filesyncd
    PRIVATE
        libfilesync
)