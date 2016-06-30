#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>
#include <cJSON.h>

// We attempt to translate Points from a JSON representation to this structure

typedef struct {
  char kind[50];
  char skin[9];
} Contains;

typedef struct {
  int x;
  int y;
  int isObstacle; //int
  char skin[9]; //char[8]
  char players[15][100]; //char[15][100]
  Contains contains;
} Map;

typedef enum {
  JSON_TO_BIN,
  BIN_TO_JSON
} ExecutionMode;

// prototypes
void jsonToBin(const char* fin, const char* fout);
void binToJson(const char* fin, const char* fout);


int main(const int argc, const char** argv) {

  ////// Read the input parameters
  const int requiredArgs = 4;
  if (argc != requiredArgs) {
    fprintf(stderr, "Expects 3 params (mode, filepath in, filepath out) but %d given.\n", argc-1);
    exit(1);
  }
  
  const char* modeStr = argv[1];
  ExecutionMode mode;
  const char* fin  = argv[2];
  const char* fout = argv[3];
  
  if (strcmp("json2bin", modeStr) == 0) {
    mode = JSON_TO_BIN;
  } else if (strcmp("bin2json", modeStr) == 0) {
    mode = BIN_TO_JSON;
  } else {
    fprintf(stderr, "The mode parameter must either be 'json2bin' or 'bin2json'.\n");
    exit(2);
  }
  
  if (access(fin, R_OK) != 0) {
    fprintf(stderr, "Cannot read from file at given filepath.\n");
    exit(3);
  }
  if (access(fin, W_OK) != 0) {
    fprintf(stderr, "Cannot write to file at given filepath.\n");
    exit(4);
  }
  /////////
  
  if (mode == JSON_TO_BIN) {
    jsonToBin(fin, fout);
  } else { // mode == BIN_TO_JSON
    binToJson(fin, fout);
  }
  
  printf("Done.\n");
  return 0;
}

char* allocBufferWithFileContents(const char* filepath) {
	FILE* f = fopen(filepath, "rb");
  	if (!f) {
		fprintf(stderr, "File does not exist or lacking permission.\n");
		exit(1);
	}
	  
	// get the file size
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	// malloc => caller is responsible for freeing this memory buffer
	char* buffer = malloc(fsize + 1);
	fread(buffer, fsize, 1, f);
	fclose(f);

	// safety: add '\0' at the end of the buffer
	buffer[fsize] = 0;
	  
	return buffer;
}


void jsonToBin(const char* fin, const char* fout) {
    // decode the JSON
    char* rawJson = allocBufferWithFileContents(fin);
    cJSON * root = cJSON_Parse(rawJson);

    free(rawJson); rawJson = NULL;

    if (!root) {
        fprintf(stderr, "Error decoding JSON.\n");
        exit(1);
    }

    // read the JSON
    Map m;

    // write the binary file
    FILE* f = fopen(fout, "wb");

    int y = cJSON_GetArraySize(root);
	cJSON* mapView = cJSON_GetObjectItem(root, "mapView");
    fwrite(&y, sizeof(int), 1, f);

    for(int i = 0 ; i < cJSON_GetArraySize(mapView); i++){

        cJSON * ligne = cJSON_GetArrayItem(mapView, i);
        int x = cJSON_GetArraySize(ligne);
        fwrite(&x, sizeof(int), 1, f);
        for(int j = 0; j < cJSON_GetArraySize(ligne); j++){
            cJSON * cell = cJSON_GetArrayItem(ligne, j);

			/////////////////////////FONCTIONNEL/////////////////////////////

            m.x = cJSON_GetObjectItem(cell, "x")->valueint;
            m.y = cJSON_GetObjectItem(cell, "y")->valueint;

            m.isObstacle = cJSON_GetObjectItem(cell, "isObstacle")->valueint;

            char charActuel = cJSON_GetObjectItem(cell, "skin")->valuestring[0];
            int k = 0;
            while(charActuel != '\0') {
                charActuel = cJSON_GetObjectItem(cell, "skin")->valuestring[k];
                m.skin[k] = charActuel;
                k++;
            }

			/////////////////////////////////////////////////////////////////

			//PLAYERS

            cJSON* tabPlayers = cJSON_CreateArray();
            tabPlayers = cJSON_GetObjectItem(cell, "players");
			int taille = cJSON_GetArraySize(tabPlayers);
			printf("Taille - %d\n", taille);

	            for (int k = 0; k < cJSON_GetArraySize(tabPlayers); k++) {
                	char charActuel = cJSON_GetArrayItem(tabPlayers, k)->valuestring[0];
                	int l = 0;
		            while(charActuel != '\0') {
		                charActuel = cJSON_GetArrayItem(tabPlayers, k)->valuestring[l];
						printf("%s\n", cJSON_GetArrayItem(tabPlayers, k)->valuestring);
						printf("Caractere actuel - %c\n", charActuel);
						//strcpy(m.players[k][l], charActuel);
						m.players[k][l] = charActuel;
						printf("Caractere dans m.players[k][l] - %c\n", m.players[k][l]);
		                
		                l++;
		            }
            	}

			//CONTENU

            cJSON* contenu = cJSON_GetObjectItem(cell, "contains");

			//KIND

            charActuel = cJSON_GetObjectItem(contenu, "kind")->valuestring[0];
            k = 0;
			//if strlen
			if(strlen(cJSON_GetObjectItem(contenu, "kind")->valuestring) == 0){
				m.contains.kind[0] = '\0';
			}
			else{
		        while(charActuel != '\0') {
		            charActuel = cJSON_GetObjectItem(contenu, "kind")->valuestring[k];
		            m.contains.kind[k] = charActuel;
		            k++;
		        }
			}
			//SKIN
			printf("test - %s\n", cJSON_GetObjectItem(contenu, "skin")->valuestring);
			if(strlen(cJSON_GetObjectItem(contenu, "skin")->valuestring) == 0){
				m.contains.skin[0] = '\0';
			}
			else
			{
				charActuel = cJSON_GetObjectItem(contenu, "skin")->valuestring[0];
				k = 0;
		        while(charActuel != '\0') {
		            charActuel = cJSON_GetObjectItem(contenu, "skin")->valuestring[k];
		            m.contains.kind[k] = charActuel;
		            k++;
		        }
			}
            fwrite(&m, sizeof(Map), 1, f);
        }
    }

    cJSON_Delete(root); root = NULL; // done with root, free the memory

    fclose(f);
}

void binToJson(const char* fin, const char* fout) {
    FILE* fichier = fopen(fin, "rb");

    int y = 0;
    fread(&y, sizeof(int), 1, fichier);
    int x = 0;
    fread(&x, sizeof(int), 1, fichier);
    int total = x * y;
    // create the JSON object
    cJSON* root = cJSON_CreateArray();

    Map* tab2 = malloc((sizeof(Map) * total) + 1);
    fread(tab2, sizeof(Map), total, fichier);

    fclose(fichier);

    for (int i = 0; i < y; i++) {
        cJSON* ligne = cJSON_CreateArray();
        for (int j = (i * x); j < (x + (i * x)); j++) {
            cJSON* mapLigne = cJSON_CreateObject();

            Map m = tab2[j];
            cJSON_AddNumberToObject(mapLigne, "x", m.x);
            cJSON_AddNumberToObject(mapLigne, "y", m.y);

            if(m.isObstacle == 1){
                cJSON_AddStringToObject(mapLigne, "isObstacle", "true");
            }
            else if(m.isObstacle == 0){
                cJSON_AddStringToObject(mapLigne, "isObstacle", "false");
            }

            cJSON_AddStringToObject(mapLigne, "skin", m.skin);


//Manque un bloc
//Créer un array
//String dans array
//While != \0
//Lire tous les mots


//GetArraySize()

//Je n'arrive pas a trouver un moyen pour size le tableau de players

            cJSON* arrayPlayers = cJSON_CreateArray();
			
			cJSON_AddItemToObject(mapLigne, "players", arrayPlayers); //Array de players
			int i = 0;
			int size = cJSON_GetArraySize(arrayPlayers);
			printf("Size de players - %i\n", size);
			while(i != size)
			{
				cJSON_AddStringToObject(arrayPlayers, "players", (char*)m.players[i]);
				printf("players - %s\n", (char*)m.players[i]); 
				i++;
			}

            cJSON* temp = cJSON_CreateObject();
	
            cJSON_AddStringToObject(temp, "kind", m.contains.kind);
            cJSON_AddStringToObject(temp, "skin", m.contains.skin);

            cJSON_AddItemToObject(mapLigne, "contains", temp);

            cJSON_AddItemToArray(ligne, mapLigne);
        }
        cJSON_AddItemToArray(root, ligne);
    }
    //Bloc code commenté dans fichier txt sans nom 1

    // print out
    char* jsonRaw = cJSON_Print(root);

    // write to file
    FILE* f = fopen(fout, "w");
    fprintf(f, "%s", jsonRaw);
    fclose(f);

    // free the memory
    cJSON_Delete(root);
    free(jsonRaw);
    free(tab2);
}


