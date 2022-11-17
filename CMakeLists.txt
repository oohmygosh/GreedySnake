cmake_minimum_required(VERSION 3.22)
project(GreedySnake)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_EXE_LINKER_FLAGS "-static")
set(EasyX_DIR ./EasyX) # 指定下载的easyx4mingw存放位置
include_directories(${EasyX_DIR}/include)
link_directories(${EasyX_DIR}/lib)

link_libraries(libEasyX.a libgdi32.a libole32.a) # 链接EasyX库

add_executable(GreedySnake main.cpp Map.cpp Map.h Snake.cpp Snake.h Game.cpp Game.h AStar.cpp AStar.h EasyX/Include/easyx.h EasyX/Include/graphics.h)
target_link_libraries (GreedySnake -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)