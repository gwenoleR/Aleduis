import random
import sys # pour les arguments

# Exemple de ligne de parametrage
#python lab_gen.py "nomDeMaCarte;OPENED;23;6;12;12;32;1;11;12;13;coins;5"

chaine=str(sys.argv[1])
#print chaine.split(";") # DEBUG
name, status, ident, maxplayers, hauteur, largeur, timeout, fovdefault, coins, chest, potion, rule, value=chaine.split(";")

codes=['00010000','00010100','00010101','00010102','00010103','00010200','00010201','00010202','00010203','00010300','00010301','00010302','00010303','00010400','00010500','00010501','00010502','00010503','00020000','00020100','00020101','00020102','00020103','00020200','00020201','00020202','00020203','00020300','00020301','00020302','00020303','00020400','00020500','00020501','00020502','00020503','00030000','00040000','00050000','01000000','01010000','01010100','01010101','01010102','01010103','01010200','01010201','01010202','01010203','01010300','01010301','01010302','01010303','01010400','01010500','01010501','01010502','01010503','01020000','01020100','01020101','01020102','01020103','01020200','01020201','01020202','01020203','01020300','01020301','01020302','01020303','01020400','01020500','01020501','01020502','01020503','01030000','01040000','01050000','02000000','02000001','02000002','02000003','02010000','02010001','02010002','02010003','02010100','02010101','02010102','02010103','02020000','02020001','02020002','02020003','02020100','02020101','02020102','02020103','02020200','02020201','02020202','02020203']

file = open("jsonReady.txt", "w")

file.write('"{\nname":"')
file.write(name)
file.write('",\n"id":')
file.write(ident)
file.write(',\n"status":"')
file.write(status)
file.write('",\n"turn":5,\n"currentPlayer":"tata",\n')
file.write('"players":["tata","toto","titi"],\n"maxPlayers":')
file.write(maxplayers)
file.write('",\n"timeout":')
file.write(timeout)
file.write(',\n"nbItems":{"coins":')
file.write(coins)
file.write(',"chest":')
file.write(chest)
file.write(',"potion":')
file.write(potion)
file.write('},\n"nbItemsLeft":{"coins":5,"potion":7},\n"fovDefault":')
file.write(fovdefault)
file.write(',\n"victoryOn":{"rule":"')
file.write(rule)
file.write('","value":')
file.write(value)
file.write('"},\n"ranking":["titi"],\n')
file.write('"inventory":{"coins":5,"potions":[{"kind":"immunity"},{"kind":"lottery-win"}]},\n')
file.write('"activePotion":null,\n"width":')
file.write(largeur)
file.write(',\n"height":')
file.write(hauteur)
file.write(',\n"mapView" : [')

for i in range(0,int(hauteur)+1):
  for j in range(0,int(largeur)+1):
    file.write('[{"x" : ')
    file.write(str(j))
    file.write(' , "y" : ')
    file.write(str(i))
    file.write(' , "isObstacle" : "')
    randoom=random.choice(codes)
    if randoom[1] == '1':
      file.write('true')
    elif randoom[1] == '0':
      file.write('false')

    file.write('", "skin"   : "')
    file.write(randoom)
    if i ==int(hauteur) and j==int(largeur):
      file.write('","players":[],"contains":{"kind":"","skin":""}}]')
    else:
      file.write('","players":[],"contains":{"kind":"","skin":""}}],')
    file.write('\n]\n}')

file.closed
