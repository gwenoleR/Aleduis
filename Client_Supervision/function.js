var server="https://mysterious-sands-47431.herokuapp.com"; //@ server de jeu
//server=".";

var JSONAllGamesData; //Stock le JSON de toutes les parties
var JSONGameData; //Stock le JSON d'une partie
var JSONPlayerData; //Stock le JSON d'un joueur

//fonction pour le lol
function getJSONData(){
	getJSONAllGames();
}

//recupere le JSON de toutes les parties
function getJSONAllGames(){
	$.ajax({
		dataType:"json",
		url: "/games",
        success: function(games){
			JSONAllGamesData=games;
			refreshGamesList();
        }
    });
}

//recupere le JSON d'une game
function getJSONGameData(gameId){
	$.ajax({
		dataType:"json",
		url: "./games/"+gameId,
		success: function(game){
			JSONGameData=game;
			console.log(JSONGameData);
			refreshAllValuesOfGame();
			refreshAllValuesOfRank();
			refreshAllPlayersList();
		} 
    });
}

//actualise l'affichage de la liste des parties
function refreshGamesList(){
	$("#gamesList").empty();
	for(var i=0;i<countGames();i++){
		if(i==0) $("#gamesList").append("<option selected value=\""+JSONAllGamesData[i].id+"\">"+JSONAllGamesData[i].id+". "+JSONAllGamesData[i].name+"</option>");
		else $("#gamesList").append("<option value=\""+JSONAllGamesData[i].id+"\">"+JSONAllGamesData[i].id+". "+JSONAllGamesData[i].name+"</option>");
	}
	getJSONGameData($("#gamesList option:selected").attr("value"));
}

//actualise l'affichage de toutes les parametres d'une partie
function refreshAllValuesOfGame(){
	$("#gameName").text(getGameName());
	$("#gameType").text(getGameType());
	$("#gameMapHeight").text(getGameMapHeight());
	$("#gameMapWidth").text(getGameMapWidth());
	$("#gameMaxPlayers").text(getGameMaxPlayers());
	$("#gameGoldLeft").text(getGameGoldLeft())
	$("#gameGoldTotal").text(getGameGoldTotal())
	$("#gameChestLeft").text(getGameChestLeft())
	$("#gameChestTotal").text(getGameChestTotal())
	$("#gamePotionsLeft").text(getGamePotionsLeft())
	$("#gamePotionsTotal").text(getGamePotionsTotal())
	$("#gameTimeByTurn").text(getGameTimeByTurn())
}

//actualise l'affichage du classement
function refreshAllValuesOfRank(gameId){
	$("#rankArrayUpdatable").empty();
	var rankLine;
	var player;
	for(var i=0;i<countPlayersInGame();i++){
		player = JSONGameData.ranking[i];
		
		rankLine = "<br>\
					<div class=\"aLine\" id=\""+(1+i)+"\">\
						<span class=\"rankPlayer\">"+getPlayerName(player)+"</span>\
						<span class=\"rankScore\">"+getPlayerScore(player)+"</span>\
						<span class=\"rankGold\">"+getPlayerGold(player)+"</span>\
						<span class=\"rankPosition\">"+getPlayerPosition(player)+"</span>\
						<span class=\"rankVision\">"+getPlayerVision(player)+"</span>\
						<span class=\"rankPlace\">"+(1+i)+"</span>\
					</div>";
		$("#rankArrayUpdatable").append(rankLine);
	}
}

//actualise l'affichage de la liste des joueur
function refreshAllPlayersList(){
	$("#playersList").empty();
	var player;
	$("#playersList").append("<option>---</option>");
	for(var i=0;i<countPlayersInGame();i++){
		player = JSONGameData.ranking[i];
		$("#playersList").append("<option>"+player+"</option>");
	}
}

//actualise l'affichage de l'inventaire d'un joueur
function refreshPlayerInventory(playerName){
	$("#playerPotionsList").empty();
	$("#playerCoinsNumber").empty();
	var player=getThePlayer(playerName);
	$("#playerCoinsNumber").text("Pièces : "+player.inventory.coins);
	for(var i=0; i<countPlayerInventoryPotion(player);i++){
		$("#playerPotionsList").append("<li>"+player.inventory.potions[i].kind+"</li>");
	}
}

//compte le nombre de parties
function countGames(){
	return JSONAllGamesData.length;
}

//compte le nombre de joueur dans une partie
function countPlayersInGame(){
	return JSONGameData.ranking.length;
}

//compte le nombre de potion dans l'inventaire d'un joueur
function countPlayerInventoryPotion(thePlayer){
	return thePlayer.inventory.potions.length;
}

//recupere le nom d'une partie
function getGameName(){
	debugLog(JSONGameData.name);
	return JSONGameData.name;
}

//recupere le type de jeu d'une partie
function getGameType(){
	debugLog(JSONGameData.victoryOn.rule);
	if(JSONGameData.victoryOn.value) return JSONGameData.victoryOn.rule + " (" + JSONGameData.victoryOn.value + ")";
	else return JSONGameData.victoryOn.rule;
}

//recupere la hauteur de la carte d'une partie
function getGameMapHeight(){
	debugLog(JSONGameData.height);
	return JSONGameData.height;
}

//recupere la largeur de la carte d'une partie
function getGameMapWidth(){
	debugLog(JSONGameData.width);
	return JSONGameData.width;
}

//recupere le nombre max de joueur d'une partie
function getGameMaxPlayers(){
	debugLog(JSONGameData.maxPlayers);
	return JSONGameData.maxPlayers;
}

//recupere le nombre RESTANT de coffres d'une partie
function getGameGoldLeft(gameId){
	debugLog(JSONGameData.nbItemsLeft.coins);
	return JSONGameData.nbItemsLeft.coins;
}

//recupere le nombre TOTAL de coffres d'une partie
function getGameGoldTotal(gameId){
	debugLog(JSONGameData.nbItems.coins);
	return JSONGameData.nbItems.coins;
}
//recupere le nombre RESTANT de coffres d'une partie
function getGameChestLeft(gameId){
	return "N";
}

//recupere le nombre TOTAL de coffres d'une partie
function getGameChestTotal(gameId){
	debugLog(JSONGameData.nbItems.chest);
	return JSONGameData.nbItems.chest;
}

//recupere le nombre RESTANT de potions d'une partie
function getGamePotionsLeft(gameId){
	debugLog(JSONGameData.nbItemsLeft.potion);
	return JSONGameData.nbItemsLeft.potion;
}

//recupere le nombre TOTAL de potions d'une partie
function getGamePotionsTotal(gameId){
	debugLog(JSONGameData.nbItems.potion);
	return JSONGameData.nbItems.potion;
}

//recupere le parametre timeout d'une partie
function getGameTimeByTurn(gameId){
	debugLog(JSONGameData.timeout);
	return JSONGameData.timeout;
}

//creer une nouvelle partie
$(document).ready(function(){
	$("#createTheGame").click(function(){
		//Récupération des informations du tableau de paramètre de la nouvelle partie
		var name = $("#newGameName").val();
		var width = parseInt($("#newGameMapWidth").val());
		var height = parseInt($("#newGameMapHeight").val());
		var vision = parseInt($("#newGamePlayerVision").val());
		var nbCoins = parseInt($("#newGameGoldNumber").val());
		var nbChests = parseInt($("#newGameChestNumber").val());
		var nbPotions = parseInt($("#newGamePotionNumber").val());
		var timeOut = parseInt($("#newGameTimeByTurn").val());
		var typeGame = $("#newGameType option:selected").text();
		var howMany = parseInt($("#newGameTypeObjectNumber").val());	
		var username = "user";
		var password = "password"; 

		//Renvoie au format JSON
		var rawJSON = JSON.stringify({"name": name, "width": width, "height": height, "maxPlayers": 15,
		"fovDefault": vision, "nbItems": {"coins": nbCoins, "chest": nbChests, "potions": nbPotions},
		"timeout":timeOut, "victoryOn":{ "rule":typeGame, "value":howMany}});
		
		//Bug - Envoie un OPTION
		$.ajax('/games', {
            		method: "POST",
			contentType: 'application/json',
			beforeSend: function (xhr) {
    				xhr.setRequestHeader ("Authorization", "Basic " + btoa(username + ":" + password));
			},
			data: rawJSON,
			success:(function(){
				//Envoie en json
				console.log(rawJSON);
				alert("success");
			}),
			error : function(){
				alert("error");
			}
		});
	});
});

//recupere toute les infos d'un joueur
function getThePlayer(playerName){
	var thePlayer;
	for(var i=0; i<countPlayersInGame();i++){
		if(JSONGameData.players[i].username==playerName) thePlayer=JSONGameData.players[i];
	}
	return thePlayer;
}

//recupere le nom d'un joueur
function getPlayerName(playerName){
	debugLog(getThePlayer(playerName).username);
	return getThePlayer(playerName).username;
}

//recupere le score d'un joueur
function getPlayerScore(playerName){
	debugLog(getThePlayer(playerName).inventory.coins);
	return getThePlayer(playerName).inventory.coins;
}

//recupere le nombre de pieces d'une joueur
function getPlayerGold(playerName){
	debugLog(getThePlayer(playerName).inventory.coins);
	return getThePlayer(playerName).inventory.coins;
}

//recupere la position d'un joueur
function getPlayerPosition(playerName){
	var pos = getThePlayer(playerName).x + "," + getThePlayer(playerName).y;
	debugLog(pos);
	return pos;
}

//recupere le champs de vision d'un joueur
function getPlayerVision(playerName){
	debugLog("Vision "+getThePlayer(playerName).fov);
	return getThePlayer(playerName).fov;
}

//recupere le nombre de pieces d'une joueur
function getPlayerInventoryCoins(playerName){
	debugLog(getThePlayer(playerName).inventory.coins);
	return getThePlayer(playerName).inventory.coins;
}

//recupere les potions d'un joueur
function getPlayerInventoryCoins(playerName){
	debugLog(getThePlayer(playerName).inventory.potion);
	return getThePlayer(playerName).inventory.potion;
}

//Fonction debug : decommenter pour debug
function debugLog(value){
	//console.log(value);
}

function refreshTurn(gameId){
	debugLog(JSONGameData.turn);
	return JSONGameData.turn);
}

//Vérifie le tour toutes les 2 secondes
$(document).ready(function(){
	setInterval('refreshTurn', 2000); //Déclenche la fonction toutes les 2 secondes
});

function printMap(){
	//Je récupère un JSON Array de JSON Array
	//Récupérer morceau par morceau des arrays
	//Récupérer hauteur / largeur de la map
	//For qui parcours dans un premier la largeur et dedans un For qui parcours la hauteur
	//Découper bloc par bloc et après charger l'image qui correspond au bloc
	//Faire un switch case de 400 lignes dégueulasse
	//On récupère le code bloc, on l'analyse et selon la valeur, on arrive a remplir une route dynamiquement
	//Fonction each() qui parcourt un array, du coup on each() dans un each() pour parcourir le bloc dans l'array

	$.each(JSONGameData.mapview, function(i){
		JSONGameData.mapview[i];
		var i = 0;
		var j = 0;
		var tableauImg= [JSONGameData.getGameMapWidth()][JSONGameData.getGameMapHeight()];
		var fichier = "bitmaps/";
		var chemin = "rotated/"

		//Types
		switch(JSONGameDAta.mapview[i].substring(0,2)){
			case '00': // Si c'est 00 c'est franchissable
				fichier.append("franchissable-");
				break;
			case '01': // Si c'est 01 c'est infranchissable
				fichier.append("infranchissable-");
				break;
			case '02': // Si c'est 02 c'est objet
				fichier.append("objet-");
				break;
		}
		//Sous-types
		switch(JSONGameDAta.mapview[i].substring(2,4)){
			case '00': // Si c'est 00 c'est Défaut // Piece
				if(fichier == "objet-"){
					fichier.append("piece");
				} else {
					fichier.append("ground-default.png");
				}
				break;
			case '01': // Si c'est 01 c'est un chemin // Potion
				if(fichier == "objet-"){ 
					fichier.append("potion");
				} else {
					fichier.append("chemin-");
				}
				break;
			case '02': // Si c'est 02 c'est une route // Coffre
				if(fichier == "objet-"){
					fichier.append("coffre");
				} else {
					fichier.append("route-");
				}
				break;
			case '03': // Si c'est 03 c'est Terrain1
				if(fichier == "franchissable-" ||fichier == "infranchissable-"){
					fichier.append("ground-primary");
				}
				break;
			case '04': // Si c'est 04 c'est Terrain2
				if(fichier == "franchissable-" ||fichier == "infranchissable-"){
					fichier.append("ground-secondary");
				}
				break;
			case '05': // Si c'est 05 c'est Terrain Fake
				if(fichier == "franchissable-" ||fichier == "infranchissable-"){
					fichier.append("ground-fake");
				}
				break;
		}
		//Variation
		switch(JSONGameDAta.mapview[i].substring(4,6)){
			case '00': // Si c'est 00 c'est plein // Non réservé pour piece // Potion simple // Coffre 1
				if(fichier == "objet-coffre"){
					fichier.append("1");
				}
				else if (fichier == "franchissable-chemin" || fichier == "franchissable-route" || fichier == "infranchissable-chemin" || fichier == "infranchissable-route"){
					fichier.append("-p");
				}
				break;
			case '01': // Si c'est 01 c'est ligne // Fausse pièce pour potion // Coffre 2
				if(fichier == "objet-coffre"){
					fichier.append("2");
				}else if(fichier == "objet-potion"){
					fichier.append("-fake");
				}
				else if (fichier == "franchissable-chemin" || fichier == "franchissable-route" || fichier == "infranchissable-chemin" || fichier == "infranchissable-route"){
					fichier.append("-l");
				}
				break;
			case '02': // Si c'est 02 c'est coude // Tas de pièces
				if (fichier == "franchissable-chemin" || fichier == "franchissable-route" || fichier == "infranchissable-chemin" || fichier == "infranchissable-route"){
					fichier.append("-v");
				}
				else if ( fichier == "objet-coffre"){
					fichier.append("-pieces");
				}
				break;
			case '03': // Si c'est 03 c'est un T
				if (fichier == "franchissable-chemin" || fichier == "franchissable-route" || fichier == "infranchissable-chemin" || fichier == "infranchissable-route"){
					fichier.append("-t");
				}
				break;
			case '04': // Si c'est 04 c'est un croisement
				if (fichier == "franchissable-chemin" || fichier == "franchissable-route" || fichier == "infranchissable-chemin" || fichier == "infranchissable-route"){
					fichier.append("-x");
				}
				break;
			case '05': // Si c'est 05 c'est une impasse
				if (fichier == "franchissable-chemin" || fichier == "franchissable-route" || fichier == "infranchissable-chemin" || fichier == "infranchissable-route"){
					fichier.append("-i");
				}
				break;
		}
		//Orientation
		switch(JSONGameDAta.mapview[i].substring(6,8)){
			case '00': // Si c'est 00 rotation 0 degré
				fichier.append(".png");
				j=j+1;
				break;
			case '01': // Si c'est 01 rotation 90 degrés
				fichier = chemin + fichier.append("rot090.png");
				j=j+1;
				break;
			case '02': // Si c'est 02 rotation 180 degrés
				fichier = chemin + fichier.append("rot180.png");
				j=j+1;
				break;
			case '03': // Si c'est 03 rotation 270 degrés
				fichier = chemin + fichier.append("rot270.png");
				j=j+1;
				break;		
		}
		tableauImg[i][j] = fichier;
	}
	document.getElementById("mondiv").innerHTML = tableauImg.join();	
}

//De 0 a hauteur x largeur pour parcourir le tableau
//Image a la position x,y du tableau


