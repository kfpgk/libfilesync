target_sources(libfilesync 
    PRIVATE
        Logger.cpp
        TimeFormat.cpp
    PUBLIC FILE_SET public_headers TYPE HEADERS
    FILES
        Literals.hpp
    PRIVATE FILE_SET private_headers TYPE HEADERS
    FILES
        Debug.hpp
        Logger.hpp
        TimeFormat.hpp
)

configure_file(
    Literals.hpp ${PUBLIC_INCLUDE_DIR}/libfilesync/utility/Literals.hpp COPYONLY
)