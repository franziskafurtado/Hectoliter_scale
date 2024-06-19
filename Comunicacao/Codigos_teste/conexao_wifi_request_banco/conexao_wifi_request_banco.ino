#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Kanekinho";
const char* password = "kanekudo";

const char* apiUrl = "https://joaopedrogalera.pythonanywhere.com:80/machineSync";  // Replace with your API endpoint

int cont = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (cont == 0) {
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      Serial.print("Connecting to server: ");
      Serial.println(apiUrl);

      http.begin(client, apiUrl);

      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"uuid\":\"4688356cecb043a78b15e4e101b7fdfd\",\"secret\":\"6KMayg5Vw5PTUb52iMz8OUT7JvSFH21i5PSpdvRFPosfjFoAXSIjFIWrdoj2wyFcG7Lf1q6Z4pnYe8cGW3h0lVL4vCAV23Kl0R26M30SyOqmiy62JcxHqfcSVs58kVdTXggXZnpy5dg1cdiutFb2QACphXSbK6Y970bHwYSVaSgKxcjlDrAatAxYX8BkTxYqsBL8VEytIoZehDGiPLtFHEW2Rg0wWA4meefDBUSRUoW2GAPm1qRY2gPV1iMdxMr0\",\"status\":\"0\"}");

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      String response = http.getString();

      if (httpResponseCode > 0) {
        Serial.println("Response message:");
        Serial.println(response);
      } else {
        Serial.println("Error sending request");
      }
        
      // Free resources
      http.end();

    } else {
      Serial.println("WiFi disconnected!");
    }
  }
  cont = 1;
}
