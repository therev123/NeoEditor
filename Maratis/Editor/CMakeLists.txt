## Find source and headers
FILE(GLOB_RECURSE HEADERS *.h)
FILE(GLOB_RECURSE SOURCES *.c*)

IF(CMAKE_HAS_SORT)
  LIST(SORT HEADERS)
  LIST(SORT SOURCES)
ENDIF(CMAKE_HAS_SORT)

## Build
ADD_DEFINITIONS(-DM_PACKAGE_WRITABLE)

INCLUDE_DIRECTORIES(
  ## Maratis include directories
  ${MARATIS_COMMON_INCLUDE_DIR}
  ${MCORE_INCLUDE_DIR}
  ${MENGINE_INCLUDE_DIR}
  ${MGUI_INCLUDE_DIR}

  ## 3rdparty include directories
  ${ASSIMP_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIR}
  ${FREETYPE_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIR}
  ${JPEG_INCLUDE_DIR}
  ${PNG_INCLUDE_DIR}
  ${LUA_INCLUDE_DIR}
  ${NPK_INCLUDE_DIR}
  ${SDL_INCLUDE_DIR}
  ${OPENAL_INCLUDE_DIR}
  ${TINYXML_INCLUDE_DIR}
  ${ZLIB_INCLUDE_DIR}
  ${RAKNET_INCLUDE_DIR}
)

SET(CMAKE_INSTALL_RPATH "@loader_path/.")
ADD_EXECUTABLE(MaratisEditor ${SOURCES} ${HEADERS})

TARGET_LINK_LIBRARIES(MaratisEditor 
  ## Maratis libraries
  ${MARATIS_COMMON_LIBRARIES}
  ${MCORE_LIBRARIES}
  ${MENGINE_LIBRARIES}
  ${MGUI_LIBRARIES}

  ## 3rdparty libraries
  ${ASSIMP_LIBRARIES}
  ${BULLET_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${JPEG_LIBRARIES}
  ${PNG_LIBRARIES}
  ${VORBIS_LIBRARIES}
  ${OGG_LIBRARIES}
  ${LUA_LIBRARIES}
  ${NPK_LIBRARIES}
  ${SDL_LIBRARIES}
  ${TINYXML_LIBRARIES}
  ${OPENAL_LIBRARIES}
  ${ZLIB_LIBRARIES}
  ${RAKNET_LIBRARIES}

  ## System libraries
  ${OPENGL_LIBRARIES}
  ${PLATFORM_LIBRARIES}
)

INSTALL(TARGETS MaratisEditor DESTINATION ${MARATIS_INSTALL_DIR})
