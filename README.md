# COSC 369: Faculty Teaching and Office-Hours Scheduler

Bowie State University | Software Engineering I | Assignment demonstration

## Purpose
A modular **C** application reads **exactly two runtime input files** (`data/courses.csv` and `data/faculty.csv`) and generates `output/teaching_schedule.csv` and `output/office_hours.csv`. The reusable scheduling/CSV-processing implementation is compiled into `libschedule.a`. Optional Python postprocessing generates Prolog facts and a text report from the **generated output**, not additional C inputs.

## Run (Linux or WSL with GCC and make)
```bash
make
make test
make report
```
Or directly `./scheduler data/courses.csv data/faculty.csv` after `make`.

On Windows, install WSL with Ubuntu, then run `sudo apt update && sudo apt install build-essential python3`, navigate to this folder, and execute the commands above.

## Optional Prolog queries
Install SWI-Prolog and Python PySwip (both architectures must match). Run:
```bash
python3 -m pip install pyswip
python3 scripts/query_prolog.py
```
The C build and scheduling tests **do not require Prolog**. `output/knowledge_base.pl` contains generated `teaches`, `meeting`, `office_hour`, and `who_teaches` clauses.

## Input file provenance and limitations
- `courses.csv`: CTEC workbook course rows plus one separately labeled COSC 369 syllabus row. CTEC spreadsheet's instructor header says **Instructor 2025**, not verified current 2026 teaching assignments.
- `faculty.csv`: unique instructor names in the CTEC workbook (no office-hour claims) plus Dr. Appolo Tankeh's stated COSC 369 office hours.
- The supplied project office-hours sample includes Monday/Wednesday examples, but those are **sample formatting**, not a verified faculty input. We do not invent office hours for CTEC instructors.
- The spreadsheet has two asynchronous courses without clock times and some potentially invalid or unusual time entries; the C program flags these for review instead of guessing corrections.
- The syllabus heading mentions Spring 2026 but its weekly teaching schedule lists Fall 2026 dates; this discrepancy should be verified by the instructor.
- Office-hour conflict checks are based on exact textual instructor matches and week-day/time overlap; an office-hours row is only printed as confirmed when the input includes actual times.
- For a production scheduler, use stronger department-identification rules, normalized instructor IDs, and administrative approval before publishing faculty office hours.

## Project structure
```
src/main.c              CLI entrypoint
src/scheduler.c         CSV reading, scheduling, validation, reporting
include/scheduler.h     Public API
libschedule.a           Compiled static library
Makefile                Dependencies, build, reports, tests
scripts/generate_facts.py  Optional C-output-to-Prolog conversion
scripts/query_prolog.py    Optional PySwip queries
scripts/export_report.py  Readable text schedule
output/                Generated schedule CSV, .pl, and text
reports/               PDF assignment report
```

## How it works
1. Open both local CSV files; parse quoted cells correctly.
2. Preserve source rows and mark missing or anomalous meeting times.
3. Write an instructor teaching schedule using the course records.
4. Check provided faculty office-hour slots against every matching teaching block on the same weekdays.
5. Write confirmed slots and explicitly flag unknown office hours.
6. Optionally derive Prolog facts from the generated output and query who teaches a course.

## Compiling a static library
`gcc -c src/scheduler.c -o build/scheduler.o`, then `ar rcs libschedule.a build/scheduler.o`. The main program links it with `-L. -lschedule`. The Makefile runs this automatically and rebuilds modules if prerequisites change.

## Academic integrity
This package is a working reference implementation; review, understand, test, adapt, and acknowledge any external assistance as required by the course syllabus.
# cosc-369-faculty-scheduler
