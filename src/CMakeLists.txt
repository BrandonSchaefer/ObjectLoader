set (OBJ_LOADER_SRC
     main.cpp
     Token.cpp
     ObjLexer.cpp
    )

set (OBJ_LOADER_HDR
     Token.h
     ObjLexer.h
    )

include_directories (${OBJ_LOADER_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

add_executable (../obj_parser ${OBJ_LOADER_SRC} ${OBJ_LOADER_HDR})
