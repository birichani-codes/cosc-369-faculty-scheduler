CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2 -Iinclude
AR = ar
OBJS = build/main.o build/scheduler.o
.PHONY: all run report facts test clean
all: scheduler
build:
	mkdir -p build
build/main.o: src/main.c include/scheduler.h | build
	$(CC) $(CFLAGS) -c $< -o $@
build/scheduler.o: src/scheduler.c include/scheduler.h | build
	$(CC) $(CFLAGS) -c $< -o $@
libschedule.a: build/scheduler.o
	$(AR) rcs $@ $^
scheduler: build/main.o libschedule.a
	$(CC) $(CFLAGS) build/main.o -L. -lschedule -o $@
run: scheduler
	mkdir -p output
	./scheduler data/courses.csv data/faculty.csv
facts: run
	python3 scripts/generate_facts.py
report: facts
	python3 scripts/export_report.py
# Runs without SWI-Prolog, which is optional.
test: run
	python3 tests/test_outputs.py
clean:
	rm -rf build scheduler libschedule.a output/*.csv output/*.pl output/*.txt
