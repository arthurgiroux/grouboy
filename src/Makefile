all: gbemulator

gbemulator: cpu.o mmu.o emulator.o main.o
	g++ -o ../build/gbemulator ../build/cpu.o ../build/mmu.o ../build/emulator.o ../build/main.o

mmu.o: mmu.cpp mmu.hpp
	g++ -o ../build/mmu.o -c mmu.cpp -W -Wall -ansi -pedantic

cpu.o: cpu.cpp cpu.hpp mmu.hpp
	g++ -o ../build/cpu.o -c cpu.cpp -W -Wall -ansi -pedantic

emulator.o: emulator.cpp emulator.hpp
	g++ -o ../build/emulator.o -c emulator.cpp -W -Wall -ansi -pedantic

main.o: main.cpp emulator.hpp
	g++ -o ../build/main.o -c main.cpp -W -Wall -ansi -pedantic

clean:
	rm -rf ../build/*.o