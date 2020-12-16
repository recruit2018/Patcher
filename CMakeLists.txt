cmake_minimum_required(VERSION 3.1 )


project(Patcher VERSION 0.1 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS_DIR ./include)
set(HEADERS ${HEADERS_DIR}/sshclient.h ${HEADERS_DIR}/sshchannel.h ${HEADERS_DIR}/sshkey.h ${HEADERS_DIR}/sshprocess.h
    ${HEADERS_DIR}/sshsftp.h ${HEADERS_DIR}/sshsftpcommandsend.h ${HEADERS_DIR}/sshsftpcommand.h
    ${HEADERS_DIR}/patcherfunc.h ${HEADERS_DIR}/jsoncontroller.h)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    message("OpenSSL${OPENSSL_VERSION} was found ")
endif()

find_package(Libssh2 REQUIRED CONFIG)
if(LIBSSH2_FOUND)
    message("Libssh2 was found ")
endif()

find_package(Qt5 REQUIRED COMPONENTS Core Network Widgets)
if(Qt5_FOUND)
    set(QT_LIBRARIES Qt5::Core Qt5::Network Qt5::Widgets)
    message("Qt${Qt5_VERSION} was found ")
endif()

aux_source_directory(./srs SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${HEADERS_DIR})
target_link_libraries(${PROJECT_NAME} Libssh2::libssh2 ${QT_LIBRARIES} OpenSSL::SSL)

