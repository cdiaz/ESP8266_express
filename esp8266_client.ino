#include <ESP8266WiFi.h>

const char* ssid = "ssid_name";
const char* password = "ssid_password";

const char* host = "server_url";
const char* token = "QTXDMTXX";

int value = 1;

void setup(){

  //Conectar a la red Wifi..
  Serial.begin(9200);
  Serial.println("Conectando dispositivo: ");
  WiFi.begin(ssid,password);  

  while (WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
    }
    Serial.println();
    Serial.println("Conectado a Red Wi-Fi");
    Serial.println(ssid);
    Serial.println("La IP del dispositivo es: ");
    Serial.println(WiFi.localIP());
}

void loop() {

    delay(5000);
    ++value;
            
    Serial.println("Estableciendo conexión con el servidor");
    Serial.println(host);
    
    WiFiClient client;
    const int httpPort = 3000;
    if (!client.connect(host, httpPort)) {  
      Serial.print("Conexion fallida");
      return;
    }
    
    //Parámetros
     String postStr ="value=";
            postStr += String(value);
            postStr += "&token=";
            postStr += String(token); 
     
     //Petición POST
     Serial.println("Realizando peticion: ");
     client.print("POST /receiver HTTP/1.1\n");
     client.print("Host: ");
     client.println(host);
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr); 

    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.print(">>> Sin respuesta del servidor");
        client.stop();
        return;
        }
      }

      //Respuesta del servidor
      while(client.available()){
        String response = client.readStringUntil('\r');
        Serial.println(response);
        }
  }