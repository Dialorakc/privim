notePkg: todo.cpp noteVim.hpp
	g++ todo.cpp -o todo -lncurses
run: todo
	./todo
