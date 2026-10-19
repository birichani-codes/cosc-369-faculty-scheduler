#include "scheduler.h"
#include <stdio.h>
int main(int argc, char **argv) {
    const char *c = argc > 1 ? argv[1] : "data/courses.csv";
    const char *f = argc > 2 ? argv[2] : "data/faculty.csv";
    if (argc != 1 && argc != 3) { fprintf(stderr,"Usage: %s [courses.csv faculty.csv]\n",argv[0]); return 2; }
    return create_reports(c,f,"output") ? 0 : 1;
}
