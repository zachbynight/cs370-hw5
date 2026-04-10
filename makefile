MAIN_FILES=Scheduler.c Instruction.c
MAIN_OUT_FILE=Scheduler.out

FILES_TO_ZIP=${MAIN_FILES} README.md makefile
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
	./${MAIN_OUT_FILE} 24 3 4 5 6

clean_tzz:
	rm -fv -r tzz

fcfs_simple:
	make build
	clear
	./${MAIN_OUT_FILE} 0,0,5,0 1,5,15,0 2,20,10,0

fcfs_big_arrives_first:
	make build
	clear
	./${MAIN_OUT_FILE} "0,0,15,0" "1,1,3,0", "2,2,3,0"

fcfs_big_arrives_last:
	make build
	clear
	./${MAIN_OUT_FILE} "1,0,3,0", "2,1,3,0" "0,2,15,0"

fcfs_test_different_order:
	make build
	clear
	./${MAIN_OUT_FILE} "1,0,3,0" "0,2,15,0" "2,1,3,0"
	./${MAIN_OUT_FILE} "1,0,3,0" "2,1,3,0" "0,2,15,0"