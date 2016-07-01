CMAKE_MINIMUM_REQUIRED(VERSION 3.2.0)
PROJECT(duality-server LANGUAGES CXX)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS on)

ADD_SUBDIRECTORY(mocca/ext/AES)
ADD_SUBDIRECTORY(mocca/ext/base64)
ADD_SUBDIRECTORY(mocca/ext/jsoncpp)
ADD_SUBDIRECTORY(mocca/ext/sha1)
ADD_SUBDIRECTORY(mocca/ext/lz4)
ADD_SUBDIRECTORY(mocca/mocca)

IF (WIN32)
	SET(PLATFORM_SOURCES
		duality-server/common/SysUtil.Windows.cpp)
ELSE ()
	SET(PLATFORM_SOURCES
		duality-server/common/SysUtil.Posix.cpp)
ENDIF ()

ADD_EXECUTABLE(duality-server
	duality-server/common/SysUtil.cpp
	${PLATFORM_SOURCES}	
	duality-server/handlers/Handlers.cpp
	duality-server/main.cpp)

TARGET_INCLUDE_DIRECTORIES(
	duality-server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/duality-server)

SET_PROPERTY(TARGET duality-server PROPERTY CXX_STANDARD 14)

TARGET_LINK_LIBRARIES(duality-server PRIVATE mocca)