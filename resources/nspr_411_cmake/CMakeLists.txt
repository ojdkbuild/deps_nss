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

# this file is based on https://github.com/staticlibs/mirror_freetype2/blob/master/CMakeLists.txt
cmake_minimum_required ( VERSION 2.8.12 )
project ( lookaside_nspr C )
set ( ${PROJECT_NAME}_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../../lookaside/nspr )

# standalone build
if ( NOT DEFINED STATICLIB_TOOLCHAIN )
    if ( NOT DEFINED ${PROJECT_NAME}_TOOLCHAIN )
        if ( WIN32 )
            if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
                set ( ${PROJECT_NAME}_TOOLCHAIN "windows_amd64_msvc" )
            else ( )
                set ( ${PROJECT_NAME}_TOOLCHAIN "windows_i386_msvc" )
            endif ( )
        elseif ( UNIX )
            set ( ${PROJECT_NAME}_TOOLCHAIN "linux_amd64_gcc" )
        else ( )
            message ( FATAL_ERROR "Cannot determine toolchain to use, please specify [${PROJECT_NAME}_TOOLCHAIN]" )
        endif ( )
    endif ( )
else ( )
    set ( ${PROJECT_NAME}_TOOLCHAIN ${STATICLIB_TOOLCHAIN} )
endif ( )

# headers
if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "linux_[^_]+_[^_]+" )
    set ( ${PROJECT_NAME}_PRCPUCFG_FILENAME _linux.cfg )
elseif ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_[^_]+_[^_]+" )
    set ( ${PROJECT_NAME}_PRCPUCFG_FILENAME _win95.cfg )
endif ( )
file (COPY ${${PROJECT_NAME}_SOURCE_DIR}/pr/include/md/${${PROJECT_NAME}_PRCPUCFG_FILENAME}
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ )
file ( RENAME ${CMAKE_CURRENT_BINARY_DIR}/include/${${PROJECT_NAME}_PRCPUCFG_FILENAME}
        ${CMAKE_CURRENT_BINARY_DIR}/include/prcpucfg.h )
file (COPY ${CMAKE_CURRENT_LIST_DIR}/../nspr_include/_pr_bld.h
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ )


# library
set ( ${PROJECT_NAME}_SRC )

# io
set ( ${PROJECT_NAME}_IO_SRC 
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prfdcach.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prmwait.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/priometh.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/pripv6.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prmapopt.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prlayer.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prlog.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prmmap.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prpolevt.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prprf.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prscanf.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prstdio.c )
source_group ( "src_io" FILES ${${PROJECT_NAME}_IO_SRC} )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_IO_SRC} )

# linking
set ( ${PROJECT_NAME}_LINKING_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/linking/prlink.c )
source_group ( "src_linking" FILES ${${PROJECT_NAME}_LINKING_SRC} )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_LINKING_SRC} )

# malloc
set ( ${PROJECT_NAME}_MALLOC_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/malloc/prmalloc.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/malloc/prmem.c )
source_group ( "src_malloc" FILES ${${PROJECT_NAME}_MALLOC_SRC} )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_MALLOC_SRC} )

# md
set ( ${PROJECT_NAME}_MD_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/prosdep.c )
source_group ( "src_md" FILES ${${PROJECT_NAME}_MD_SRC} )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_MD_SRC} )

# memory
set ( ${PROJECT_NAME}_MEMORY_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/memory/prseg.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/memory/prshma.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/memory/prshm.c )
source_group ( "src_memory" FILES ${${PROJECT_NAME}_MEMORY_SRC} )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_MEMORY_SRC} )

# misc
set ( ${PROJECT_NAME}_MISC_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/pralarm.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/pratom.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/praton.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prcountr.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prdtoa.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prenv.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prerr.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prerror.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prerrortable.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prinit.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prinrval.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/pripc.c        
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prlog2.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prlong.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prnetdb.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prolock.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prrng.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prsystem.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prthinfo.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prtime.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prtpool.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/prtrace.c )
    if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_[^_]+_[^_]+" )
        list ( APPEND ${PROJECT_NAME}_MISC_SRC
                ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/misc/pripcsem.c )
    endif ( )
source_group ( "src_misc" FILES ${${PROJECT_NAME}_MISC_SRC} )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_MISC_SRC} )

# threads
set ( ${PROJECT_NAME}_THREADS_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/threads/prcmon.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/threads/prrwlock.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/threads/prtpd.c )
if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_[^_]+_[^_]+" )
    list ( APPEND ${PROJECT_NAME}_THREADS_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/threads/prdump.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/threads/prmon.c
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/threads/prsem.c        
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/threads/prcthr.c )
endif ( )
source_group ( "src_threads" FILES ${${PROJECT_NAME}_THREADS_SRC} )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_THREADS_SRC} )

# version
set ( ${PROJECT_NAME}_VERSION_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/prvrsion.c )
source_group ( "src_version" FILES ${${PROJECT_NAME}_VERSION_SRC} )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_VERSION_SRC} )

# lib/ds
set ( ${PROJECT_NAME}_LIB_DS_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ds/plarena.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ds/plhash.c )
source_group ( "src_lib_ds" FILES ${${PROJECT_NAME}_LIB_DS_SRC} )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_LIB_DS_SRC} )

# lib/libc
set ( ${PROJECT_NAME}_LIB_LIBC_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/src/strlen.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/src/strcpy.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/src/strdup.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/src/strcase.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/src/strcat.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/src/strcmp.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/src/strchr.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/src/strpbrk.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/src/strstr.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/src/strtok.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/src/base64.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/src/plerror.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/src/plgetopt.c )
source_group ( "src_lib_libc" FILES ${${PROJECT_NAME}_LIB_LIBC_SRC} ) 
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_LIB_LIBC_SRC} )

if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "linux_[^_]+_[^_]+" )
    # md/unix
    set ( ${PROJECT_NAME}_MD_UNIX_SRC
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/unix/unix.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/unix/unix_errors.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/unix/uxproces.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/unix/uxrng.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/unix/uxshm.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/unix/uxwrap.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/unix/linux.c )
    source_group ( "src_md_unix" FILES ${${PROJECT_NAME}_MD_UNIX_SRC} ) 
    list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_MD_UNIX_SRC} )

    # pthreads
    set ( ${PROJECT_NAME}_PTHREADS_SRC
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/pthreads/ptio.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/pthreads/ptsynch.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/pthreads/ptthread.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/pthreads/ptmisc.c )
    source_group ( "src_pthreads" FILES ${${PROJECT_NAME}_PTHREADS_SRC} ) 
    list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_PTHREADS_SRC} )

endif ( )

if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_[^_]+_[^_]+" )

    # windows
    set ( ${PROJECT_NAME}_WINDOWS_SRC
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prdir.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prfile.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prio.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/io/prsocket.c )
    source_group ( "src_windows" FILES ${${PROJECT_NAME}_WINDOWS_SRC} )
    list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_WINDOWS_SRC} )

    # md/windows
    set ( ${PROJECT_NAME}_MD_WINDOWS_SRC                  
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/ntmisc.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/ntsec.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/ntsem.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/ntinrval.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/ntmisc.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/ntgc.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/w95thred.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/w95io.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/w95cv.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/w32rng.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/w95sock.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/win32_errors.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/w32ipcsem.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/w32poll.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/w32shm.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/windows/w95dllmain.c )
    source_group ( "src_md_windows" FILES ${${PROJECT_NAME}_MD_WINDOWS_SRC} )  
    list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_MD_WINDOWS_SRC} ) 
    
    # threads/combined
    set ( ${PROJECT_NAME}_THREADS_COMBINED_SRC
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/threads/combined/prucpu.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/threads/combined/prucv.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/threads/combined/prulock.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/threads/combined/pruthr.c
            ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/threads/combined/prustack.c )
    source_group ( "src_threads_combined" FILES ${${PROJECT_NAME}_THREADS_COMBINED_SRC} )
    list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_THREADS_COMBINED_SRC} )
    
endif ( )

set ( ${PROJECT_NAME}_INCLUDES 
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/include
        ${${PROJECT_NAME}_SOURCE_DIR}/pr/include/private
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ds
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/include )

set ( ${PROJECT_NAME}_HEADERS ) 
file ( GLOB ${PROJECT_NAME}_RESOURCES_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/include/*.h )
set ( ${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_RESOURCES_HEADERS} )
source_group ( "include_resources" FILES ${${PROJECT_NAME}_RESOURCES_HEADERS} )
file ( GLOB ${PROJECT_NAME}_PR_HEADERS ${${PROJECT_NAME}_SOURCE_DIR}/pr/include/*.h )
set ( ${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_PR_HEADERS} )
source_group ( "include_pr" FILES ${${PROJECT_NAME}_PR_HEADERS} )
file ( GLOB ${PROJECT_NAME}_PR_PRIVATE_HEADERS ${${PROJECT_NAME}_SOURCE_DIR}/pr/include/private/*.h )
set ( ${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_PR_PRIVATE_HEADERS} )
source_group ( "include_pr_private" FILES ${${PROJECT_NAME}_PR_PRIVATE_HEADERS} ) 
file ( GLOB ${PROJECT_NAME}_LIB_DS_HEADERS ${${PROJECT_NAME}_SOURCE_DIR}/lib/ds/*.h )
set ( ${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_LIB_DS_HEADERS} )
source_group ( "include_lib_ds" FILES ${${PROJECT_NAME}_LIB_DS_HEADERS} )
file ( GLOB ${PROJECT_NAME}_LIB_LIBC_HEADERS ${${PROJECT_NAME}_SOURCE_DIR}/lib/libc/include/*.h )
set ( ${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_LIB_LIBC_HEADERS} )
source_group ( "include_lib_libc" FILES ${${PROJECT_NAME}_LIB_LIBC_HEADERS} )

# linux
if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "linux_[^_]+_[^_]+" )
    set ( ${PROJECT_NAME}_OPTS -UHAVE_CVAR_BUILT_ON_SEM )
    set ( ${PROJECT_NAME}_DEFS
            -DLINUX=1
            -DHAVE_VISIBILITY_HIDDEN_ATTRIBUTE=1
            -DHAVE_VISIBILITY_PRAGMA=1
            -DXP_UNIX=1
            -D_GNU_SOURCE=1
            -DHAVE_FCNTL_FILE_LOCKING=1        
            -D_REENTRANT=1 
            -DHAVE_DLADDR=1
            -DHAVE_LCHOWN=1
            -DHAVE_SETPRIORITY=1
            -DHAVE_STRERROR=1
            -DHAVE_SYSCALL=1
            -DHAVE___SECURE_GETENV=1
            -DFORCE_PR_LOG
            -D_PR_PTHREADS )
endif ( )
if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "linux_amd64_[^_]+" )
    set ( ${PROJECT_NAME}_DEFS ${${PROJECT_NAME}_DEFS} -Dx86_64=1 )
    set ( ${PROJECT_NAME}_SRC_ASM ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/unix/os_Linux_x86_64.s )
    set_property ( SOURCE ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/unix/os_Linux_x86_64.s PROPERTY LANGUAGE C )
endif ( )
if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "linux_i386_[^_]+" )
    set ( ${PROJECT_NAME}_DEFS ${${PROJECT_NAME}_DEFS} -Di386=1 )
    set ( ${PROJECT_NAME}_SRC_ASM ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/unix/os_Linux_x86.s )
    set_property ( SOURCE ${${PROJECT_NAME}_SOURCE_DIR}/pr/src/md/unix/os_Linux_x86.s PROPERTY LANGUAGE C )
endif ( )
if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "linux_arm[^_]+_[^_]+" )
    set ( ${PROJECT_NAME}_DEFS ${${PROJECT_NAME}_DEFS} -DARM=1 )
endif ( )

# windows
if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_[^_]+_[^_]+" )
    set ( ${PROJECT_NAME}_OPTS 
            -UWINNT            
            /wd4244
            /wd4267 
            /wd4554 )
    set ( ${PROJECT_NAME}_DEFS           
            -DXP_PC=1 
            -DWIN95=1 
            -D_PR_GLOBAL_THREADS_ONLY=1
            -D_CRT_SECURE_NO_WARNINGS=1 
            -D_CRT_NONSTDC_NO_WARNINGS=1 
            -DWIN32=1              
            -DFORCE_PR_LOG 
            -D_NSPR_BUILD_ )
endif ( )
if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_i386_[^_]+" )
    set ( ${PROJECT_NAME}_DEFS ${${PROJECT_NAME}_DEFS} -D_X86_=1 ) 
endif ( )
if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_amd64_[^_]+" )
    set ( ${PROJECT_NAME}_DEFS ${${PROJECT_NAME}_DEFS} -D_AMD64_=1 ) 
endif ( )       

# library
add_library ( ${PROJECT_NAME} STATIC 
        ${${PROJECT_NAME}_SRC} 
        ${${PROJECT_NAME}_SRC_ASM}
        ${${PROJECT_NAME}_HEADERS} )
target_include_directories ( ${PROJECT_NAME} BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} ) 
target_compile_options ( ${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_OPTS} )
target_compile_definitions ( ${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_DEFS} )

# build order
add_dependencies ( ${PROJECT_NAME} ojdkbuild_zlib )
