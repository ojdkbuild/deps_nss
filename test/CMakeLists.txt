
cmake_minimum_required ( VERSION 2.8.12 )

# project
include ( ${CMAKE_CURRENT_LIST_DIR}/../../../resources/cmake/ojdkbuild_common.cmake )
project ( ojdkbuild_nss_test CXX )

# dependencies
ojdkbuild_add_subdirectory ( ${CMAKE_CURRENT_LIST_DIR}/../../nss )
set ( ${PROJECT_NAME}_DEPS nss )
ojdkbuild_pkg_check_modules ( ${PROJECT_NAME}_DEPS_PC REQUIRED ${PROJECT_NAME}_DEPS )

# tests
enable_testing ( )
# nss_test
add_executable ( nss_test ${CMAKE_CURRENT_LIST_DIR}/nss_test.cpp )
target_include_directories ( nss_test BEFORE PRIVATE ${${PROJECT_NAME}_DEPS_PC_INCLUDE_DIRS} )
target_link_libraries ( nss_test ${${PROJECT_NAME}_DEPS_PC_LIBRARIES} )
target_compile_options ( nss_test PRIVATE ${${PROJECT_NAME}_DEPS_PC_CFLAGS_OTHER} )
set_target_properties ( nss_test PROPERTIES FOLDER "test" )
if ( DEFINED CMAKE_MEMORYCHECK_COMMAND )
    add_test ( nss_test 
            ${CMAKE_MEMORYCHECK_COMMAND} ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS} 
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/nss_test )
else ( )
    add_test ( nss_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/nss_test )
endif ( )
