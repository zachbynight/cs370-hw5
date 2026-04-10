MAIN_FILES=Scheduler.c
MAIN_OUT_FILE=Scheduler

FILES_TO_ZIP=${MAIN_FILES} README.md Makefile
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