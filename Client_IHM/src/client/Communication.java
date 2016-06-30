/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Collections;
import org.json.JSONArray;
import org.json.JSONObject;

/**
 *
 * @author florian
 */
public class Communication {
    
    public static String get(String url, String user, String pass) throws IOException {

        String source = "";
        URL oracle = new URL(url);
        URLConnection yc = oracle.openConnection();

        String userpass = user + ":" + pass;
        String basicAuth = "Basic " + javax.xml.bind.DatatypeConverter.printBase64Binary(userpass.getBytes());

        yc.setRequestProperty("Authorization", basicAuth);

        BufferedReader in = new BufferedReader(
                new InputStreamReader(
                        yc.getInputStream()));
        String inputLine;

        while ((inputLine = in.readLine()) != null) {
            source += inputLine;
        }
        in.close();
        return source;
    }

    public static String post(String adress, String user, String pass, String Json) throws IOException {
        String result = "";
        OutputStreamWriter writer = null;
        BufferedReader reader = null;
        try {


//création de la connection
            URL url = new URL(adress);
            URLConnection conn = url.openConnection();
            String userpass = user + ":" + pass;
            System.out.println(userpass);
            System.out.println(Json);
            String basicAuth = "Basic " + javax.xml.bind.DatatypeConverter.printBase64Binary(userpass.getBytes());

            conn.setRequestProperty("Authorization", basicAuth);
            conn.setRequestProperty("Content-Type", "application/json");
            conn.setDoOutput(true);

//envoi de la requête
            writer = new OutputStreamWriter(conn.getOutputStream());
            writer.write(Json);
            writer.flush();

//lecture de la réponse
            reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
            String ligne;
            while ((ligne = reader.readLine()) != null) {
                result += ligne;
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println(e);
        } finally {
            try {
                writer.close();
            } catch (Exception e) {
            }
            try {
                reader.close();
            } catch (Exception e) {
            }
        }
        return result;
    }
    
    public static String postCompte(String adress, String Json) throws IOException {
        String result = "";
        OutputStreamWriter writer = null;
        BufferedReader reader = null;
        try {


//création de la connection
            URL url = new URL(adress);
            URLConnection conn = url.openConnection();
            
            System.out.println(Json);
            
            conn.setRequestProperty("Content-Type", "application/json");
            conn.setDoOutput(true);

//envoi de la requête
            writer = new OutputStreamWriter(conn.getOutputStream());
            writer.write(Json);
            writer.flush();

//lecture de la réponse
            reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
            String ligne;
            while ((ligne = reader.readLine()) != null) {
                result += ligne;
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println(e);
        } finally {
            try {
                writer.close();
            } catch (Exception e) {
            }
            try {
                reader.close();
            } catch (Exception e) {
            }
        }
        return result;
    }
    
    public static void sendTurn(LecteurJson lec, String user, String password, String idgame, TestSerial2 com,String pseudo,String serveur) throws Exception {
        if (lec.getCurrentPlayer().toLowerCase().equals(pseudo.toLowerCase())) {
            String result = "";
            ArrayList potionNonTrier = null;
            JSONArray arr;
            // Récupération et comptage des potions
             if (lec.getInvPotions().length()!=0) {
                
            

             
             ArrayList quantiter = new ArrayList() ;
             ArrayList nomPotion;
             
             
             arr = lec.getInvPotions();
             
             nomPotion = new ArrayList();
             potionNonTrier = new ArrayList();
             for (int i = 0; i < arr.length(); i++) {
             potionNonTrier.add(arr.getJSONObject(i).getString("kind"));   
             nomPotion.add(arr.getJSONObject(i).getString("kind"));
             }
             Collections.sort(nomPotion);

             ArrayList potion = new ArrayList();
             potion.add(0, nomPotion.get(0));
             int j = 0;
             int quantpotion = 0;

             for (int i = 0; i < nomPotion.size(); i++) {

             if (potion.get(j).equals(nomPotion.get(i))) {
             quantpotion++;
             if (quantiter.isEmpty()== false){
                 quantiter.remove(j);
             }
             quantiter.add(j,quantpotion);
             } else {
             quantpotion = 0;
             j++;
             potion.add(j, nomPotion.get(i));
             }

             }
             //Fin récupération et comptage des potions
             String str = "POTIONS;";
             for (int i = 0; i < quantiter.size()-1 ; i++) {
             str = str + potion.get(i) + ":" + quantiter.get(i) + ";";
             }
             str = str + potion.get(quantiter.size()-1) + ":" + quantiter.get(quantiter.size()-1);

            com.envoieMessage(str);
             }
            com.envoieMessage("START");
            String message = com.receptionMessage();

            JSONObject obj = new JSONObject();
            //array.contains("DEPLACEMENT")
            if (message.contains("DEPLACEMENT")) {
                if (message.contains("1")) { //HAUT
                    obj.put("action", "move");
                    obj.put("value", "up");
                } else if (message.contains("2")) { //BAS
                    obj.put("action", "move");
                    obj.put("value", "down");
                } else if (message.contains("3")) { //GAUCHE
                    obj.put("action", "move");
                    obj.put("value", "left");
                } else if (message.contains("4")) { //DROIT
                    obj.put("action", "move");
                    obj.put("value", "right");
                }

            } else {

                int index = message.indexOf(";");
                obj.put("action", "potion");
                obj.put("value",potionNonTrier.indexOf(message.substring(index+1)));
            }

            String jsonText = obj.toString();
            try {
                result = post(serveur+"/game/" + idgame + "/moves", user, password, jsonText);
            } catch (IOException e) {
                result = e.getMessage();
            }

            com.envoieMessage("WAIT");
            System.out.println(result);
        } else {
            com.envoieMessage("WAIT");
        }

    }
    
}
