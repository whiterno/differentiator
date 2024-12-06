Headers := include/differentiator.h include/read_file.h include/utils.h include/simplification.h include/node_utils.h include/parsing.h
Objects := obj/main.o obj/differentiator.o obj/read_file.o obj/utils.o obj/bin_exp_tree.o obj/dump_bet.o obj/simplification.o obj/node_utils.o obj/parsing.o
Flags   := -I include -I lib/bin_exp_tree/include -I lib/stack/stack_hcpp

main.out: ${Objects} $(Headers)
	g++ ${Flags} ${Objects} -o main.out

obj/main.o: src/main.cpp ${Headers}
	g++ ${Flags} -c src/main.cpp -o obj/main.o

obj/differentiator.o: src/differentiator.cpp ${Headers}
	g++ ${Flags}  -c src/differentiator.cpp -o obj/differentiator.o

obj/read_file.o: src/read_file.cpp ${Headers}
	g++ ${Flags}  -c src/read_file.cpp -o obj/read_file.o

obj/utils.o: src/utils.cpp ${Headers}
	g++ ${Flags}  -c src/utils.cpp -o obj/utils.o

obj/simplification.o: src/simplification.cpp ${Headers}
	g++ ${Flags}  -c src/simplification.cpp -o obj/simplification.o

obj/node_utils.o: src/node_utils.cpp ${Headers}
	g++ ${Flags}  -c src/node_utils.cpp -o obj/node_utils.o

obj/parsing.o: src/parsing.cpp ${Headers}
	g++ ${Flags}  -c src/parsing.cpp -o obj/parsing.o

library:
	g++ -c lib/bin_exp_tree/src/bin_exp_tree.cpp -o obj/bin_exp_tree.o
	g++ -c lib/bin_exp_tree/src/dump_bet.cpp -o obj/dump_bet.o

run: main.out
	./main.out
