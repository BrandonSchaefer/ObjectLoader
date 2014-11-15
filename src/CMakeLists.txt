find_package (PkgConfig)

set (OBJ_REQUIRED
     sdl2
     glesv2
    )

pkg_check_modules(OBJ REQUIRED ${OBJ_REQUIRED})

set (OBJ_LOADER_SRC
     main.cpp
     Face.cpp
     Matrix4x4.cpp
     ObjLexer.cpp
     Obj.cpp
     SDLTimer.cpp
     Token.cpp
     Vertex.cpp
    )

set (OBJ_LOADER_HDR
     Face.h
     Matrix4x4.h
     ObjLexer.h
     Obj.h
     SDLTimer.h
     Shaders.h
     Token.h
     Vertex.h
    )

include_directories (${OBJ_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

add_executable (obj_parser ${OBJ_LOADER_SRC} ${OBJ_LOADER_HDR})

target_link_libraries (obj_parser ${OBJ_LIBRARIES} ${OBJ_LDFLAGS})
