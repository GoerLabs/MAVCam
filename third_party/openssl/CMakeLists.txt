cmake_minimum_required(VERSION 3.14)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()
project(external-openssl)

message(STATUS "Preparing external project \"openssl\"")

set(OPENSSL_VERSION 3.0.9)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(OPENSSL_BUILD_TYPE "--debug")
else()
    set(OPENSSL_BUILD_TYPE "--release")
endif()
message("OPENSSL_BUILD_TYPE set to ${OPENSSL_BUILD_TYPE}")

include(ExternalProject)

if(ANDROID)
    if(${CMAKE_ANDROID_ARCH_ABI} STREQUAL "armeabi-v7a")
        set(OPENSSL_PLATFORM "android-arm")
    elseif(${CMAKE_ANDROID_ARCH_ABI} STREQUAL "arm64-v8a")
        set(OPENSSL_PLATFORM "android-arm64")
    elseif(${CMAKE_ANDROID_ARCH_ABI} STREQUAL "x86")
        set(OPENSSL_PLATFORM "android-x86")
    elseif(${CMAKE_ANDROID_ARCH_ABI} STREQUAL "x86_64")
        set(OPENSSL_PLATFORM "android-x86_64")
    else()
        message(FATAL_ERROR "Unexpected PLATFORM: \"${PLATFORM}\"")
    endif()

    message("OPENSSL_PLATFORM set to ${OPENSSL_PLATFORM}")


    ExternalProject_add(
        openssl
        URL https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz
        URL_HASH MD5=8b2aff668b8ce0da24b9505ebfd26b4d
        PREFIX openssl
        DOWNLOAD_DIR "${DOWNLOAD_DIR}"
        PATCH_COMMAND patch -p 0 < ${PROJECT_SOURCE_DIR}/dockcross-android.patch
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env ANDROID_NDK=$ENV{CROSS_ROOT} <SOURCE_DIR>/Configure --sysroot=$ENV{CROSS_ROOT}/sysroot ${OPENSSL_PLATFORM} --prefix=${CMAKE_INSTALL_PREFIX} ${OPENSSL_BUILD_TYPE} no-shared
        BUILD_COMMAND make
        INSTALL_COMMAND make install_sw
        )

elseif(IOS)
    if(${PLATFORM} STREQUAL "OS64")
        set(OPENSSL_PLATFORM "ios64-xcrun")
    elseif(${PLATFORM} STREQUAL "SIMULATOR64")
        set(OPENSSL_PLATFORM "iossimulator-xcrun")
    else()
        message(FATAL_ERROR "Unexpected PLATFORM: \"${PLATFORM}\"")
    endif()

    message("OPENSSL_PLATFORM set to ${OPENSSL_PLATFORM}")

    ExternalProject_add(
        openssl
        URL https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz
        URL_HASH MD5=8b2aff668b8ce0da24b9505ebfd26b4d
        PREFIX openssl
        DOWNLOAD_DIR "${DOWNLOAD_DIR}"
        CONFIGURE_COMMAND <SOURCE_DIR>/Configure ${OPENSSL_PLATFORM} --prefix=${CMAKE_INSTALL_PREFIX} ${OPENSSL_BUILD_TYPE} no-shared
        )

elseif(CMAKE_CROSSCOMPILING) # This assumes we use dockcross
    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm")
        set(OPENSSL_PLATFORM "linux-armv4")
    elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
        set(OPENSSL_PLATFORM "linux-aarch64")
    else()
        message(FATAL_ERROR "Unexpected PLATFORM: \"${PLATFORM}\"")
    endif()

    message("OPENSSL_PLATFORM set to ${OPENSSL_PLATFORM}")

    ExternalProject_add(
        openssl
        URL https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz
        URL_HASH MD5=8b2aff668b8ce0da24b9505ebfd26b4d
        PREFIX openssl
        DOWNLOAD_DIR "${DOWNLOAD_DIR}"
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CROSS_COMPILE="" <SOURCE_DIR>/Configure ${OPENSSL_PLATFORM} --prefix=${CMAKE_INSTALL_PREFIX} ${OPENSSL_BUILD_TYPE} no-shared
        BUILD_COMMAND make
        INSTALL_COMMAND make install_sw
        )

elseif(MSVC)
    message("CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
    # Only available from cmake 3.20, for now do it manually.
    #cmake_path(NATIVE_PATH CMAKE_INSTALL_PREFIX NORMALIZE NATIVE_INSTALL_PREFIX)
    string(REPLACE "/" "\\" NATIVE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    message("NATIVE_INSTALL_PREFIX: ${NATIVE_INSTALL_PREFIX}")

    ExternalProject_add(
        openssl
        URL https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz
        URL_HASH MD5=8b2aff668b8ce0da24b9505ebfd26b4d
        PREFIX openssl
        DOWNLOAD_DIR "${DOWNLOAD_DIR}"
        CONFIGURE_COMMAND perl <SOURCE_DIR>/Configure VC-WIN64A --prefix=${NATIVE_INSTALL_PREFIX} --openssldir=${NATIVE_INSTALL_PREFIX} ${OPENSSL_BUILD_TYPE} no-shared no-asm
        BUILD_COMMAND set MAKEFLAGS= && nmake
        INSTALL_COMMAND set MAKEFLAGS= && nmake install_sw
        )
elseif(CMAKE_HOST_APPLE)
    set(OPENSSL_VERSION 1.1.1o)
    ExternalProject_add(
        openssl
        URL https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz
        URL_HASH MD5=d05e96e200d2ff0aef20c114cb5f17bf
        PREFIX openssl
        DOWNLOAD_DIR "${DOWNLOAD_DIR}"
        CONFIGURE_COMMAND <SOURCE_DIR>/config ${OPENSSL_BUILD_TYPE} --prefix=${CMAKE_INSTALL_PREFIX} no-shared
        BUILD_COMMAND make
        INSTALL_COMMAND make install_sw
        )
else()
    ExternalProject_add(
        openssl
        URL https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz
        URL_HASH MD5=8b2aff668b8ce0da24b9505ebfd26b4d
        PREFIX openssl
        DOWNLOAD_DIR "${DOWNLOAD_DIR}"
        CONFIGURE_COMMAND <SOURCE_DIR>/config ${OPENSSL_BUILD_TYPE} --prefix=${CMAKE_INSTALL_PREFIX} no-shared
        BUILD_COMMAND make
        INSTALL_COMMAND make install_sw
        )
endif()
