MAIN_FILES=Scheduler.c Instruction.c FCFS.c SJFP.c PtrList.c
HEADER_FILES=Scheduler.h Instruction.h FCFS.h SJFP.h PtrList.h
MAIN_OUT_FILE=Scheduler.out

FILES_TO_ZIP=${MAIN_FILES} ${HEADER_FILES} README.md makefile
ZIP_FILE=Zach-Townsend-HW5.zip


all:
	make build

build:
	gcc ${MAIN_FILES} -o ${MAIN_OUT_FILE}
	make zip

clean:
	rm -fv *.out
	rm -fv *.zip
	make clean_tzz

zip:
	zip ${ZIP_FILE} ${FILES_TO_ZIP}

test_preemptive:
	make build
	clear
	./${MAIN_OUT_FILE} input_files/test_preemptive.csv

test_large:
	make build
	clear
	./${MAIN_OUT_FILE} input_files/large.csv

test_small:
	make build
	clear
	./${MAIN_OUT_FILE} input_files/small.csv

test_big_arrives_first:
	make build
	clear
	./${MAIN_OUT_FILE} input_files/big_arrives_first.csv