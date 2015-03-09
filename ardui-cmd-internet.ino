/*
  Web Server
 
Le script lorsqu'il reçoit une requete http :
  - analyse celle-ci pour voir si c'est un simple appel ou si c'est pour passer des commandes
 
  Sans parametre, le script renvoi l'état de la patte 2 sous forme 0 ou 1
  Avec parametre, le script cherche la presence du ? puis recherche la chaine R1=0 ou R1=1 pour mettre la patte 3 dans ce meme etat
 
  exemple : http://192.168.0.55?R1=0 provoquera la mise a 0 de la patte 3
 */

#include <SPI.h>
#include <Ethernet.h>

// --- Déclaration des constantes des broches E/S numériques ---
const int LED[7]={2,3,5,6,7,8,9}; // Constante pour les broches
//--- la broche 4 est utilisée par le module ethernet

// variables will change:
int realaisState = 0;         // variable for reading the pushbutton status

// variables globales utiles
String chaineRecue=""; // déclare un string vide global
String chaineAtester="";
int comptChar=0; // variable de comptage des caractères reçus

//------ tableau de String ----
// variables globales de mémorisation de l'état d'une case à cocher "" ou "checked"
String etatCase[7]={ "","","","","","",""};


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,55);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(115200);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  // Avec la patte 2 Lit l'état de marche du chauffe-eau
  // Permet ainsi de savoir si par exemple nous ne sommes pas en ma
  pinMode(2, INPUT_PULLUP);
  // La patte 3 Commande le relais
  pinMode (3,OUTPUT);
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        comptChar=comptChar+1; // incrémente le compteur de caractère reçus
        chaineRecue=chaineRecue+c; // ajoute le caractère reçu au String
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          //-- analyse la présence du ? => présent si appui sur bouton envoi coté client
          // variables pour analyse de la requete reçue
          String chaineAnalyse=""; // crée un string utilisé pour analyser la requete
          int indexChaine=0; // variable index pour parcourir la chaine reçue
          indexChaine=5; // analyse le 6ème caractère
          chaineAnalyse=chaineRecue.substring(indexChaine,indexChaine+1); // extrait le 6ème caractère
          //Si nous sommes en presence d'une requete du genre ?R1=0&R2=1 
          if (chaineAnalyse == "?"){    
            chaineAtester = chaineRecue.substring(6,10);
            Serial.println(chaineAtester);
            if (chaineAtester == "R1=0") {
              Serial.println("Bingo R1=0 je met a zero");
              digitalWrite(3, LOW);
            }
            if (chaineAtester == "R1=1") {
              client.println("Bingo R1=1 je met a 1");
              digitalWrite(3, HIGH);
            }
          } else {
              Serial.println("Il faut lire l etat du relais");
              int sensorVal = digitalRead(2);
              Serial.println(sensorVal);
              client.println("Chauffeau eau = " + sensorVal);
          }
          
          // send a standard http response header
          //client.println("HTTP/1.1 200 OK");
          //client.println("Content-Type: text/html");
          //client.println("Connection: close");  // the connection will be closed after completion of the response
	  //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          //client.println();
          //client.println("<!DOCTYPE HTML>");
          //client.println("<html>");
          //client.println("<br />"); 
          //client.println(chaineRecue);
          //Si un ordre a était reçu alors il y a un ? à la position 6 la requete est du genre http://192.168.0.55?R1=0
          // output the value of each analog input pin
          //for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
          //  int sensorReading = analogRead(analogChannel);
          //  client.print("analog input ");
          //  client.print(analogChannel);
          //  client.print(" is ");
          //  client.print(sensorReading);
          //  client.println("<br />");       
          // }
          //client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
    chaineRecue="";
  }
}

