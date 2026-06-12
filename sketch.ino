#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

//Portas de Entrada
#define ledAzul 21

#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// =========================
// WIFI
// =========================

const char* ssid = "Wokwi-GUEST";
const char* password = "";

String firebaseUrl = "https://minhaestacaomet-default-rtdb.firebaseio.com";

void enviarFirebase(float temperature, float humidity){
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Estamos offline, conecte a internet.");
    return;
  }

  HTTPClient http;

  String url = firebaseUrl + "/estacao1.json";

  http.begin(url);

  http.addHeader(
    "Content-Type","application/json"
  );

  String json = "{";
  json += "\"temperatura\":"+String(temperature, 1)+",";
  json += "\"umidade\":"+String(humidity, 1)+",";
  json += "\"uptime\":"+String(millis());
  json += "}"; 

  int httpCode = http.PUT(json);

  if(httpCode > 0){
    String response = http.getString();

    Serial.println("Resposta: ");
    Serial.println(response);
  }

  http.end();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(ledAzul, OUTPUT);

  dht.begin();
  WiFi.begin(ssid, password);

  Serial.print("Conectando");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado ao WIFI");
}

void loop() {
  //DHT 22 Data
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  Serial.println("Sensor DHT");
  Serial.print(temperatura);
  Serial.println("°c");

  Serial.print(umidade);
  Serial.println("%");

  enviarFirebase(temperatura, umidade);

  // put your main code here, to run repeatedly:
  digitalWrite(ledAzul, HIGH);
  delay(1000); // this speeds up the simulation
  digitalWrite(ledAzul, HIGH);
  delay(1000); // this speeds up the simulation
}
