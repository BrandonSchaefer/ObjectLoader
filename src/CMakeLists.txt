set (OBJ_LOADER_SRC
     main.cpp
     Face.cpp
     ObjLexer.cpp
     Obj.cpp
     Token.cpp
     Vertex.cpp
    )

set (OBJ_LOADER_HDR
     Face.h
     ObjLexer.h
     Obj.h
     Token.h
     Vertex.h
    )

include_directories (${OBJ_LOADER_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

add_executable (obj_parser ${OBJ_LOADER_SRC} ${OBJ_LOADER_HDR})
