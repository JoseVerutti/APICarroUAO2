#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "VERUTTY";
const char* password = "1ucianav";
const char* apiUrl = "https://apicarrouao2.onrender.com/car";

void setup() {
  Serial.begin(115200);
  
  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");
}

void postDataToAPI(float velocidad, float latitude, float longitud, float temperatura, float humedad, float altura) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // Configurar la solicitud POST
    http.begin(apiUrl);
    http.addHeader("Content-Type", "application/json");
    
    // Crear el cuerpo de la solicitud JSON con los valores de las variables
    String requestBody = "{\"velocidad\":" + String(velocidad) +
                         ",\"latitude\":" + String(latitude) +
                         ",\"longitud\":" + String(longitud) +
                         ",\"temperatura\":" + String(temperatura) +
                         ",\"humedad\":" + String(humedad) +
                         ",\"altura\":" + String(altura) + "}";
    
    // Realizar la solicitud POST
    int httpResponseCode = http.POST(requestBody);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Respuesta del servidor:");
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.println("Error en la solicitud POST");
    }
    
    // Liberar recursos
    http.end();
  }
}

void loop() {
  // Ejemplo de uso de la función postDataToAPI con valores de variables
  float velocidad = 10.5;
  float latitude = 37.7749;
  float longitud = -122.4194;
  float temperatura = 25.3;
  float humedad = 60.2;
  float altura = 100.0;
  
  postDataToAPI(velocidad, latitude, longitud, temperatura, humedad, altura);
  
  delay(5000); // Esperar 5 segundos antes de realizar la siguiente solicitud
}