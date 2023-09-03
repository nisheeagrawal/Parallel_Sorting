CC=mpic++
GPP=g++
GFLAGS= -g -Wall
CFLAGS= -g -Wall -fopenmp
EE=mpiexec -oversubscribe

# Define output directories
OUT_DIR=bin
NP ?=""
N ?=""
TARGET1= $(OUT_DIR)/MergeQuickSortMPI
TARGET2 = $(OUT_DIR)/MergeRadixSortMPI
TARGET3 = $(OUT_DIR)/MergeSortHybird
TARGET4 = $(OUT_DIR)/MergeSortMPI
TARGET5 =$(OUT_DIR)/QuickSortHybird
TARGET6 =$(OUT_DIR)/RadixSortHybird
TARGET7 =$(OUT_DIR)/Sequential
all: exe1 exe2 exe3 exe4 exe5 exe6 exe7

exe1: Main/MergeQuickSortMPI.cpp
	mkdir -p $(OUT_DIR)
	@echo "Compling $(TARGET2)"
	$(CC) $(CFLAGS) -o bin/MergeQuickSortMPI Main/MergeQuickSortMPI.cpp
exe2: Main/MergeRadixSortMPI.cpp
	@echo "Compling $(TARGET3)"
	$(CC) $(CFLAGS) -o bin/MergeRadixSortMPI Main/MergeRadixSortMPI.cpp
exe3:Main/MergeSortHybird.cpp
	@echo "Compling $(TARGET4)"
	$(CC) $(CFLAGS) -o bin/MergeSortHybird Main/MergeSortHybird.cpp
exe4: Main/MergeSortMPI.cpp
	@echo "Compling $(TARGET5)"
	$(CC) $(CFLAGS) -o bin/MergeSortMPI Main/MergeSortMPI.cpp
exe5: Main/QuickSortHybird.cpp
	@echo "Compling $(TARGET6)"
	$(CC) $(CFLAGS) -o bin/QuickSortHybird Main/QuickSortHybird.cpp
exe6: Main/RadixSortHybird.cpp
	@echo "Compling $(TARGET7)"
	$(CC) $(CFLAGS) -o bin/RadixSortHybird Main/RadixSortHybird.cpp
exe7:
	@echo "Compling $(TARGET1)"
	$(GPP) $(GFLAGS) -o bin/Sequential Main/Sequential.cpp
run:
	@echo "Build successful. Executable: $(TARGET1)"
	${EE} -n ${NP} ${TARGET1} ${N}
	@echo "Build successful. Executable: $(TARGET2)"
	${EE} -n ${NP} ${TARGET2} ${N}
	@echo "Build successful. Executable: $(TARGET3)"
	${EE} -n ${NP} ${TARGET3} ${N}
	@echo "Build successful. Executable: $(TARGET4)"
	${EE} -n ${NP} ${TARGET4} ${N}
	@echo "Build successful. Executable: $(TARGET5)"
	${EE} -n ${NP} ${TARGET5} ${N}
	@echo "Build successful. Executable: $(TARGET6)"
	${EE} -n ${NP} ${TARGET6} ${N}
	@echo "Build successful. Executable: $(TARGET7)"
	${TARGET7} ${N}
	
clean:
	rm -rf bin/