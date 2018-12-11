
#This is an example Makefile that will create the executables exampletiming and
#exampleexperiment. This Makefile can either be completed with rules to create
#the executables for your own experiment or the rules here can be added to your
#own Makefile.

#Define your compiler/linker if needed.
#CC=

#Strict options
LDFLAGS=
CFLAGS=-Wextra -Wall -ansi -pedantic -std=c99 -O3
#Lenient: add/remove any compilation or linking flags that you feel are necessary or not.
# LDFLAGS=
# CFLAGS=

DIROK=dirOK.o
#Linux/Unix users can try to comment the above and uncomment the following line instead.
# DIROK=dirOK-linux.o

#Lists all executable, used by the "all" and "clean" rules.
EXECS=exampletiming glowexperiment randomexperiment visualization plot_para

all: $(EXECS)

#Add here any rules needed for your own experiments

#Example experiments:
exampletiming: glowworm.o benchmarksdeclare.o $(DIROK) benchmarkshelper.o benchmarks.o benchmarksnoisy.o fgeneric.o exampletiming.o
	$(CC) $(LDFLAGS) -o $@ $^ -lm
glowexperiment: glowworm.o benchmarksdeclare.o $(DIROK) benchmarkshelper.o benchmarks.o benchmarksnoisy.o fgeneric.o glowexperiment.o
	$(CC) $(LDFLAGS) -o $@ $^ -lm
randomexperiment: random.o benchmarksdeclare.o $(DIROK) benchmarkshelper.o benchmarks.o benchmarksnoisy.o fgeneric.o randomexperiment.o
	$(CC) $(LDFLAGS) -o $@ $^ -lm
visualization: glow_visualizer.o benchmarksdeclare.o $(DIROK) benchmarkshelper.o benchmarks.o benchmarksnoisy.o fgeneric.o glowvis.o
	$(CC) $(LDFLAGS) -o $@ $^ -lm
plot_para: plot_function.o benchmarksdeclare.o $(DIROK) benchmarkshelper.o benchmarks.o benchmarksnoisy.o fgeneric.o
	$(CC) $(LDFLAGS) -o $@ $^ -lm

#Your own experiment should have bbobStructures.h as a dependency and so should be added here.
exampletiming.o exampleexperiment.o benchmarkshelper.o fgeneric.o benchmarks.o benchmarksnoisy.o benchmarksdeclare.o : bbobStructures.h

#Benchmarks object files are optimized so they are exceptions to the implicit *.o generation rule.
benchmarks.o : benchmarks.c
	$(CC) -c $(CFLAGS) -O2 $<

benchmarksnoisy.o : benchmarksnoisy.c
	$(CC) -c $(CFLAGS) -O2 $<

#Clean
.PHONY: clean

#Comment the command starting with rm and uncomment the comment starting with del to make the makefile work under Windows.
clean:
	rm -f *.o $(EXECS)
#	del *.o exampleexperiment.exe exampletiming.exe 

