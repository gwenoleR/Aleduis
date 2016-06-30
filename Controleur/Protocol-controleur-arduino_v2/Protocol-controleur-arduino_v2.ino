#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library

LiquidCrystal_I2C  lcd(0x20,4,5,6,0,1,2,3);

//===============Def contantes===============
const String keyArray[] = {"POTIONS","DEPLACEMENT","START","STOP","WAIT","INFO","POTIONS"};
//Tableau de matching qui servira pour le "switch case"
const char terminatorSignal='\0'; //Signal de fin de commande dans la lecture du Serial
const char masterSeparator=';'; //Séparateur principale d'une commande
const char slaveSeparator=':'; //Séparateur secondaire d'une commande
const String startSignal="START";

const int buttonUp    = 10;
const int buttonDown  = 9;
const int buttonLeft  = 11;
const int buttonRight = 8;
const int buttonMenu  = 12;

//===============Def variables globales===============
String keyWord; //Le mot cle (correspond a la commande reçu)
String reading; //La ligne lu dans le Serial
int indexOfKeyArray; //L'index correspondant au matching pour le "switch case"
//tester si je suis obliger de leurs atribuer une valeur de basse ou non.
int stateUp   = 0, stateDown = 0, stateLeft = 0, stateRight= 0, stateMenu = 0;
int menu      = 0, menuOld   = 0;
int state     = 0, stateOld  = 0;
int playOrNot = 0;
int touchepresse = 0;

byte customChar_Top[8]     = {0b00000,0b00100,0b00100,0b01110,0b01110,0b11111,0b11111,0b00000};
  
byte customChar_Bottom[8]  = {0b00000,0b11111,0b11111,0b01110,0b01110,0b00100,0b00100,0b00000};
  
byte customChar_Right[8]   = {0b00000,0b10000,0b11000,0b11100,0b11110,0b11100,0b11000,0b10000};
  
byte customChar_Left[8]    = {0b00000,0b00001,0b00011,0b00111,0b01111,0b00111,0b00011,0b00001};
  
byte customChar_Full[8]    = {0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};



//Fonction qui recupere le mot cle de la commande
String getKeyWord(String readed){
  if(readed.indexOf(masterSeparator)>=-2){ //On verifie si il exite au moins un separateur
    String theWord=readed.substring(0,readed.indexOf(masterSeparator));
    //on cherche la premiere occurence du ";" qui correcpond a la fin du mot cle
    return theWord;
  }
}

//fonction qui fait le matching entre le mot cle et un index (pour le "switch case")
int findTheWord(String theWord){
  int pos=-1; //Variable qui stockera l'index
  for(int i=0; i<5; i++){ //On parcourt le tableau de matching
    if(theWord==keyArray[i]){ //Si le mot match dans le tableau
      pos=i; //On enregistre son index
    }
  }
  return pos+1;
}

//Fonction de convertion d'un String en char* (tres pratique mais inutilise dans cette version)
/**char* strToChar(String s) {
    unsigned int bufSize = s.length() + 1; //String length + null terminator
    char* ret = new char[bufSize];
    s.toCharArray(ret, bufSize);
    return ret;
}**/

//POTIONS;Potion1:12;Potion2:4\0 // Exemple de commande pour la MAJ des potions

/*//Fonction qui rafraichit la liste des potions du joueur
void refreshListPotion(String aString){
  //Serial.println("Refreshing list!"); 
  int nbOfTypeOfPotions=countItemInReadedSerial(aString); //Recuperation du nombre de type de potions differentes
  int i=0; //compteur de boucle
  String nameOfPotions[nbOfTypeOfPotions]; //Tableau qui stockera le nom des potions
  int nbOfPotions[nbOfTypeOfPotions]; //Tableau qui stockera le nombre de potions 
  int numOfPotions[nbOfTypeOfPotions]; //Numero de potion (juste pour l'affichage)
  
  aString=aString.substring(aString.indexOf(masterSeparator)+1); //Nettoyage de la chaine a traiter
  //Boucle de traitement de la commande "P" lue dans le Serial
  while(i < nbOfTypeOfPotions){ //On boucle en fonction du nombre de types de potions trouvees

    nameOfPotions[i]=aString.substring(0, aString.indexOf(slaveSeparator));//On sotck le nom
    aString=aString.substring(aString.indexOf(slaveSeparator)+1); //Nettoyage de la chaine a traiter

    nbOfPotions[i]=aString.substring(0, aString.indexOf(masterSeparator)).toInt(); //On stock le nombre
    aString=aString.substring(aString.indexOf(masterSeparator)+1); //Nettoyage de la chaine a traiter
    
    numOfPotions[i]=i+1; //On genère un numero de position a partir du compteur de boucle
    
    i++; //On incremente le compteur
  }
  printArray(nameOfPotions, nbOfPotions, numOfPotions, nbOfTypeOfPotions); //debug : affichage de la liste des potions
  printArrayEcran(nameOfPotions, nbOfPotions, numOfPotions, nbOfTypeOfPotions);
}*/

//Fonction qui rafraichit la liste des potions du joueur
void refreshListPotion(String aString){
  //Serial.println("Refreshing list!"); 
  int nbOfTypeOfPotions=countItemInReadedSerial(aString); //Recuperation du nombre de type de potions differentes
  int i=0; //compteur de boucle
  String nameOfPotions[nbOfTypeOfPotions]; //Tableau qui stockera le nom des potions
  int nbOfPotions[nbOfTypeOfPotions]; //Tableau qui stockera le nombre de potions 
  int numOfPotions[nbOfTypeOfPotions]; //Numero de potion (juste pour l'affichage)
  
  aString=aString.substring(aString.indexOf(masterSeparator)+1); //Nettoyage de la chaine a traiter
  //Boucle de traitement de la commande "P" lue dans le Serial
  while(i < nbOfTypeOfPotions){ //On boucle en fonction du nombre de types de potions trouvees

    nameOfPotions[i]=aString.substring(0, aString.indexOf(slaveSeparator));//On sotck le nom
    aString=aString.substring(aString.indexOf(slaveSeparator)+1); //Nettoyage de la chaine a traiter

    nbOfPotions[i]=aString.substring(0, aString.indexOf(masterSeparator)).toInt(); //On stock le nombre
    aString=aString.substring(aString.indexOf(masterSeparator)+1); //Nettoyage de la chaine a traiter
    
    numOfPotions[i]=i+1; //On genère un numero de position a partir du compteur de boucle
    
    i++; //On incremente le compteur
  }
  printArray(nameOfPotions, nbOfPotions, numOfPotions, nbOfTypeOfPotions); //debug : affichage de la liste des potions
  printArrayEcran(nameOfPotions, nbOfPotions, numOfPotions, nbOfTypeOfPotions);
}


//Fonction qui compte de nombre de parametres d'une commande (sert pour compter le nombre de potions differentes)
int countItemInReadedSerial(String aStringToProcess) {
  int count=0; //Compteur de potions
  int i=0; //Compteur de boucle
  while(i < aStringToProcess.length()){ //Tant que le compteur ne depace pas la taille de la chaine
    if(aStringToProcess[i]==masterSeparator){ //Si on trouve un ";" 
      count++; //On incremente le compteur de potions
    }
    i++; //On incremente le compteur de boucle
  }
  return count;
}



//Fonction de chaque debut de tour
void startTheTurn(){playOrNot=0;lcdMessage("    C'est a","   votre tour");}

//Fonction de fin de partie
void stopTheGame(){playOrNot=1;lcdMessage("    BYE BYE","");}

//Fonction de fin de tour
void waitYourTurn(){playOrNot=1;lcdMessage("C'est pas encore","   votre tour");}

//Fonction d'affichage information
void infoTheGame(String messageInfo){
  Serial.print(messageInfo);
  lcdMessage("Message d'Info",messageInfo);
}

//Fonction d'affichage des la liste des potions dans le Serial
void printArray(String stringArray[], int nbArray[], int numArray[], int nbOfPotions){
  for(int i=0; i<nbOfPotions; i++){
    Serial.print(numArray[i]);
    Serial.print(". ");
    Serial.print(stringArray[i]);
    Serial.print("  x");
    Serial.println(nbArray[i]);
  }
}

//Fonction d'affichage des la liste des potions a l'écran
void printArrayEcran(String stringArray[], int nbArray[], int numArray[], int nbOfPotions){
  for(int i=0; i<nbOfPotions; i++){
    lcd.print(numArray[i]);
    lcd.print(". ");
    lcd.print(stringArray[i]);
    lcd.print("  x");
    lcd.println(nbArray[i]);
  }
}

//Fonction permetant l'envoie de la direction a l'IHM
void sendDirectionToIHM(int numButton){
  String stringToSend="DEPLACEMENT;"+String(numButton)+"\0"; //Creation de la chaine a envoyer
  Serial.println(stringToSend);
}

//Fonction permetant l'envoie de la potion a utilise
void sendUsedPotionToIHM(String nameOfPotion){
  String stringToSend="POTION;"+String(nameOfPotion)+"\0"; //Creation de la chaine a envoyer
  Serial.println(stringToSend);
}


//===================MENU===================
void start() {
  lcd.setCursor(0,0);
  lcd.print("   DEMMARAGE");
  lcd.setCursor(0,1); 
  for (int i=0; i <16; i++){
      lcd.write((uint8_t)4);
      delay(100);
   } 
}

// ma fonction de message lcd pour LES MENUS
void lcdMessMenu(String message_Down){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write((uint8_t)0);
  lcd.print(" valider avec ");
  lcd.write((uint8_t)2);
  lcd.setCursor(0,1);
  lcd.write((uint8_t)1);
  lcd.print(message_Down);
}

// ma fonction de message lcd pour TOUT L ECRAN
void lcdMessage(String message_Up,String message_Down){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(message_Up);
  lcd.setCursor(0,1);
  lcd.print(message_Down);
}

int TouchePresse(){
  touchepresse=0;
    if (stateMenu == HIGH || stateUp == HIGH || stateDown == HIGH || stateLeft == HIGH || stateRight == HIGH ){
      touchepresse =1;
      return touchepresse;
    }
}


//=============== INIT ===============
void setup() {
  //Initialisation de l'arduino
  Serial.begin(9600);
   pinMode(buttonUp,   INPUT);
   pinMode(buttonDown, INPUT);
   pinMode(buttonLeft, INPUT);
   pinMode(buttonRight,INPUT);
   pinMode(buttonMenu, INPUT);
   lcd.createChar(0, customChar_Top);
   lcd.createChar(1, customChar_Bottom);
   lcd.createChar(2, customChar_Right);
   lcd.createChar(3, customChar_Left);
   lcd.createChar(4, customChar_Full);
   lcd.begin (16,2);
   start();
   lcd.setCursor(0,0);
   waitYourTurn(); // INIT qui fait que l'on attend que le jeux demmmare pour jouer

}

//=============== MAIN ===============
void loop() {
  stateUp=   digitalRead(buttonUp   );
  stateDown= digitalRead(buttonDown );
  stateLeft= digitalRead(buttonLeft );
  stateRight=digitalRead(buttonRight);
  stateMenu= digitalRead(buttonMenu );

if (playOrNot==0){
  if (stateMenu == HIGH)  {
    lcdMessMenu("  DEPLACEMENT");
    }
    
  if (menu==0){
    if (stateMenu == HIGH)    {menu = 0;}
    if (stateUp == HIGH)    {state += 1;}
    if (stateDown == HIGH)  {state -= 1;}
    if (stateRight == HIGH && state==1)  {menu = 1;}
    if (stateRight == HIGH && state==2)  {menu = 2;}
    if (state>2 ){state=1;}
    else if (state<1 ){state=2;}
     
    if (state != stateOld){    
       if (state == 1) {lcdMessMenu("  DEPLACEMENT");}
       else if (state == 2) {lcdMessMenu("   POTION");}
       else {lcdMessage("  ERREUR MENU","    PRICIPAL");}
       stateOld = state;
      }
    }
  
  else if (menu ==1){
    lcdMessage("Quitte avec menu"," ou Deplace toi");
    if (stateMenu  == HIGH)   {menu = 0;}
    if (stateUp    == HIGH)   {sendDirectionToIHM(1);stateOld=state;}
    if (stateDown  == HIGH)   {sendDirectionToIHM(2);stateOld=state;}
    if (stateRight == HIGH)   {sendDirectionToIHM(4);stateOld=state;}
    if (stateLeft  == HIGH)   {sendDirectionToIHM(3);stateOld=state;}
  
  }

  else if (menu ==2){
      
    lcdMessMenu("  MENU POTION");
//    if (stateMenu == HIGH)    {menu = 0;}
//    if (stateUp == HIGH)    {state += 1;}
//    if (stateDown == HIGH)  {state -= 1;}
//    if (stateRight == HIGH && state==1)  {}
//    if (stateRight == HIGH && state==2)  {}
//    if (state>2 ){state=1;}
//    else if (state<1 ){state=2;}
    
  }
  
  else {lcdMessage(" ERREUR MENU"," ERREUR MENU");}
  
  }
  
  // ================ CODE DE VAL================
  reading=Serial.readString();//Lecture du Serial
  //Si le serial n'est pas vide, on commance le traitement
  if(reading!=""){
    keyWord=getKeyWord(reading); //On recupere le mot cle pour savoir la commande a executer
    
    //Important pour le "switch case"
    indexOfKeyArray=findTheWord(keyWord); //Matching de la commande avec le tableau de definition
    if(indexOfKeyArray!=-1){ //Si le mot a ete trouve alors on continue le traitement
      //Selon la commande reçu, on effectue les actions suivante    
      switch (indexOfKeyArray) {
        case 1: //Mise a jour de la liste des potions
          refreshListPotion(reading); 
          break;
        case 3: //Commencement du tour du joueur
          startTheTurn(); 
          break; 
        case 4: //Arrete le jeu en cas de fin de partie
          stopTheGame();
          break; 
        case 5: //Met en pause le programme jusqu'au prochain tour du joueur
          waitYourTurn(); 
          break; 
        case 6: //Demmare une partie et affiche des infos
          infoTheGame("bonne partie");
          break;
        default: 
          break;
      }
    }
    delay(5);
  }
  
  else if (playOrNot==1){
    if (touchepresse == 1){
      lcdMessage("Les boutons sont","DESACTIVE !  XD");
      delay (2500);
    }
  }
  
  /*else { ERREUR je sais pas pourquoi
  lcdMessage("ERREUR: DROIT"," DE JOUER :-(");
  }*/
  //Serial.print(playOrNot);
}
