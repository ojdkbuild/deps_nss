# Copyright 2016, akashche at redhat.com
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required ( VERSION 2.8.12 )

# project
include ( ${CMAKE_CURRENT_LIST_DIR}/../../resources/cmake/ojdkbuild_common.cmake )
project ( ojdkbuild_nss C CXX )
set ( ${PROJECT_NAME}_OJDKBUILD_VERSION 3.19.1-0 )

# dependencies
ojdkbuild_add_subdirectory ( ${CMAKE_CURRENT_LIST_DIR}/../zlib )

# static libraries
add_subdirectory ( ${CMAKE_CURRENT_LIST_DIR}/resources/nspr_4108_cmake ${CMAKE_CURRENT_BINARY_DIR}/lookaside_nspr )
add_subdirectory ( ${CMAKE_CURRENT_LIST_DIR}/resources/nss_3191_cmake ${CMAKE_CURRENT_BINARY_DIR}/lookaside_nss )

set ( ${PROJECT_NAME}_DEPS zlib )
ojdkbuild_pkg_check_modules ( ${PROJECT_NAME}_DEPS_PC REQUIRED ${PROJECT_NAME}_DEPS )

# shared_library
add_library ( ${PROJECT_NAME} SHARED ${CMAKE_CURRENT_LIST_DIR}/resources/${PROJECT_NAME}.def )
set_target_properties ( ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C )
target_link_libraries ( ${PROJECT_NAME} 
        lookaside_nss
        lookaside_nss_softokn
        lookaside_nss_util
        lookaside_nspr 
        ${${PROJECT_NAME}_DEPS_PC_LIBRARIES}
        ws2_32
        winmm )
        
# public headers
set ( ${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/install/include )
file ( MAKE_DIRECTORY ${${PROJECT_NAME}_INCLUDE_DIR} )
set ( ${PROJECT_NAME}_NSPR_DIR ${CMAKE_CURRENT_LIST_DIR}/../../lookaside/nspr )
set ( ${PROJECT_NAME}_NSS_DIR ${CMAKE_CURRENT_LIST_DIR}/../../lookaside/nss )
set ( ${PROJECT_NAME}_NSS_UTIL_DIR ${CMAKE_CURRENT_LIST_DIR}/../../lookaside/nss-util )
file ( COPY 
        ${${PROJECT_NAME}_NSS_DIR}/lib/freebl/alghmac.h
        ${${PROJECT_NAME}_NSS_DIR}/lib/freebl/blapi.h
        ${${PROJECT_NAME}_NSS_DIR}/lib/freebl/blapit.h
        ${${PROJECT_NAME}_NSS_DIR}/lib/freebl/ecl/ecl-exp.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/hasht.h
        ${${PROJECT_NAME}_NSS_DIR}/lib/cryptohi/keythi.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/nssilckt.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/nssilock.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/nssrwlkt.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/pkcs11.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/pkcs11n.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/pkcs11p.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/pkcs11t.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/pkcs11u.h
        ${${PROJECT_NAME}_NSPR_DIR}/lib/ds/plarena.h
        ${${PROJECT_NAME}_NSPR_DIR}/lib/ds/plarenas.h
        ${${PROJECT_NAME}_NSPR_DIR}/lib/ds/plhash.h
        ${${PROJECT_NAME}_NSPR_DIR}/lib/libc/include/plstr.h
        ${${PROJECT_NAME}_NSPR_DIR}/pr/include/prclist.h
        ${CMAKE_CURRENT_BINARY_DIR}/lookaside_nspr/include/prcpucfg.h
        ${${PROJECT_NAME}_NSPR_DIR}/pr/include/prcvar.h
        ${${PROJECT_NAME}_NSPR_DIR}/pr/include/prinrval.h
        ${${PROJECT_NAME}_NSPR_DIR}/pr/include/prlink.h
        ${${PROJECT_NAME}_NSPR_DIR}/pr/include/prlock.h
        ${${PROJECT_NAME}_NSPR_DIR}/pr/include/prlog.h
        ${${PROJECT_NAME}_NSPR_DIR}/pr/include/prmon.h
        ${CMAKE_CURRENT_BINARY_DIR}/lookaside_nspr/include/prtypes.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/secasn1.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/secasn1t.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/seccomon.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/secerr.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/secitem.h
        ${${PROJECT_NAME}_NSS_DIR}/lib/pk11wrap/secmodt.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/secoid.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/secoidt.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/secport.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/utilmodt.h
        ${${PROJECT_NAME}_NSS_UTIL_DIR}/lib/util/utilrename.h
        DESTINATION ${${PROJECT_NAME}_INCLUDE_DIR} )
file ( MAKE_DIRECTORY ${${PROJECT_NAME}_INCLUDE_DIR}/obsolete )  
file ( COPY 
        ${${PROJECT_NAME}_NSPR_DIR}/pr/include/obsolete/protypes.h
        DESTINATION ${${PROJECT_NAME}_INCLUDE_DIR}/obsolete )

# pkg-config
set ( ${PROJECT_NAME}_MODULE nss )
set ( ${PROJECT_NAME}_BUNDLE_LIB_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX} )
set ( ${PROJECT_NAME}_PC_REQUIRES_PRIVATE zlib )
set ( ${PROJECT_NAME}_PC_CFLAGS "-I${CMAKE_CURRENT_BINARY_DIR}/install/include" )
set ( ${PROJECT_NAME}_PC_LIBS "-L${CMAKE_LIBRARY_OUTPUT_DIRECTORY} -l${PROJECT_NAME}" )
configure_file ( ${CMAKE_CURRENT_LIST_DIR}/../../resources/cmake/pkg-config.in 
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/nss.pc )