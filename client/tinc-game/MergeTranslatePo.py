import subprocess
import os
from pathlib import Path

# Exist solution to merge po files doesn't reserve translated text for unknown reason
# Have tried:
# PoEdit, update from po
# msgcat or msgmerge
# This python script use first po file as header and put all translated msg together
# to improve translate load speed
languageCode = 'zh'

# Clean files
tempOutputFilePath = Path('po/'+languageCode+'/temp.po')
uniqueResultFilePath = Path('po/'+languageCode+'/tinc-game.po')
compiledResultFilePath = Path('po/'+languageCode+'/tinc-game.mo')
if os.path.exists(tempOutputFilePath):
    os.remove(tempOutputFilePath)
if os.path.exists(uniqueResultFilePath):
    os.remove(uniqueResultFilePath)

# Collect file header
firstFileHeaderCollected = False
firstFileHeaderLines = []
for moFile in Path('po/'+languageCode).glob('*.po'):
    if firstFileHeaderCollected:
        break
    if moFile.is_file():
        with open(moFile, 'r', encoding='utf-8') as f:
            for line in f:
                firstFileHeaderLines.append(line)
                if line.startswith('"X-Generator:'):
                    firstFileHeaderCollected = True
                    break
# Collect file content
allLines = []
for moFile in Path('po/'+languageCode).glob('*.po'):
    headerSkipped = False
    if moFile.is_file():
        with open(moFile, 'r', encoding='utf-8') as f:
            for line in f:
                if line.startswith('"X-Generator:'):
                    headerSkipped = True
                    continue
                if headerSkipped:
                    allLines.append(line)
# Save      
with open(tempOutputFilePath, "w", encoding="utf-8") as file:
    firstFileHeaderLines.extend(allLines)
    file.writelines(firstFileHeaderLines)

# Remove duplicate
uniqueResult = subprocess.run(["msguniq", tempOutputFilePath, "-o", uniqueResultFilePath], capture_output=True, text=True)
print(uniqueResult.stdout)
if os.path.exists(tempOutputFilePath):
    os.remove(tempOutputFilePath)

# Clean po editor auto compile
for moFile in Path('po/'+languageCode).glob('*.mo'):
    moFile.unlink()
    print(f"Deleted: {moFile.name}")

# Compile
compileResult = subprocess.run(["msgfmt", "-o", compiledResultFilePath, uniqueResultFilePath], capture_output=True, text=True)
print(compileResult.stdout)
