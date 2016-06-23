CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)
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
	duality-server/handlers/TemplateProcessor.cpp
	duality-server/main.cpp)

TARGET_INCLUDE_DIRECTORIES(
	duality-server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/duality-server)

TARGET_COMPILE_FEATURES(duality-server
    PUBLIC cxx_auto_type
	PUBLIC cxx_lambdas
	PUBLIC cxx_nullptr
	PUBLIC cxx_override
	PUBLIC cxx_range_for
	PUBLIC cxx_rvalue_references
	PUBLIC cxx_static_assert
	PUBLIC cxx_strong_enums
	PUBLIC cxx_uniform_initialization)
	
TARGET_LINK_LIBRARIES(duality-server PRIVATE mocca)