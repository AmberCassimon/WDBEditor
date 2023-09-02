# Include header files, link to Qt and set some properties
target_include_directories(WDBEditor PRIVATE include)

add_subdirectory(include/WDBEditor)
add_subdirectory(src/WDBEditor)