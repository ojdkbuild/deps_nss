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
project ( lookaside_nss C ASM_MASM )
set ( ${PROJECT_NAME}_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../../lookaside/nss )
set ( ${PROJECT_NAME}_UTIL_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../../lookaside/nss-util )

# standalone build
if ( NOT DEFINED STATICLIB_TOOLCHAIN )
    if ( NOT DEFINED ${PROJECT_NAME}_TOOLCHAIN )
        if ( WIN32 )
            if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
                set ( ${PROJECT_NAME}_TOOLCHAIN "windows_amd64_msvc" )
            else ( )
                set ( ${PROJECT_NAME}_TOOLCHAIN "windows_i386_msvc" )
            endif ( )
        else ( )
            message ( FATAL_ERROR "Cannot determine toolchain to use, please specify [${PROJECT_NAME}_TOOLCHAIN]" )
        endif ( )
    endif ( )
else ( )
    set ( ${PROJECT_NAME}_TOOLCHAIN ${STATICLIB_TOOLCHAIN} )
endif ( )

# dependencies
set ( ${PROJECT_NAME}_DEPS zlib )
ojdkbuild_pkg_check_modules ( ${PROJECT_NAME}_DEPS_PC REQUIRED ${PROJECT_NAME}_DEPS )

# internal dep, not exposed to pkg-config
set ( ${PROJECT_NAME}_NSPR_INCLUDES 
        ${PROJECT_BINARY_DIR}/../lookaside_nspr/include
        ${${PROJECT_NAME}_SOURCE_DIR}/../nspr/include
        ${${PROJECT_NAME}_SOURCE_DIR}/../nspr/pr/include
        ${${PROJECT_NAME}_SOURCE_DIR}/../nspr/lib/ds
        ${${PROJECT_NAME}_SOURCE_DIR}/../nspr/lib/libc/include )

# util
set ( ${PROJECT_NAME}_UTIL_OPTS
        /wd4244
        /wd4267
        /wd4312 )
set ( ${PROJECT_NAME}_UTIL_DEFS
        -D_CRT_SECURE_NO_WARNINGS
        -D_CRT_NONSTDC_NO_WARNINGS
        -DXP_PC
        -DWIN32
        -UDEBUG
        -DNDEBUG
        -D_WINDOWS
        -DWIN95
        -DNSS_NO_INIT_SUPPORT
        -DUSE_UTIL_DIRECTLY
        -DNO_NSPR_10_SUPPORT
        -DSSL_DISABLE_DEPRECATED_CIPHER_SUITE_NAMES )
set ( ${PROJECT_NAME}_UTIL_SRC
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/quickder.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/secdig.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/derdec.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/derenc.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/dersubr.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/dertime.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/errstrs.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/nssb64d.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/nssb64e.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/nssrwlk.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/nssilock.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/oidstring
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/pkcs1sig.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/portreg.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/secalgid.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/secasn1d.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/secasn1e.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/secasn1u.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/secitem.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/secload.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/secoid.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/sectime.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/secport.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/templates
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/utf8.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/utilmod.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/utilpars.c
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/pkcs11uri.c )
source_group ( "src" FILES ${${PROJECT_NAME}_UTIL_SRC} )
set ( ${PROJECT_NAME}_UTIL_INCLUDES 
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util )
file ( GLOB ${PROJECT_NAME}_UTIL_HEADERS ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util/*.h )
source_group ( "include" FILES ${${PROJECT_NAME}_UTIL_HEADERS} )
        
# softokn
set ( ${PROJECT_NAME}_SOFTOKN_SRC )
set ( ${PROJECT_NAME}_SOFTOKN_OPTS
        /wd4005
        /wd4013
        /wd4018
        /wd4244
        /wd4267 )
set ( ${PROJECT_NAME}_SOFTOKN_DEFS
        -D_CRT_SECURE_NO_WARNINGS
        -D_CRT_NONSTDC_NO_WARNINGS
        -DXP_PC
        -DSHLIB_SUFFIX=\"dll\"
        -DSHLIB_PREFIX=\"\"
        -DSHLIB_VERSION=\"3\"
        -DSOFTOKEN_SHLIB_VERSION=\"3\"
        -DSOFTOKEN_LIB_NAME=\"softokn3.dll\"
        -DRIJNDAEL_INCLUDE_TABLES
        -DWIN32
        -UDEBUG
        -DNDEBUG
        -D_WINDOWS
        -DWIN95
        -DNSS_NO_INIT_SUPPORT
        -DUSE_UTIL_DIRECTLY
        -DNO_NSPR_10_SUPPORT
        -DSSL_DISABLE_DEPRECATED_CIPHER_SUITE_NAMES
        -DNSS_X86_OR_X64
        -DMP_IS_LITTLE_ENDIAN
        -DMP_ASSEMBLY_MULTIPLY
        -DNSS_BEVAND_ARCFOUR
        -DMP_ASSEMBLY_MULTIPLY
        #-DNSS_USE_COMBA
        -DUSE_HW_AES
        -DINTEL_GCM
        -DKRML_NOUINT128
        -DMP_API_COMPATIBLE
        -DSTDC_HEADERS
        -DHAVE_STRERROR
        -DHAVE_SNPRINTF
        -DMEMMOVE
        -D__DBINTERFACE_PRIVATE
        -Dinline=__inline )

# freebl
set ( ${PROJECT_NAME}_SOFTOKN_FREEBL_SRC
        # ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/loader.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/freeblver.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ldvector.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/sysrand.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/sha_fast.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/md2.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/md5.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/sha512.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/alghmac.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/rawhash.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/alg2268.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/arcfour.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/arcfive.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/crypto_primitives.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/blake2b.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/desblapi.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/des.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/drbg.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/cts.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ctr.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/blinit.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/fipsfreebl.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/gcm.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/hmacct.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/rijndael.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/aeskeywrap.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/camellia.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/chacha20poly1305.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/poly1305.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/dh.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ec.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecdecode.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/pqg.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/dsa.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/rsa.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/rsapkcs.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/shvfy.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/tlsprfalg.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/seed.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/jpake.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/mpi/mpprime.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/mpi/mpmontg.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/mpi/mplogic.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/mpi/mpi.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/mpi/mp_gf2m.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/mpi/mpcpucache.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/ecl.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/ecl_mult.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/ecl_gf.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/ecp_aff.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/ecp_jac.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/ecp_mont.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/ec_naf.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/ecp_jm.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/ecp_25519.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/ecp_256.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/ecp_384.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/ecp_521.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/ecp_256_32.c
        #${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/verified/Hacl_Chacha20.c
        #${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/verified/Hacl_Chacha20_Vec128.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/verified/Hacl_Curve25519.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/verified/FStar.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/gcm-x86.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/aes-x86.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/intel-gcm-wrap.c )
list ( APPEND ${PROJECT_NAME}_SOFTOKN_SRC ${${PROJECT_NAME}_SOFTOKN_FREEBL_SRC} )
source_group ( "src_freebl" FILES ${${PROJECT_NAME}_SOFTOKN_FREEBL_SRC} )

# sqlite
set ( ${PROJECT_NAME}_SOFTOKN_SQLITE_SRC               
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/sqlite/sqlite3.c )
list ( APPEND ${PROJECT_NAME}_SOFTOKN_SRC ${${PROJECT_NAME}_SOFTOKN_SQLITE_SRC} )
source_group ( "src_sqlite" FILES ${${PROJECT_NAME}_SOFTOKN_SQLITE_SRC} )

# dbm
set ( ${PROJECT_NAME}_SOFTOKN_DBM_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dbm/src/db.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dbm/src/h_bigkey.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dbm/src/h_func.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dbm/src/h_log2.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dbm/src/h_page.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dbm/src/hash.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dbm/src/hash_buf.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dbm/src/mktemp.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dbm/src/dirent.c )
list ( APPEND ${PROJECT_NAME}_SOFTOKN_SRC ${${PROJECT_NAME}_SOFTOKN_DBM_SRC} )
source_group ( "src_dbm" FILES ${${PROJECT_NAME}_SOFTOKN_DBM_SRC} )

# softoken
set ( ${PROJECT_NAME}_SOFTOKN_SOFTOKEN_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/fipsaudt.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/fipstest.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/fipstokn.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/lgglue.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/lowkey.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/lowpbe.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/padbuf.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/pkcs11.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/pkcs11c.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/pkcs11u.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/sdb.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/sftkdb.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/sftkhmac.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/sftkpars.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/sftkpwd.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/softkver.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/tlsprf.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/jpakesftk.c )
list ( APPEND ${PROJECT_NAME}_SOFTOKN_SRC ${${PROJECT_NAME}_SOFTOKN_SOFTOKEN_SRC} )
source_group ( "src_softoken" FILES ${${PROJECT_NAME}_SOFTOKN_SOFTOKEN_SRC} )

# softoken/legacydb
set ( ${PROJECT_NAME}_SOFTOKN_LEGACYDB_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/legacydb/dbmshim.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/legacydb/keydb.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/legacydb/lgattr.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/legacydb/lgcreate.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/legacydb/lgdestroy.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/legacydb/lgfind.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/legacydb/lginit.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/legacydb/lgutil.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/legacydb/lowcert.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/legacydb/lowkey.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/legacydb/pcertdb.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken/legacydb/pk11db.c )
list ( APPEND ${PROJECT_NAME}_SOFTOKN_SRC ${${PROJECT_NAME}_SOFTOKN_LEGACYDB_SRC} )
source_group ( "src_legacydb" FILES ${${PROJECT_NAME}_SOFTOKN_LEGACYDB_SRC} )

# includes
set ( ${PROJECT_NAME}_SOFTOKN_INCLUDES 
        ${${PROJECT_NAME}_UTIL_SOURCE_DIR}/lib/util
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/sqlite
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dbm/include
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/mpi
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/verified
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/softoken )

if ( MSVC_VERSION STREQUAL 1600 )
    list ( APPEND ${PROJECT_NAME}_SOFTOKN_INCLUDES
            ${CMAKE_CURRENT_LIST_DIR}/../nss_include/msvc_compat/C99 )
endif ( )

# main
set ( ${PROJECT_NAME}_SRC )
set ( ${PROJECT_NAME}_OPTS
        /wd4013
        /wd4018
        /wd4133
        /wd4244
        /wd4267 )
set ( ${PROJECT_NAME}_DEFS
        -D_CRT_SECURE_NO_WARNINGS
        -D_CRT_NONSTDC_NO_WARNINGS
        -DXP_PC
        -DWIN32
        -UDEBUG
        -DNDEBUG
        -D_WINDOWS
        -DWIN95
        -DUSE_UTIL_DIRECTLY
        -DNO_NSPR_10_SUPPORT
        -DSSL_DISABLE_DEPRECATED_CIPHER_SUITE_NAMES
        -DMOZILLA_CLIENT=1
        -DNSS_X86_OR_X64
        -DNSS_ENABLE_ZLIB
        -DIN_LIBSSL
        -DWIN32_NSS3_DLL_COMPAT
        -DSHLIB_SUFFIX=\"dll\"
        -DSHLIB_PREFIX=\"\"
        -DSHLIB_VERSION=\"\"
        -DSOFTOKEN_SHLIB_VERSION=\"3\"
        -Dinline=__inline )

# base
set ( ${PROJECT_NAME}_BASE_SRC       
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/base/arena.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/base/error.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/base/errorval.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/base/hashops.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/base/libc.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/base/tracker.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/base/item.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/base/utf8.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/base/list.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/base/hash.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_BASE_SRC} )
source_group ( "src_base" FILES ${${PROJECT_NAME}_BASE_SRC} )        

# dev
set ( ${PROJECT_NAME}_DEV_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dev/devslot.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dev/devtoken.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dev/devutil.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dev/ckhelper.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_DEV_SRC} )
source_group ( "src_dev" FILES ${${PROJECT_NAME}_DEV_SRC} )

# pki
set ( ${PROJECT_NAME}_PKI_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pki/asymmkey.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pki/certificate.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pki/cryptocontext.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pki/symmkey.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pki/trustdomain.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pki/tdcache.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pki/certdecode.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pki/pkistore.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pki/pkibase.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pki/pki3hack.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_PKI_SRC} )
source_group ( "src_dev" FILES ${${PROJECT_NAME}_PKI_SRC} )

# libpkix
set ( ${PROJECT_NAME}_LIBPIX_SRC
        # libpkix/pkix/certsel
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/certsel/pkix_certselector.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/certsel/pkix_comcertselparams.c
        # libpkix/pkix/crlsel
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/crlsel/pkix_crlselector.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/crlsel/pkix_comcrlselparams.c
        # libpkix/pkix/checker
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker/pkix_basicconstraintschecker.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker/pkix_certchainchecker.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker/pkix_crlchecker.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker/pkix_ekuchecker.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker/pkix_expirationchecker.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker/pkix_namechainingchecker.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker/pkix_nameconstraintschecker.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker/pkix_ocspchecker.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker/pkix_revocationmethod.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker/pkix_revocationchecker.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker/pkix_policychecker.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker/pkix_signaturechecker.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker/pkix_targetcertchecker.c
        # libpkix/pkix/params
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/params/pkix_trustanchor.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/params/pkix_procparams.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/params/pkix_valparams.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/params/pkix_resourcelimits.c
        # libpkix/pkix/results
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/results/pkix_buildresult.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/results/pkix_policynode.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/results/pkix_valresult.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/results/pkix_verifynode.c
        # libpkix/pkix/store
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/store/pkix_store.c
        # libpkix/pkix/top
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/top/pkix_validate.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/top/pkix_lifecycle.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/top/pkix_build.c
        # libpkix/pkix/util
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/util/pkix_tools.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/util/pkix_error.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/util/pkix_logger.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/util/pkix_list.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/util/pkix_errpaths.c
        # libpkix/pkix_pl_nss/pki
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_basicconstraints.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_cert.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_certpolicyinfo.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_certpolicymap.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_certpolicyqualifier.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_crl.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_crldp.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_crlentry.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_date.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_generalname.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_infoaccess.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_nameconstraints.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_ocsprequest.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_ocspresponse.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_publickey.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_x500name.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki/pkix_pl_ocspcertid.c
        # libpkix/pkix_pl_nss/system
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_bigint.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_bytearray.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_common.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_error.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_hashtable.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_lifecycle.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_mem.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_monitorlock.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_mutex.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_object.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_oid.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_primhash.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_rwlock.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system/pkix_pl_string.c
        # libpkix/pkix_pl_nss/module
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/module/pkix_pl_aiamgr.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/module/pkix_pl_colcertstore.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/module/pkix_pl_httpcertstore.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/module/pkix_pl_httpdefaultclient.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/module/pkix_pl_ldaptemplates.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/module/pkix_pl_ldapcertstore.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/module/pkix_pl_ldapresponse.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/module/pkix_pl_ldaprequest.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/module/pkix_pl_ldapdefaultclient.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/module/pkix_pl_nsscontext.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/module/pkix_pl_pk11certstore.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/module/pkix_pl_socket.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_LIBPIX_SRC} )
source_group ( "src_libpkix" FILES ${${PROJECT_NAME}_LIBPIX_SRC} )

# lib/certdb
set ( ${PROJECT_NAME}_CERTB_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certdb/alg1485.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certdb/certdb.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certdb/certv3.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certdb/certxutl.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certdb/crl.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certdb/genname.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certdb/stanpcertdb.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certdb/polcyxtn.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certdb/secname.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certdb/xauthkid.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certdb/xbsconst.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certdb/xconst.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_CERTB_SRC} )
source_group ( "src_certdb" FILES ${${PROJECT_NAME}_CERTB_SRC} )

# lib/certhigh
set ( ${PROJECT_NAME}_CERHIGH_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certhigh/certhtml.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certhigh/certreq.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certhigh/crlv2.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certhigh/ocsp.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certhigh/ocspsig.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certhigh/certhigh.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certhigh/certvfy.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certhigh/certvfypkix.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certhigh/xcrldist.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_CERHIGH_SRC} )
source_group ( "src_certhigh" FILES ${${PROJECT_NAME}_CERHIGH_SRC} )

# lib/pk11wrap
set ( ${PROJECT_NAME}_PK11WRAP_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/dev3hack.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11akey.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11auth.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11cert.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11cxt.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11err.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11kea.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11list.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11load.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11mech.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11merge.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11nobj.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11obj.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11pars.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11pbe.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11pk12.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11pqg.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11sdr.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11skey.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11slot.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap/pk11util.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_PK11WRAP_SRC} )
source_group ( "src_pk11wrap" FILES ${${PROJECT_NAME}_PK11WRAP_SRC} )

# lib/cryptohi
set ( ${PROJECT_NAME}_CRYPTOHI_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/cryptohi/sechash.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/cryptohi/seckey.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/cryptohi/secsign.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/cryptohi/secvfy.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/cryptohi/dsautil.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_CRYPTOHI_SRC} )
source_group ( "src_cryptohi" FILES ${${PROJECT_NAME}_CRYPTOHI_SRC} )

# lib/nss
set ( ${PROJECT_NAME}_NSS_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/nss/nssinit.c
        #${${PROJECT_NAME}_SOURCE_DIR}/lib/nss/nssoptions.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/nss/nssver.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/nss/utilwrap.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_NSS_SRC} )
source_group ( "src_nss" FILES ${${PROJECT_NAME}_NSS_SRC} )

# lib/pkcs7
set ( ${PROJECT_NAME}_PKCS7_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs7/certread.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs7/p7common.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs7/p7create.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs7/p7decode.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs7/p7encode.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs7/p7local.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs7/secmime.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_PKCS7_SRC} )
source_group ( "src_pkcs7" FILES ${${PROJECT_NAME}_PKCS7_SRC} )

# lib/pkcs12
set ( ${PROJECT_NAME}_PKCS12_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs12/p12local.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs12/p12creat.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs12/p12dec.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs12/p12plcy.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs12/p12tmpl.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs12/p12e.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs12/p12d.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_PKCS12_SRC} )
source_group ( "src_pkcs12" FILES ${${PROJECT_NAME}_PKCS12_SRC} )

# lib/smime
set ( ${PROJECT_NAME}_SMIME_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsarray.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsasn1.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsattr.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmscinfo.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmscipher.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsdecode.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsdigdata.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsdigest.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsencdata.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsencode.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsenvdata.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsmessage.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmspubkey.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsrecinfo.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsreclist.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmssigdata.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmssiginfo.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsudf.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/cmsutil.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/smimemessage.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/smimeutil.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime/smimever.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_SMIME_SRC} )
source_group ( "src_smime" FILES ${${PROJECT_NAME}_SMIME_SRC} )

# lib/crmf
set ( ${PROJECT_NAME}_CRMF_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/crmfenc.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/crmftmpl.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/crmfreq.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/crmfpop.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/crmfdec.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/crmfget.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/crmfcont.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/cmmfasn1.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/cmmfresp.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/cmmfrec.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/cmmfchal.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/servget.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/encutil.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/respcli.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/respcmn.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/challcli.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf/asn1cmn.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_CRMF_SRC} )
source_group ( "src_smime" FILES ${${PROJECT_NAME}_CRMF_SRC} )

# lib/jar
set ( ${PROJECT_NAME}_JAR_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/jar/jarver.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/jar/jarsign.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/jar/jar.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/jar/jar-ds.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/jar/jarfile.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/jar/jarint.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_JAR_SRC} )
source_group ( "src_jar" FILES ${${PROJECT_NAME}_JAR_SRC} )

# lib/ckfw
set ( ${PROJECT_NAME}_CKFW_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/crypto.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/find.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/hash.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/instance.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/mutex.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/object.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/session.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/sessobj.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/slot.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/token.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/wrap.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/mechanism.c        
        # builtins
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/builtins/anchor.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/builtins/constants.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/builtins/bfind.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/builtins/binst.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/builtins/bobject.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/builtins/bsession.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/builtins/bslot.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/builtins/btoken.c
        ${CMAKE_CURRENT_LIST_DIR}/../certdata/certdata.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/builtins/ckbiver.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_CKFW_SRC} )
source_group ( "src_ckfw" FILES ${${PROJECT_NAME}_CKFW_SRC} )

# lib/sysinit
set ( ${PROJECT_NAME}_SYSINIT_SRC
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/sysinit/nsssysinit.c )
list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_SYSINIT_SRC} )
source_group ( "src_sysinit" FILES ${${PROJECT_NAME}_SYSINIT_SRC} )

# includes
set ( ${PROJECT_NAME}_INCLUDES 
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/base
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dev
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pki
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/include
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/certsel
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/crlsel
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/checker
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/params
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/results
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/store
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/top
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix/util
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/pki
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/system
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/libpkix/pkix_pl_nss/module
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certdb
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/certhigh
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pk11wrap
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/cryptohi
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/nss
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ssl
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs7
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pkcs12
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smime
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/crmf
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/jar
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ckfw/builtins
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/sysinit )
        
if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_i386_[^_]+" )
    list ( APPEND ${PROJECT_NAME}_UTIL_DEFS
            -D_X86_ )          
    list ( APPEND ${PROJECT_NAME}_SOFTOKN_SRC
            ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/curve25519_64.c
            ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/mpi/mpi_x86_asm.c
            ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/intel-aes-x86-masm.asm
            ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/intel-gcm-x86-masm.asm )
    list ( APPEND ${PROJECT_NAME}_SOFTOKN_DEFS
            -DMP_USE_UINT_DIGIT
            -D_X86_
            -DNSS_X86
            -DMP_ASSEMBLY_SQUARE
            -DMP_ASSEMBLY_DIV_2DX1D )
    list ( APPEND ${PROJECT_NAME}_DEFS
            -D_X86_
            -DNSS_X86 )
endif ( )

if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_amd64_[^_]+" )
    list ( APPEND ${PROJECT_NAME}_UTIL_DEFS
           -D_AMD64_=1 )
    set ( ${PROJECT_NAME}_SOFTOKN_WIN64_SRC
            ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/ecl/curve25519_64.c
            ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/mpi/mpi_amd64.c
            ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/mpi/mpi_amd64_masm.asm
            ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/arcfour-amd64-masm.asm
            ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/intel-aes-x64-masm.asm
            ${${PROJECT_NAME}_SOURCE_DIR}/lib/freebl/intel-gcm-x64-masm.asm )
    list ( APPEND ${PROJECT_NAME}_SOFTOKN_SRC ${${PROJECT_NAME}_SOFTOKN_WIN64_SRC} )
    source_group ( "src_win64" FILES ${${PROJECT_NAME}_SOFTOKN_WIN64_SRC} )
    list ( APPEND ${PROJECT_NAME}_SOFTOKN_DEFS
            -D_AMD64_
            -DNSS_X64
            -DNSS_USE_64
            -DMPI_AMD64 )
    list ( APPEND ${PROJECT_NAME}_DEFS
            -D_AMD64_
            -DNSS_X64
            -DNSS_USE_64
            -DMPI_AMD64 )
endif ( )

add_library ( ${PROJECT_NAME}_util STATIC ${${PROJECT_NAME}_UTIL_SRC} ${${PROJECT_NAME}_UTIL_HEADERS} )
target_include_directories ( ${PROJECT_NAME}_util BEFORE PRIVATE 
        ${${PROJECT_NAME}_UTIL_INCLUDES}
        ${${PROJECT_NAME}_NSPR_INCLUDES}
        ${${PROJECT_NAME}_DEPS_PC_INCLUDE_DIRS} ) 
target_compile_options ( ${PROJECT_NAME}_util PRIVATE ${${PROJECT_NAME}_UTIL_OPTS} )
target_compile_definitions ( ${PROJECT_NAME}_util PRIVATE ${${PROJECT_NAME}_UTIL_DEFS} )

add_library ( ${PROJECT_NAME}_softokn STATIC ${${PROJECT_NAME}_SOFTOKN_SRC} )
target_include_directories ( ${PROJECT_NAME}_softokn BEFORE PRIVATE 
        ${${PROJECT_NAME}_SOFTOKN_INCLUDES}
        ${${PROJECT_NAME}_NSPR_INCLUDES}
        ${${PROJECT_NAME}_DEPS_PC_INCLUDE_DIRS} ) 
target_compile_options ( ${PROJECT_NAME}_softokn PRIVATE ${${PROJECT_NAME}_SOFTOKN_OPTS} )
target_compile_definitions ( ${PROJECT_NAME}_softokn PRIVATE ${${PROJECT_NAME}_SOFTOKN_DEFS} )

add_library ( ${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SRC} )
target_include_directories ( ${PROJECT_NAME} BEFORE PRIVATE
        ${${PROJECT_NAME}_INCLUDES}
        ${${PROJECT_NAME}_UTIL_INCLUDES}
        ${${PROJECT_NAME}_SOFTOKN_INCLUDES}
        ${${PROJECT_NAME}_NSPR_INCLUDES}
        ${${PROJECT_NAME}_DEPS_PC_INCLUDE_DIRS} ) 
target_compile_options ( ${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_OPTS} )
target_compile_definitions ( ${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_DEFS} )

# build order
add_dependencies ( ${PROJECT_NAME}_util lookaside_nspr )
add_dependencies ( ${PROJECT_NAME}_softokn ${PROJECT_NAME}_util )
add_dependencies ( ${PROJECT_NAME} ${PROJECT_NAME}_softokn )
