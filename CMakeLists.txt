cmake_minimum_required(VERSION 3.1)

project(Patcher VERSION 0.1 LANGUAGES C CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SOURSE_DIR ./srs)
set(HEADERS_DIR ./include)

set(TS_FILES ./patcher_ru_RU.ts)
set(UI_FILE ${SOURSE_DIR}/mainwindow.ui ${SOURSE_DIR}/commandwind.ui)

set(HEADERS ${HEADERS_DIR}/sshclient.h ${HEADERS_DIR}/sshchannel.h ${HEADERS_DIR}/sshkey.h ${HEADERS_DIR}/sshprocess.h
    ${HEADERS_DIR}/sshsftp.h ${HEADERS_DIR}/sshsftpcommandsend.h ${HEADERS_DIR}/sshsftpcommand.h
    ${HEADERS_DIR}/patcherfunc.h ${HEADERS_DIR}/jsoncontroller.h ${HEADERS_DIR}/commandtable.h ${HEADERS_DIR}/mainwindow.h
    ${HEADERS_DIR}/device.h ${HEADERS_DIR}/commandwind.h)

aux_source_directory(./srs SRC_LIST)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    message("OpenSSL ${OPENSSL_VERSION} was found ")
endif()
find_package(Libssh2 REQUIRED CONFIG)
if(LIBSSH2_FOUND)
    message("Libssh2 was found ")
endif()
#find_package(Libssh2 REQUIRED CONFIG)
#if(LIBSSH2_FOUND)
#    message("Libssh2 was found ")
#endif()


find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Network Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network Widgets LinguistTools REQUIRED)
set(QT_LIBRARIES Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Widgets)
if(Qt5_FOUND)
    message("Qt ${Qt5_VERSION} was found ")
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADERS} ${TS_FILES} ${UI_FILE})
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADERS} ${TS_FILES} ${UI_FILE})
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()


target_include_directories(${PROJECT_NAME} PRIVATE ${HEADERS_DIR})
target_link_libraries(${PROJECT_NAME} Libssh2::libssh2 ${QT_LIBRARIES} OpenSSL::SSL)

message(${CMAKE_SOURCE_DIR})
message(${CMAKE_CURRENT_SOURCE_DIR})
message(${PROJECT_SOURCE_DIR})
message(${${PROJECT_NAME}_SOURCE_DIR})
