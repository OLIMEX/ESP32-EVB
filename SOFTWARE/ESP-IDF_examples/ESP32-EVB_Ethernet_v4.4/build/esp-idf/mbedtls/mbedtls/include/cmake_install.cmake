# Install script for directory: C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ethernet_basic")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Espressif/tools/xtensa-esp32-elf/esp-2021r2-patch2-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-objdump.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aesni.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/arc4.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/blowfish.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/bn_mul.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/certs.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cipher_internal.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/compat-1.3.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecp_internal.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/entropy_poll.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/havege.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md2.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md4.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md_internal.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/net.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/padlock.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pk_internal.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs11.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/rsa_internal.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_internal.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
    "C:/Users/balano/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/xtea.h"
    )
endif()

