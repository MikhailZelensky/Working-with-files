make
python3 script.py
./program fileA fileB
./program fileA fileD 100
stat fileA fileB fileD
gzip fileA fileB
gzip -cd fileB.gz | ./program fileC
stat fileA.gz fileB.gz fileC
