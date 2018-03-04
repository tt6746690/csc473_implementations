COMPILER = clang++
CXXFLAGS = -std=c++1z -stdlib=libc++ -Wall

all : algos

algos: Contraction.o
	$(COMPILER) $(CXXFLAGS) -o algos $^

%.o : %.c Graph.h 
	$(COMPILER) $(CXXFLAGS) -c $<


clean:
	rm ./*.o algos log