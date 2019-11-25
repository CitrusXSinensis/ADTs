CXX = g++-5
CXXFLAGS = -g -std=c++14 -Wall -MMD
EXEC = memoPool2
OBJECTS = pool_test_harness.o pool.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}

