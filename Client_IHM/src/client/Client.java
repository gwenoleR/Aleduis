/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package client;

import java.io.IOException;

/**
 *
 * @author florian
 */
public class Client {

    /**
     * @param args the command line arguments
     * @throws IOException 
     */
    public static void main(String[] args) throws IOException {
        // TODO code application logic here
        
        Connexion c = new Connexion();
        c.setVisible(true);
        c.pack();
        
        /*InterfaceJeux i = new InterfaceJeux();        
        i.setVisible(true);
        i.pack();*/
       
    }
    
}
