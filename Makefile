CXX=clang++
CXXFLAGS=-std=c++17 -Wall -g -MMD
EXEC=emu
CCFiles=$(wildcard src/*.cpp)
OBJECTS=${CCFiles:.cpp=.o}
DEPENDS=${CCFiles:.cpp=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean debug
clean:
	rm ${EXEC} ${OBJECTS} ${DEPENDS}

debug:
	@echo ${CCFiles}


