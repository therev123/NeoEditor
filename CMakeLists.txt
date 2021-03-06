PROJECT(MARATIS)

## Let's use a reasonable modern version
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_HAS_SORT 1)
SET(CMAKE_MODULE_PATH
  ${MARATIS_SOURCE_DIR}/CMake
  ${MARATIS_SOURCE_DIR}/CMake/Packages
  ${CMAKE_MODULE_PATH}
)

## Disallow in-source builds
INCLUDE(OutOfSourceBuild)

## Look for SSE
INCLUDE(FindSSE)

## Documentation tool
FIND_PACKAGE(Doxygen 1.8.3)

## Set version and info
SET(MARATIS_MAJOR_VERSION 0)
SET(MARATIS_MINOR_VERSION 4)
SET(MARATIS_PATCH_VERSION 0)

IF("${NEO_VERSION_STRING}" STREQUAL "")
  SET(NEO_VERSION_STRING "\"v0.4\"")
  SET(IS_DAILY_BUILD FALSE)
ELSE()
  SET(IS_DAILY_BUILD TRUE)
ENDIF()

IF("${ANDROID}" STREQUAL "")
  SET(ANDROID FALSE)
ENDIF()

IF("${EMSCRIPTEN}" STREQUAL "")
  SET(EMSCRIPTEN FALSE)
ENDIF()

MESSAGE(STATUS "Git version: " ${NEO_VERSION_STRING})

SET(MARATIS_PACKAGE "Neo")
SET(MARATIS_VENDOR "Yannick Pflanzer")
SET(MARATIS_VERSION "${MARATIS_MAJOR_VERSION}.${MARATIS_MINOR_VERSION}.${MARATIS_PATCH_VERSION}")
SET(MARATIS_COPYRIGHT "Copyright (c) 2014, ${MARATIS_VENDOR}.")

MESSAGE(STATUS "${MARATIS_PACKAGE} ${MARATIS_VERSION}")

## Packaging
IF(NOT ${IS_DAILY_BUILD})
    SET(CPACK_PACKAGE_NAME "${MARATIS_PACKAGE}")
ELSE()
    SET(CPACK_PACKAGE_NAME "${MARATIS_PACKAGE}-git")
ENDIF()

SET(CPACK_PACKAGE_VENDOR "${MARATIS_VENDOR}")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Neo Editor and Maratis game engine.")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Data/neo-icon.png")
SET(CPACK_PACKAGE_VERSION_MAJOR "${MARATIS_MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${MARATIS_MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${MARATIS_PATCH_VERSION}")

## Set install path
SET(MARATIS_INSTALL_DIR Neo)

## Set build flags
IF(WIN32)
  FIND_PACKAGE(OpenGL REQUIRED)

  LINK_DIRECTORIES("$ENV{DXSDK_DIR}/Lib/$ENV{PROCESSOR_ARCHITECTURE}")
  SET(SDL_BUILDING_LIBRARY ON)
  SET(USE_NETWORKING TRUE)
  
  SET(DIRENT_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/dirent/include)

  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    # LuaJIT does not build with the current MinGW
    SET(STANDARD_LUA TRUE)

    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MARATIS_BUILD_FLAGS} -O2 -DWIN32 -I\"$ENV{DXSDK_DIR}/Include\"")
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__Debug__ -I\"$ENV{DXSDK_DIR}/Include\"")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MARATIS_BUILD_FLAGS} -O2 -DWIN32 -I\"$ENV{DXSDK_DIR}/Include\"")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__Debug__ -I\"$ENV{DXSDK_DIR}/Include\"")

    SET(CMAKE_SHARED_LINKER_FLAGS "-lcomctl32 -lkernel32 -Wl,--subsystem,windows,--as-needed,--allow-multiple-definition -static -static-libstdc++ -static-libgcc")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--allow-multiple-definition -static -static-libstdc++ -static-libgcc")
    SET(CPACK_STRIP_FILES ON)

  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++
    
    IF(SSE2_TRUE)	
		SET(SSE_FLAG /arch:SSE2)
	ELSEIF(SSE_TRUE)
		SET(SSE_FLAG /arch:SSE)
    ENDIF()

	
    SET(flag_var "/MD")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MARATIS_BUILD_FLAGS} /EHsc /nologo /GR /FC /DWIN32 /D_MBCS")
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Z7 /W4 /Od /MDd /D_DEBUG /D__Debug__")
    SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /W1 /O2 /Ot /MD /DNDEBUG /fp:fast ${SSE_FLAG}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MARATIS_BUILD_FLAGS} /EHsc /nologo /GR /FC /DWIN32 /D_MBCS")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7 /W4 /Od /MDd /D_DEBUG /D__Debug__")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W1 /O2 /Ot /MD /DNDEBUG /fp:fast ${SSE_FLAG}")
  endif()

  SET(FREETYPE_LIBRARY "freetype")
  SET(OPENAL_LIBRARY "OpenAL32")
  SET(ZLIB_LIBRARY "zlib")
  SET(MARATIS_BUILD_FLAGS "")
  
  SET(CPACK_PACKAGE_ICON "")
  SET(CPACK_NSIS_CREATE_ICONS "CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\NeoEditor.lnk' '\$INSTDIR\\\\Neo\\\\neo-editor.exe'")
	
  SET(CPACK_GENERATOR "NSIS;ZIP")
ENDIF(WIN32)

IF(APPLE)
  FIND_PACKAGE(OpenGL REQUIRED)
  FIND_LIBRARY(FREETYPE_LIBRARY freetype PATHS "${MARATIS_SOURCE_DIR}/3rdparty/freetype/osx" NO_DEFAULT_PATH)
  FIND_LIBRARY(ZLIB_LIBRARY zlib PATHS "${MARATIS_SOURCE_DIR}/3rdparty/zlib/osx" NO_DEFAULT_PATH)
  FIND_LIBRARY(OPENAL_LIBRARY OpenAL)
  SET(MARATIS_BUILD_FLAGS "-isysroot /Developer/SDKs/MacOSX10.6.sdk -DOSX")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MARATIS_BUILD_FLAGS}")
  SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__Debug__")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MARATIS_BUILD_FLAGS}")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__Debug__")
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl")
ENDIF(APPLE)

IF(UNIX AND NOT APPLE AND NOT ANDROID AND NOT EMSCRIPTEN)
  
  SET(USE_NETWORKING TRUE)

  IF(USE_GLES)
    FIND_PACKAGE(OpenGLES REQUIRED)
    FIND_PACKAGE(OpenGLES2 REQUIRED)
    ADD_DEFINITIONS(-DUSE_GLES)
  ELSEIF()
    FIND_PACKAGE(OpenGL REQUIRED)
  ENDIF(USE_GLES)  

  SET(FREETYPE_LIBRARY "freetype")
  SET(OPENAL_LIBRARY "openal")
  SET(ZLIB_LIBRARY "zlib")

  SET(MARATIS_BUILD_FLAGS "")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MARATIS_BUILD_FLAGS} -O3 -fPIC -DLINUX")
  SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -Og -D__Debug__")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MARATIS_BUILD_FLAGS} -O3 -fPIC -DLINUX")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Og -D__Debug__")
  SET(CMAKE_SHARED_LINKER_FLAGS "-ldl -Wl,--as-needed,--allow-multiple-definition,--build-id")

  SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lm -ldl -Wl,--allow-multiple-definition,-rpath ." )

  SET(CPACK_PACKAGE_CONTACT Yannick Pflanzer <yannick.pflanzer@scary-squid.de>)
  SET(CPACK_GENERATOR "STGZ;TGZ;DEB;RPM;")
  SET(CPACK_STRIP_FILES ON)

  # Don't use -fPIC for executables
  SET(DISABLE_PIC_EXECUTABLES ON)
  
ENDIF(UNIX AND NOT APPLE AND NOT ANDROID AND NOT EMSCRIPTEN)

IF(ANDROID)

  FIND_PACKAGE(OpenGLES REQUIRED)
  FIND_PACKAGE(OpenGLES2 REQUIRED)

  SET(FREETYPE_LIBRARY "freetype")
  SET(OPENAL_LIBRARY "openal")
  SET(ZLIB_LIBRARY "zlib")

  SET(STANDARD_LUA ON)
  SET(MARATIS_BUILD_FLAGS "")
  
  SET(ANDROID_LIBDIR ${OPENGLES2_INCLUDE_DIR}/../lib)
  
  SET(ARM_FLAGS "-march=armv6 -marm -mfloat-abi=softfp -mfpu=vfp")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MARATIS_BUILD_FLAGS} -I${OPENGLES2_INCLUDE_DIR} -O3 ${ARM_FLAGS}")
  SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -I${OPENGLES2_INCLUDE_DIR} -D__Debug__")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MARATIS_BUILD_FLAGS} ${ARM_FLAGS} -O3 -fno-exceptions -fno-rtti -nostdlib -I${OPENGLES2_INCLUDE_DIR} -I${ANDROID_NDK_ROOT}/sources/cxx-stl/stlport/stlport")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -I${OPENGLES2_INCLUDE_DIR} -D__Debug__")

  SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L${OPENGLES2_INCLUDE_DIR}/../lib -L${LIBRARY_OUTPUT_PATH_ROOT}/libs/armeabi-v7a" )
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -L${OPENGLES2_INCLUDE_DIR}/../lib -L${LIBRARY_OUTPUT_PATH_ROOT}/libs/armeabi-v7a")

  SET(ANDROID_USE_STLPORT TRUE)
  SET(LIBRARY_OUTPUT_PATH_ROOT ${CMAKE_SOURCE_DIR})

  ADD_DEFINITIONS("-Dva_list=__builtin_va_list")
  ADD_DEFINITIONS("-Dva_start=__builtin_va_start")
  ADD_DEFINITIONS("-Dva_end=__builtin_va_end")
  ADD_DEFINITIONS("-Dva_arg=__builtin_va_arg")
  ADD_DEFINITIONS("-DHAVE_MALLOC")
  ADD_DEFINITIONS("-DANDROID")
  ADD_DEFINITIONS("-DUSE_GLES")
  ADD_DEFINITIONS("-D__ANDROID__")
  
  # for libogg
  ADD_DEFINITIONS("-D_ANDROID")
  
  # for openal
  SET(LIBTYPE STATIC)
ENDIF()

IF(EMSCRIPTEN)
  FIND_PACKAGE(OpenGLES REQUIRED)
  FIND_PACKAGE(OpenGLES2 REQUIRED)

  SET(USE_GLES TRUE)
  SET(FREETYPE_LIBRARY "freetype")
  SET(OPENAL_LIBRARY "openal")
  SET(ZLIB_LIBRARY "zlib")

  SET(MARATIS_BUILD_FLAGS "")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MARATIS_BUILD_FLAGS} -O3 -fPIC -DLINUX")
  SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -D__Debug__")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MARATIS_BUILD_FLAGS} -O3 -fPIC -DLINUX")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D__Debug__")
  SET(CMAKE_SHARED_LINKER_FLAGS "-ldl -Wl,--as-needed,--allow-multiple-definition,--build-id")

  SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} --preload-file project" )

  SET(SDL_VIDEO TRUE)
  add_definitions(-Wno-warn-absolute-paths -s FULL_ES2=1)
  ADD_DEFINITIONS("-DUSE_GLES")

  SET(SDL_VIDEO_DRIVER_EMSCRIPTEN TRUE)

  # Don't use -fPIC for executables
  SET(DISABLE_PIC_EXECUTABLES ON)
ENDIF()

# Include cpack after all system specifics
INCLUDE(CPack)

## Set up 3rd party dependencies
SET(ASSIMP_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/assimp/include)
SET(BULLET_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/bullet)
SET(FREETYPE_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/freetype/include)
SET(GLEW_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/glew/include/GL)
SET(JPEG_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/libjpeg)
SET(PNG_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/libpng ${MARATIS_SOURCE_DIR}/3rdparty/)
SET(OGG_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/libogg/include)
SET(VORBIS_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/libvorbis/include)

if(STANDARD_LUA)
    SET(LUA_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/lua-standard)
else()
    SET(LUA_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/lua/src ${CMAKE_BINARY_DIR}/3rdparty/lua)
endif(STANDARD_LUA)

SET(NPK_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/npk/include)

SET(SDL_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/sdl2/include)
SET(OPENAL_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/openal-soft/include)
SET(TINYXML_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/tinyxml)
SET(ZLIB_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/zlib)
SET(FLTK_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/fltk ${CMAKE_BINARY_DIR}/3rdparty/fltk)

SET(ASSIMP_LIBRARIES assimp)
SET(BULLET_LIBRARIES bullet)
SET(FREETYPE_LIBRARIES "${FREETYPE_LIBRARY}")
SET(GLEW_LIBRARIES libglew_static)
SET(JPEG_LIBRARIES jpeg)
SET(PNG_LIBRARIES png)
SET(OGG_LIBRARIES ogg)
SET(VORBIS_LIBRARIES vorbis)
SET(FLTK_LIBRARIES fltk_gl)

if(STANDARD_LUA)
    SET(LUA_LIBRARIES lua)
else()
    SET(LUA_LIBRARIES liblua)
endif(STANDARD_LUA)

if(USE_NETWORKING)
    SET(RAKNET_LIBRARIES RakNetLibStatic)
    SET(RAKNET_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/3rdparty/raknet/Source)
    add_definitions(-DUSE_NETWORKING)
endif()

SET(NPK_LIBRARIES npk)
SET(SDL_LIBRARIES SDL2)
SET(TINYXML_LIBRARIES tinyxml)
SET(OPENAL_LIBRARIES "${OPENAL_LIBRARY}")
SET(ZLIB_LIBRARIES "${ZLIB_LIBRARY}")

## Set up SDK dependencies
SET(MCORE_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/MSDK/MCore/Includes)
SET(MENGINE_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/MSDK/MEngine/Includes)
SET(MGUI_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/MSDK/MGui/Includes)

SET(MCORE_LIBRARIES MCore)
SET(MENGINE_LIBRARIES MEngine)
SET(MGUI_LIBRARIES MGui)

## Set up Maratis dependencies
SET(MARATIS_COMMON_INCLUDE_DIR ${MARATIS_SOURCE_DIR}/Maratis/Common)
SET(MARATIS_COMMON_LIBRARIES MaratisCommon)

## Set up binary output path
SET(MARATIS_BINARY_OUTPUT ${CMAKE_BINARY_DIR}/BinaryOutput)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MARATIS_BINARY_OUTPUT})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MARATIS_BINARY_OUTPUT})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MARATIS_BINARY_OUTPUT})

## Set up subdirectories...
ADD_SUBDIRECTORY(3rdparty)
ADD_SUBDIRECTORY(MSDK)
ADD_SUBDIRECTORY(Maratis)
ADD_SUBDIRECTORY(Data)
ADD_SUBDIRECTORY(Documentation)
ADD_SUBDIRECTORY(Tests/LuaApi)

## Install files
INSTALL(FILES 
  CREDITS
  LICENSE
  README.md
  DESTINATION ${MARATIS_INSTALL_DIR})
