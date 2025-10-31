notePkg: todo.cpp noteVim.hpp frameBuffer.hpp
	g++ todo.cpp -o todo -lncurses
run: todo
	./todo
