//Def contantes
const String keyArray[] = {"POTIONS","DEPLACEMENT","START","STOP","WAIT","INFO","POTIONS"}; //Tableau de matching qui servira pour le "switch case"
const char terminatorSignal='\0'; //Signal de fin de command dans la lecture du Serial
const char masterSeparator=';'; //Séparateur principale d'une commande
const char slaveSeparator=':'; //Séparateur secondaire d'une commande
const String startSignal="START";

//Def variables globales
String keyWord; //Le mot cle (correspond a la command reçu)
String reading; //La ligne lu dans le Serial
int indexOfKeyArray; //L'index correspondant au matching pour le "switch case"

void setup() {
  //Initialisation de l'arduino
  Serial.begin(9600);
}

void loop() {
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
            
           break;
        default: 
          break;
      }
    }
  }
}

//Fonction qui recupere le mot cle de la commande
String getKeyWord(String readed){
  if(readed.indexOf(masterSeparator)>=0){ //On verifie si il exite au moins un separateur
    String theWord=readed.substring(0,readed.indexOf(masterSeparator)); //on cherche la premiere occurence du ";" qui correcpond a la fin du mot cle
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

//Fonction qui rafraichit la liste des potions du joueur
void refreshListPotion(String aString){
  Serial.println("Refreshing list!"); 
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
void startTheTurn(){
  Serial.println("C'est a votre tour");
}

//Fonction de fin de partie
void stopTheGame(){
  Serial.println("Byebye");
}

//Fonction de fin de tour
void waitYourTurn(){
  Serial.println("C'est pas encore votre tour");
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

