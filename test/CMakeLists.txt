add_executable(Reconstruct ${CMAKE_CURRENT_SOURCE_DIR}/reconstruct.cpp)

target_link_libraries(Reconstruct PRIVATE WDB)