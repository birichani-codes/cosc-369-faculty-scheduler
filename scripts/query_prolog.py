"""Optional: requires local SWI-Prolog and pip install pyswip."""
from pathlib import Path
from pyswip import Prolog
p=Prolog()
p.consult(str(Path(__file__).resolve().parent.parent/'output/knowledge_base.pl'))
for result in p.query("who_teaches('COSC 369',Professor)"):
    print('COSC 369 instructor:',result['Professor'])
for result in p.query("office_hour('Dr. Appolo Tankeh',Days,Start,End)"):
    print('Office hours:',result)
