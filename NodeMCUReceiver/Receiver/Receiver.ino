#include <WiFiClient.h>
#include <FirebaseESP32.h>
#include <WiFi.h>

//Pengaturan firebase
#define FIREBASE_HOST "https://datalogger-arg-be6a6-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "MuMwkbjVmzxEVcGouf5RaCOBJA56aBWymg2lFhnI"

//Pengaturan jaringan
#define WIFI_SSID "HYME"
#define WIFI_PASSWORD "00980200"

FirebaseData firebaseData;
FirebaseJson curahhujanarg;
FirebaseJson waktuarg;

unsigned long previousMillis = 0;
const long interval = 1000;

//variable parsing
String arrData[2];


void setup() {
  Serial.begin(9600);
  //Penyambungan jaringan
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP : ");
  Serial.print(WiFi.localIP());

  //Penyambungan Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  Serial.println("-------------------------------------");
  Serial.println("Terhubung");
}
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;
    String data = "";
    while (Serial.available() > 0) {
      data += char(Serial.read());
    }
    data.trim();
    if (data != "") {
      //parsing data
      int index = 0;
      for (int i = 0; i <= data.length(); i++) {
        char delimiter = '#';
        if (data[i] != delimiter) {
          arrData[index] += data[i];
        } else {
          index++;
        }
      }
      if (index == 1) {
        Serial.println(arrData[0]);
        Serial.println(arrData[1]);
        waktuarg.set("waktu/waktu", arrData[0]);
        curahhujanarg.set("data_arg/curah_hujan", arrData[1]);
        Firebase.updateNode(firebaseData, "data_arg", curahhujanarg);
        Firebase.updateNode(firebaseData, "waktu_arg", waktuarg);
      }
      arrData[0] = "";
      arrData[1] = "";
    }
  }
}