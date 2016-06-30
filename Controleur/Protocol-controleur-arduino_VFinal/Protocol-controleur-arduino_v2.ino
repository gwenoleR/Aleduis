#include <Wire.h>   // JE SAIS PAS
#include <LCD.h> // A VERIFIER
#include <LiquidCrystal_I2C.h>  // NewLiquidCrystal library

LiquidCrystal_I2C  lcd(0x20, 4, 5, 6, 0, 1, 2, 3);  // deffinition des pin de brochage de mon écran lcd

//===============Def contantes===============
const String keyArray[] = {"POTIONS", "DEPLACEMENT", "START", "STOP", "WAIT", "INFO", "POTIONS"}; //Tableau de matching qui servira pour le "switch case"
const char terminatorSignal = '\0';  //Signal de fin de commande dans la lecture du Serial
const char masterSeparator  =  ';';  //Séparateur principale d'une commande
const char slaveSeparator   =  ':';  //Séparateur secondaire d'une commande

const int buttonUp    = 10;  // Definition des brochages de Pin de mes boutons
const int buttonDown  = 9;
const int buttonLeft  = 11;
const int buttonRight = 8;
const int buttonMenu  = 12;

//===============Def variables globales===============
String keyWord; //Le mot cle (correspond a la commande reçu)
String reading; //La ligne lu dans le Serial
int indexOfKeyArray; //L'index correspondant au matching pour le "switch case"

//tester si je suis obliger de leurs atribuer une valeur de basse ou non.
//Definition de mes variables 
int stateUp   = 0, stateDown = 0, stateLeft = 0, stateRight = 0, stateMenu = 0; // etat de mes boutons
int menu      = 0, menuOld   = 0; // EST CE QUE JE M'EN SERT
int state     = 0, stateOld  = 0;
int playOrNot = 0;  // Droit de jouer
int tp = 0;  // Touche presse

int const nbOfTypeOfPotionsMAX = 6;   // nombre max de potions PAS TOP
String nameOfPotions[nbOfTypeOfPotionsMAX]; //Tableau qui stockera le nom des potions
int      nbOfPotions[nbOfTypeOfPotionsMAX]; //Tableau qui stockera le nombre de potions
int     numOfPotions[nbOfTypeOfPotionsMAX]; //Numero de potion (juste pour l'affichage)


//========= CHARACTERE SPECIAUX ===========
// FAIRE UN TABLEAU DE TABLEAU ?
//fleche vers le haut
byte customChar_Top[8]     = {0b00000, 0b00100, 0b00100, 0b01110, 0b01110, 0b11111, 0b11111, 0b00000};
//fleche vers le bas
byte customChar_Bottom[8]  = {0b00000, 0b11111, 0b11111, 0b01110, 0b01110, 0b00100, 0b00100, 0b00000};
//fleche vers la droite
byte customChar_Right[8]   = {0b00000, 0b10000, 0b11000, 0b11100, 0b11110, 0b11100, 0b11000, 0b10000};
//fleche vers la gauche (inutile mais prevu en cas d'evolution)
byte customChar_Left[8]    = {0b00000, 0b00001, 0b00011, 0b00111, 0b01111, 0b00111, 0b00011, 0b00001};
//charactere de chargement pour le démarage
byte customChar_Full[8]    = {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};




//Fonction qui recupere le mot cle de la commande
String getKeyWord(String readed) {
  if (readed.indexOf(masterSeparator) >= -2) { //On verifie si il exite au moins un separateur
    String theWord = readed.substring(0, readed.indexOf(masterSeparator));//on cherche la premiere occurence du ";" qui correcpond a la fin du mot cle
    return theWord;
  }
}

//fonction qui fait le matching entre le mot cle et un index (pour le "switch case")
int findTheWord(String theWord) {
  int pos = -1; //Variable qui stockera l'index
  for (int i = 0; i < 6; i++) { //On parcourt le tableau de matching
    if (theWord == keyArray[i]) { //Si le mot match dans le tableau
      pos = i; //On enregistre son index
    }
  }
  return pos + 1;
}


//Fonction qui rafraichit la liste des potions du joueur
void refreshListPotion(String aString) {
  int nbOfTypeOfPotions = countItemInReadedSerial(aString); //Recuperation du nombre de type de potions differentes
  int i = 0; //compteur de boucle
  
  //nbOfTypeOfPotionsMAX = nbOfTypeOfPotions; //A FAIRE
  aString = aString.substring(aString.indexOf(masterSeparator) + 1); //Nettoyage de la chaine a traiter
  
  //Boucle de traitement de la commande "POTIONS" lue dans le Serial
  while (i < nbOfTypeOfPotions) { //On boucle en fonction du nombre de types de potions trouvees

    nameOfPotions[i] = aString.substring(0, aString.indexOf(slaveSeparator)); //On sotck le nom
    aString = aString.substring(aString.indexOf(slaveSeparator) + 1); //Nettoyage de la chaine a traiter
    nbOfPotions[i] = aString.substring(0, aString.indexOf(masterSeparator)).toInt(); //On stock le nombre
    aString = aString.substring(aString.indexOf(masterSeparator) + 1); //Nettoyage de la chaine a traiter
    numOfPotions[i] = i + 1; //On genère un numero de position a partir du compteur de boucle
    i++; //On incremente le compteur
  } 
}

//Fonction qui compte de nombre de parametres d'une commande (sert pour compter le nombre de potions differentes)
int countItemInReadedSerial(String aStringToProcess) {
  int count = 0; //Compteur de potions
  int i = 0; //Compteur de boucle
  while (i < aStringToProcess.length()) { //Tant que le compteur ne depace pas la taille de la chaine
    if (aStringToProcess[i] == masterSeparator) { //Si on trouve un ";"
      count++; //On incremente le compteur de potions
    }
    i++; //On incremente le compteur de boucle
  }
  return count;
}

//Fonction de chaque debut de tour
void startTheTurn() {
  playOrNot = 0;  //  le joueur a le droit de jouer
  lcdMessage("    C'est a", "   votre tour"); // Message Fixe`
  menu=0;
}

//Fonction de fin de partie
void stopTheGame() {
  playOrNot = 2;  //  le joueur n a plus le droit de jouer
  lcdMessage("    BYE BYE", ""); // Message Fixe
}

//Fonction de fin de tour
void waitYourTurn() {
  playOrNot = 1;  //  le joueur n a plus le droit de jouer
  lcdMessage("C'est pas encore", "   votre tour"); // Message Fixe
}

//Fonction d'affichage information
// A REVOIR CAR LES GUILLEMETS SONT GARDE
void infoTheGame(String messageInfo) {
  int i = 0; //compteur de boucle
  messageInfo = messageInfo.substring(messageInfo.indexOf(masterSeparator) + 1); //Nettoyage de la chaine a traiter
  //Serial.print(messageInfo);
  lcdMessage("Message d'Info", messageInfo);
}

//Fonction d'affichage des la liste des potions a l'écran
void printArrayEcran(int potionNav) {
  lcd.clear();                          // efface l ecran
  lcd.setCursor(0, 0);                  // curseur ligne 0, colone 0
  lcd.print(numOfPotions[potionNav]);   // numero de la potion dans la liste
  lcd.print(". ");
  lcd.print(nameOfPotions[potionNav]);  // nom de la potion 
  lcd.setCursor(0, 1);                  // curseur ligne 1, colone 0
  lcd.print(" Quantite : ");
  lcd.println(nbOfPotions[potionNav]);  // Quantite associe a la potion
  //exemple +----------------+
  //        | 2. magnet      |
  //        | Quantite : 23  |
  //        +----------------+
}

//Fonction permetant l'envoie de la direction a l'IHM
void sendDirectionToIHM(int numButton) {
  String stringToSend = "DEPLACEMENT;" + String(numButton) + "\0"; //Creation de la chaine a envoyer
  Serial.println(stringToSend);
}

//Fonction permetant l'envoie de la potion a utilise
void sendUsedPotionToIHM(String nameOfPotion) {
  String stringToSend = "POTION;" + String(nameOfPotion) + "\0"; //Creation de la chaine a envoyer
  Serial.println(stringToSend); // debugg
}

// affichage du demmarage 
void start() {
  lcd.clear();                          // efface l ecran
  lcd.setCursor(0, 0);                  // curseur ligne 0, colone 0         
  lcd.print("   DEMMARAGE");
  lcd.setCursor(0, 1);                  // curseur ligne 1, colone 0
  for (int i = 0; i < 16; i++) {        // Barre dechargement avec un caractere special sur 16 cases
    lcd.write((uint8_t)4);
    delay(100);
  }
    //exemple +----------------+
    //        |   DEMMARAGE    |
    //        |000000000       |
    //        +----------------+
}

// ma fonction de message lcd pour LES MENUS
void lcdMessMenu(String message_Down) { // affiche la basse fixe de mon menu
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write((uint8_t)0);
  lcd.print(" valider avec ");
  lcd.write((uint8_t)2);
  lcd.setCursor(0, 1);
  lcd.write((uint8_t)1);
  lcd.print(message_Down);
  //affiche +----------------+
  //        |^ valider avec  |
  //        |v               |
  //        +----------------+
}

// ma fonction de message lcd pour TOUT L ECRAN
void lcdMessage(String message_Up, String message_Down) { // sert surout pour les POPUP d alertte
  lcd.clear();              // efface l ecran
  lcd.setCursor(0, 0);      // ligne 0,colone0
  lcd.print(message_Up);    // Mesage du haut (16 caracteres max)
  lcd.setCursor(0, 1);      // ligne 1,colone0
  lcd.print(message_Down);  // Mesage du bas  (16 caracteres max)
}
// test si une touche est presse
int TouchePresse() {

  if (stateMenu == HIGH || stateUp == HIGH || stateDown == HIGH || stateLeft == HIGH || stateRight == HIGH ) {tp = 1;}
  else {tp = 0;}
  return tp;
}

//=============== INIT ===============
void setup() {
  lcd.begin (16, 2);            // initialisation de l ecran en 16 collones, 2 lignes
  Serial.begin(9600);           //Initiallise les bauds de communication 
  Serial.setTimeout(200);       // defini le temps d'attente max pour la lecture du port serie
  pinMode(buttonUp,   INPUT);   // Defini les boutons en entree
  pinMode(buttonDown, INPUT);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonRight, INPUT);
  pinMode(buttonMenu, INPUT);
  lcd.createChar(0, customChar_Top);     // associe les caracteres a un indice
  lcd.createChar(1, customChar_Bottom);
  lcd.createChar(2, customChar_Right);
  lcd.createChar(3, customChar_Left);
  lcd.createChar(4, customChar_Full);
  start();                      // affcihe la barre de chargement du demmarage (sa ne sert a rien mais c est style)
  lcd.setCursor(0, 0);          // curseur ligne 0, colone 0
  waitYourTurn();               // INIT qui fait que l'on attend que le jeux demmmare pour jouer
}

//=============== MAIN ===============
void loop() {
  stateUp =    digitalRead(buttonUp   );   // deffini les états des boutons respectif
  stateDown =  digitalRead(buttonDown );
  stateLeft =  digitalRead(buttonLeft );
  stateRight = digitalRead(buttonRight);
  stateMenu =  digitalRead(buttonMenu );


  // ================ CODE DE VAL================
  reading = Serial.readString(); //Lecture du Serial
  //Si le serial n'est pas vide, on commance le traitement
  if (reading != "") {
    keyWord = getKeyWord(reading); //On recupere le mot cle pour savoir la commande a executer

    //Important pour le "switch case"
    indexOfKeyArray = findTheWord(keyWord); //Matching de la commande avec le tableau de definition
    if (indexOfKeyArray != -1) { //Si le mot a ete trouve alors on continue le traitement
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
        case 6: //Affiche des infos
          infoTheGame(reading);
          break;
        default:
          break;
      }
    }
    delay(5);
  }
  // avec le droit de jour
  if (playOrNot == 0) { 
    if (stateMenu == HIGH)  {lcdMessage("DEPLAECEZ VOUS", "DANS LE MENU");}
    // MENU PRINCIPAL
    if (menu == 0) {  
      if (stateMenu == HIGH)    {menu = 0;}               //pour revenir au menu principal (useless)
      if (stateUp == HIGH)    {state += 1;}               // pour naviguer dans le menu 
      if (stateDown == HIGH)  {state -= 1;}               //idem
      if (stateRight == HIGH && state == 1)  {menu = 1;}  // acceder au menu 1 donc deplacement
      if (stateRight == HIGH && state == 2)  {menu = 2;}  // acceder au menu 2 donc potions
      
      // pour boucler l'incrementation (1,2,1,2,...)
      if (state > 2 ) {state = 1;}
      else if (state < 1 ) {state = 2;}

      if (state != stateOld) {  //si une touche est appuyer (pour eviter de flooder)
        if (state == 1) {lcdMessMenu("  DEPLACEMENT");}     // affiche le nom respectif au menu 
        else if (state == 2) {lcdMessMenu("   POTION");}    //
        else {lcdMessage("  ERREUR MENU", "    PRICIPAL");} // CATCH ERROR
        stateOld = state;
      }
    }
    // MENU DEPLACEMENTS
    else if (menu == 1) {
      lcdMessage("Quitte avec menu", " ou Deplace toi");
      if (stateMenu  == HIGH)   {menu = 0;}   // reviens au menu principal
      if (stateUp    == HIGH)   {sendDirectionToIHM(1);lcdMessage("  Deplacement ", "   Effectue");delay(1500);menu = 0;}  // envoie le deplacement correspondant
      if (stateDown  == HIGH)   {sendDirectionToIHM(2);lcdMessage("  Deplacement ", "   Effectue");delay(1500);menu = 0;}
      if (stateRight == HIGH)   {sendDirectionToIHM(4);lcdMessage("  Deplacement ", "   Effectue");delay(1500);menu = 0;}
      if (stateLeft  == HIGH)   {sendDirectionToIHM(3);lcdMessage("  Deplacement ", "   Effectue");delay(1500);menu = 0;}
    }
    // MENU POTIONS
    else if (menu == 2) {
      lcdMessMenu("  MENU POTION");
      if (stateMenu == HIGH)    {menu = 0;}               //pour revenir au menu principal
      if (stateUp == HIGH)    {state += 1;}               // pour naviguer dans le menu 
      if (stateDown == HIGH)  {state -= 1;}               //idem
      if (stateRight == HIGH)  {
        if (nbOfPotions[state]>0){ // si la quantite de postion est suffisante
          sendUsedPotionToIHM(nameOfPotions[state]);
          //nbOfPotions[state] -= 1;  // on retire une potion 
          }
        else {  // popup de qauntite insufisante
          lcdMessage("    QUANTITE ", "  INSUFISANTE");
          delay(1500);
          }
        }

      if (state > nbOfTypeOfPotionsMAX){state=0;} // boucle sur le nombre max de potions (0.1.2.3.4.5.0.1...)
      if (state < 0){state=nbOfTypeOfPotionsMAX;}
      
      printArrayEcran(state);   // affiche la potion correspondant à la postion de navigation de state
           
    }
    // CATCH ERROR
    else {lcdMessage(" ERREUR MENU", " ERREUR MENU");}
  }

  // pas le droit de jouer en tant que attendre
  else if (playOrNot == 1) {
    if (TouchePresse() == 1) {
      lcdMessage("Les boutons sont", "   DESACTIVE !");
      delay (2000);
      lcdMessage("C'est pas encore", "   votre tour");
    }
  }

  // pas le droit de jouer en tant que stop
  else if (playOrNot == 2) {
    if (TouchePresse() == 1) {
      lcdMessage("Les boutons sont", "   DESACTIVE !");
      delay (2000);
      lcdMessage("    BYE BYE", "");
    }
  }
}
