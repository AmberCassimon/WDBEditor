# Include header files, link to Qt and set some properties
target_include_directories(WDBEditor PRIVATE include)

string(TIMESTAMP BUILD_DATE "%Y-%m-%d" UTC)

target_compile_definitions(WDBEditor PRIVATE
    MAJOR_VERSION=${PROJECT_VERSION_MAJOR}
    MINOR_VERSION=${PROJECT_VERSION_MINOR}
    PATCH_VERSION=${PROJECT_VERSION_PATCH}
    BUILD_DATE="${BUILD_DATE}"
)

add_subdirectory(include/WDBEditor)
add_subdirectory(src/WDBEditor)