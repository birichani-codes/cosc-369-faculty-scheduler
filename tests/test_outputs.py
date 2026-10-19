from pathlib import Path
import csv
base=Path(__file__).resolve().parent.parent
cs=list(csv.DictReader((base/'output/teaching_schedule.csv').open()))
fs=list(csv.DictReader((base/'output/office_hours.csv').open()))
assert len(cs)==57, len(cs)
assert sum(c['course']=='COSC 369' for c in cs)==1
assert next(c for c in cs if c['course']=='COSC 369')['start']=='09:30'
assert any(x['instructor']=='Dr. Appolo Tankeh' and x['status']=='provided - no teaching conflict' for x in fs)
assert len([x for x in cs if 'asynchronous' in x['validation']])==2
assert len([x for x in cs if 'review source time' in x['validation']])>=1
print('PASS: course rows, COSC 369 facts, office hours, asynchronous records and anomalous times')
