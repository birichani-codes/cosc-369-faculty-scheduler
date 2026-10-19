"""Plain-text report; optional documentation export without extra packages."""
from pathlib import Path
import csv
root=Path(__file__).resolve().parent.parent
courses=list(csv.DictReader((root/'output/teaching_schedule.csv').open()))
offices=list(csv.DictReader((root/'output/office_hours.csv').open()))
with (root/'output/schedule_summary.txt').open('w') as f:
 f.write('BOWIE STATE UNIVERSITY - GENERATED SCHEDULE SUMMARY\n\nCLASS SCHEDULE\n')
 for c in courses:f.write(f"{c['course']} | {c['instructor']} | {c['days']} | {c['start']}-{c['end']} | {c['room']} | {c['validation']}\n")
 f.write('\nOFFICE HOURS (only source-confirmed times)\n')
 for r in offices:
  if r['start']:f.write(f"{r['instructor']} | {r['days']} | {r['start']}-{r['end']} | {r['status']}\n")
 f.write('\nOther faculty office hours were not supplied in source documents.\n')
print('Generated output/schedule_summary.txt')
