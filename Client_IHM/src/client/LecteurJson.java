/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package client;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Iterator;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;




/**
 *
 * @author florian
 */
public class LecteurJson {

    private String result = "";
    
    private JSONObject obj;

    private String name;
    private int id;
    private int width;
    private int height;
    private String status;
    private int turn;
    private String currentPlayer;
    private JSONArray players;
    private int maxPlayers;
    private int timeout;

    private int coins;
    private int chest;
    private int potion;

    private int coinsLeft;
    private int potionLeft;

    private int fovDefault;

    private String rule;
    private int value;

    private JSONArray ranking;

    private int invCoins;
    private JSONArray invPotions;

    private int kind;
    private String numberOfTurnsLeft;

    private JSONArray mapView;
    
    

    /**
     * 
     * @param map : chaine JSON a decoder
     */
    public LecteurJson() {
        

    }
    
    public void Lecture(String map){
        //Convertie un fichier en string pour la lectur en Json
        /*try {

            BufferedReader br = new BufferedReader(new FileReader("map13x13.txt"));
            StringBuilder sb = new StringBuilder();
            String line = br.readLine();
            while (line != null) {
                sb.append(line);
                line = br.readLine();
            }
            result = sb.toString();*/
            
            //Map contenue dans le fichier map.txt
            //obj = new JSONObject(result);
            
            //Map recupere du serveur 
            obj = new JSONObject(map);
            
            //Map en dur
            //obj = new JSONObject("{\"name\":\"Testcarte\",\"id\":1,\"width\":3,\"height\":3,\"status\":\"OPENED\",\"turn\":5,\"currentPlayer\":\"tata\",\"players\":[\"tata\",\"toto\",\"titi\"],\"maxPlayers\":5,\"timeout\":30,\"nbItems\":{\"coins\":10,\"chest\":10,\"potion\":10},\"nbItemsLeft\":{\"coins\":5,\"potion\":7},\"fovDefault\":1,\"victoryOn\":{\"rule\":\"coins\",\"value\":5},\"ranking\":[\"titi\"],\"inventory\":{\"coins\":5,\"potions\":[{\"kind\":\"immunity\"},{\"kind\":\"lottery-win\"}]},\"activePotion\":null,\"mapView\":[[{\"x\":0,\"y\":0,\"isObstacle\":false,\"skin\":\"00000000\",\"players\":[],\"contains\":{\"kind\":\"\",\"skin\":\"\"}}],[{\"x\":0,\"y\":1,\"isObstacle\":false,\"skin\":\"00000000\",\"players\":[\"toto\"],\"contains\":{\"kind\":\"\",\"skin\":\"\"}}],[{\"x\":0,\"y\":2,\"isObstacle\":true,\"skin\":\"01000000\",\"players\":[],\"contains\":{\"kind\":\"\",\"skin\":\"\"}}],[{\"x\":1,\"y\":0,\"isObstacle\":false,\"skin\":\"00000000\",\"players\":[],\"contains\":{\"kind\":\"\",\"skin\":\"\"}}],[{\"x\":1,\"y\":1,\"isObstacle\":true,\"skin\":\"01000000\",\"players\":[],\"contains\":{\"kind\":\"\",\"skin\":\"\"}}],[{\"x\":1,\"y\":2,\"isObstacle\":false,\"skin\":\"00000000\",\"players\":[],\"contains\":{\"kind\":\"potion\",\"skin\":\"\"}}],[{\"x\":2,\"y\":0,\"isObstacle\":false,\"skin\":\"00000000\",\"players\":[\"tata\"],\"contains\":{\"kind\":\"\",\"skin\":\"\"}}],[{\"x\":2,\"y\":1,\"isObstacle\":false,\"skin\":\"00000000\",\"players\":[],\"contains\":{\"kind\":\"chest\",\"skin\":\"\"}}],[{\"x\":2,\"y\":2,\"isObstacle\":false,\"skin\":\"00000000\",\"players\":[\"titi\"],\"contains\":{\"kind\":\"\",\"skin\":\"\"}}]]}");

            name = obj.getString("name");
            id = obj.getInt("id");
            width = obj.getInt("width");
            height = obj.getInt("height");
            status = obj.getString("status");
            turn = obj.getInt("turn");
            currentPlayer = obj.getString("currentPlayer");
            players = obj.getJSONArray("players");
            maxPlayers = obj.getInt("maxPlayers");
            timeout = obj.getInt("timeout");

            coins = obj.getJSONObject("nbItems").getInt("coins");
            chest = obj.getJSONObject("nbItems").getInt("chest");
            potion = obj.getJSONObject("nbItems").getInt("potion");

            coinsLeft = obj.getJSONObject("nbItemsLeft").getInt("coins");
            potionLeft = obj.getJSONObject("nbItemsLeft").getInt("potion");

            fovDefault = obj.getInt("fovDefault");

            rule = obj.getJSONObject("victoryOn").getString("rule");
            value = obj.getJSONObject("victoryOn").getInt("value");

            ranking = obj.getJSONArray("ranking");

            invCoins = obj.getJSONObject("inventory").getInt("coins");
            invPotions = obj.getJSONObject("inventory").getJSONArray("potions");

            mapView = obj.getJSONArray("mapView");

        /*} catch (Exception e) {
            System.out.println(e);
        }*/
    }

    public String getName() {
        return name;
    }

    public int getId() {
        return id;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    public String getStatus() {
        return status;
    }

    public int getTurn() {
        return turn;
    }

    public String getCurrentPlayer() {
        return currentPlayer;
    }

    public JSONArray getPlayers() {
        return players;
    }

    public int getMaxPlayers() {
        return maxPlayers;
    }

    public int getTimeout() {
        return timeout;
    }

    public int getCoins() {
        return coins;
    }

    public int getChest() {
        return chest;
    }

    public int getPotion() {
        return potion;
    }

    public int getCoinsLeft() {
        return coinsLeft;
    }

    public int getPotionLeft() {
        return potionLeft;
    }

    public int getFovDefault() {
        return fovDefault;
    }

    public String getRule() {
        return rule;
    }

    public int getValue() {
        return value;
    }

    public JSONArray getRanking() {
        return ranking;
    }

    public int getInvCoins() {
        return invCoins;
    }

    public JSONArray getInvPotions() {
        return invPotions;
    }

    public int getKind() {
        return kind;
    }

    public String getNumberOfTurnsLeft() {
        return numberOfTurnsLeft;
    }

    public JSONArray getMapView() {
        return mapView;
    }

}
