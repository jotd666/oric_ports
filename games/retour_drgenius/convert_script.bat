tap2bas.py -i retourgenius.tap -o retourgenius.bas

bas2c.py -l -m -i retourgenius.bas -o basic
bin2c.py -i retour.bin -o asm -c all -e basic\asm_entrypoints.txt
pause
