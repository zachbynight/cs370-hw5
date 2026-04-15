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

test_zip_unzip:
	make clean
	make build
	mkdir tzz
	zip tzz.zip ${ZIP_FILE}
	unzip tzz.zip -d tzz
	unzip tzz/${ZIP_FILE} -d tzz
	cd tzz
	make build

clean_tzz:
	rm -fv -r tzz

big_arrives_first:
	make build
	clear
	./${MAIN_OUT_FILE} "0,0,15,0" "1,1,3,0", "2,2,3,0"

big_arrives_last:
	make build
	clear
	./${MAIN_OUT_FILE} "1,0,3,0", "2,1,3,0" "0,2,15,0"

simple:
	make build
	clear
	./${MAIN_OUT_FILE} 0,0,5,0 1,6,5,0

test_preemptive:
	make build
	clear
	./${MAIN_OUT_FILE} 1,0,8,0   2,1,4,0   3,2,9,0   4,3,5,0

same_arrival_time:
	make build
	clear
	./${MAIN_OUT_FILE} 1,0,6,0 2,0,8,0 3,0,7,0 4,0,3,0