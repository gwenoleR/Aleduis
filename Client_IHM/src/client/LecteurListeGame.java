/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package client;

import org.json.JSONArray;
import org.json.JSONObject;

/**
 *
 * @author florian
 */
public class LecteurListeGame {

    private JSONArray arr;
    private int[] id;
    private String[] nomPartie;
    private int taille;

    public LecteurListeGame(String liste) {

        arr = new JSONArray(liste);

        id = new int[arr.length()];
        nomPartie = new String[arr.length()];

        for (int i = 0; i < arr.length(); i++) {
            if (arr.getJSONObject(i).getString("status").equals("OPENED")) {
                id[i] = arr.getJSONObject(i).getInt("id");
                nomPartie[i] = arr.getJSONObject(i).getString("name");
            }

        }
        
        taille = id.length;

    }

    public int getTaille() {
        return taille;
    }

    public int[] getId() {
        return id;
    }

    public String[] getNomPartie() {
        return nomPartie;
    }

}
