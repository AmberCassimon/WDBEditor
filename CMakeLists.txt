cmake_minimum_required(VERSION 3.25)
project(WDBEditor VERSION 1.3.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Necessary for QT to work
# Has to do with the Q_OBJECT macro and the generation of moc_* files
# See: https://stackoverflow.com/questions/14170770/unresolved-external-symbol-public-virtual-struct-qmetaobject-const-thiscal#comment70225454_15913504
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt5, we just need the widgets
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Set up our executable
add_executable(WDBEditor)

# And our library
add_library(WDB)

target_link_libraries(WDBEditor PRIVATE WDB Qt5::Widgets)

add_subdirectory(app)
add_subdirectory(lib)
