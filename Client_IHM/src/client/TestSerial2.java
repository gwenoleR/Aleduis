/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package client;

import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Enumeration;
import java.util.Random;

/**
 *
 * @author florian
 */
public class TestSerial2 {
    
    SerialPort serialPort;
   
	
	/**
	* A BufferedReader which will be fed by a InputStreamReader 
	* converting the bytes into characters 
	* making the displayed results codepage independent
	*/
	private BufferedReader input;
	/** The output stream to the port */
	private OutputStream output;
	/** Milliseconds to block while waiting for port open */
	private static final int TIME_OUT = 2000;
	/** Default bits per second for COM port. */
	private static final int DATA_RATE = 9600;

	public void initialize() {
         

		CommPortIdentifier portId = null;
		Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();

		//First, Find an instance of serial port as set in PORT_NAMES.
		while (portEnum.hasMoreElements()) {
			CommPortIdentifier currPortId = (CommPortIdentifier) portEnum.nextElement();
				if (currPortId.getName().equals("COM3")) {// port's name in processing
					portId = currPortId;
					break;
				}
		}
		
                
                
		if (portId == null) {
			System.out.println("Could not find COM port.");
			return;
		}

		try {
			// open serial port, and use class name for the appName.
			serialPort = (SerialPort) portId.open(this.getClass().getName(),
					TIME_OUT);

			// set port parameters
			serialPort.setSerialPortParams(DATA_RATE,
					SerialPort.DATABITS_8,
					SerialPort.STOPBITS_1,
					SerialPort.PARITY_NONE);

			// open the streams
			input = new BufferedReader(new InputStreamReader(serialPort.getInputStream()));
			output = serialPort.getOutputStream();

	
		} catch (Exception e) {
			System.err.println(e.toString());
		}
	}

	/**
	 * This should be called when you stop using the port.
	 * This will prevent port locking on platforms like Linux.
	 */
	public synchronized void close() {
		if (serialPort != null) {
			serialPort.close();
		}
	}
        
        public void envoieMessage(String message) throws Exception{
            
            String str = message;
            
            Thread.sleep(4000);
            output.write(str.getBytes());
            output.flush();
            Thread.sleep(4000);
            
        }
        
        public String receptionMessage()throws Exception{
            String inputLine ="";
            
            Thread.sleep(4000);
            inputLine=input.readLine();
            Thread.sleep(4000);
            
            return inputLine;
        }


	public static void main(String[] args) throws Exception {
		TestSerial2 main = new TestSerial2();
		String inputLine ="";
                main.initialize();
	
		System.out.println("Started");
		String str = "";
		
                Thread.sleep(5000);
                
                str = "INFO;Debut de partie";// START STOP WAIT
                main.output.write(str.getBytes());
                main.output.flush();
                inputLine=main.input.readLine();

                
                Thread.sleep(5000);
                
                main.close();
		System.out.println("Fin");
	}
    
}
