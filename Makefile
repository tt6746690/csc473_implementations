COMPILER = clang++
CXXFLAGS = -std=c++1z -stdlib=libc++ -Wall
OUTFILES = min_cut

all : $(OUTFILES)

%: %.o
	$(COMPILER) $(CXXFLAGS) -o algos $^

%.o : %.c 
	$(COMPILER) $(CXXFLAGS) -c $<


clean:
	rm ./*.o $(OUTFILES) log