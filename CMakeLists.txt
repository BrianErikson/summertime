cmake_minimum_required(VERSION 3.6)
project(summertime)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(PkgConfig REQUIRED)
pkg_search_module(CURL REQUIRED libcurl)
pkg_search_module(JSONCPP REQUIRED jsoncpp)
pkg_search_module(NOTIFY REQUIRED libnotify)

include_directories(summertime ${CURL_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS}
	${NOTIFY_INCLUDE_DIRS})

set(SOURCE_FILES summertime.hpp summertime.cpp)
set(SUMMERTIME_LIBRARIES ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES}
	${NOTIFY_LIBRARIES})

add_executable(summertime ${SOURCE_FILES})
install(TARGETS summertime RUNTIME DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/summertime.conf DESTINATION etc)

add_library(libsummertime SHARED ${SOURCE_FILES})
set_target_properties(libsummertime PROPERTIES OUTPUT_NAME summertime)

target_link_libraries(summertime ${SUMMERTIME_LIBRARIES})

add_executable(tests test.cpp)
add_dependencies(tests libsummertime)

target_link_libraries(tests ${SUMMERTIME_LIBRARIES} libsummertime)
