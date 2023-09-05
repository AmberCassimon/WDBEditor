# Required for fopen_s to work
target_compile_definitions(WDB PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:__STDC_WANT_LIB_EXT1__=1$>
)

target_include_directories(WDB PUBLIC include)

add_subdirectory(include/libWDB)
add_subdirectory(src/libWDB)
