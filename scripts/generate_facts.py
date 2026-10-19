"""Convert already generated C output into SWI-Prolog facts (no extra input files to C)."""
import csv
from pathlib import Path
ROOT=Path(__file__).resolve().parent.parent

def atom(s):
    return "'" + str(s).replace('\\','\\\\').replace("'","\\'").replace('\n',' ') + "'"
with (ROOT/'output/knowledge_base.pl').open('w') as f:
    f.write('% Generated from the C program results. Never hand-code rows.\n')
    for c in csv.DictReader((ROOT/'output/teaching_schedule.csv').open()):
        f.write(f"teaches({atom(c['instructor'])},{atom(c['course'])}).\n")
        f.write(f"meeting({atom(c['course'])},{atom(c['days'])},{atom(c['start'])},{atom(c['end'])}).\n")
    for r in csv.DictReader((ROOT/'output/office_hours.csv').open()):
        if r['status']=='provided - no teaching conflict':
            f.write(f"office_hour({atom(r['instructor'])},{atom(r['days'])},{atom(r['start'])},{atom(r['end'])}).\n")
    f.write("who_teaches(Course,Professor) :- teaches(Professor,Course).\n")
print('Generated output/knowledge_base.pl')
