#include <RTClib.h>
RTC_DS3231 rtc;

const int pin_interrupt = 2;
long int jumlah_tip = 0;
long int temp_jumlah_tip = 0;
float curah_hujan = 0.00;
float milimeter_per_tip = 0.45;
int buzz = 12;
String hari;
String waktu;
String tanggal;
String jam, menit, detik, dataKirim;
volatile boolean flag = false;

void hitung_curah_hujan() {
  flag = true;
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rtc.begin();
  pinMode(pin_interrupt, INPUT);
  pinMode(buzz, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pin_interrupt), hitung_curah_hujan, FALLING);
  //setting pertama download program
  // rtc.adjust(DateTime(F(_DATE), F(__TIME_)));
  printSerial();
}

void (*funcReset)(void) = 0;

void loop() {
  //baca permintaan dari nodeMCU
  String minta = "";
  while (Serial.available() > 0) {
    minta += char(Serial.read());
  }
  minta.trim();
  kirimData();
}
String tambahangka(String tambah) {
  if (tambah.length() == 1) {
    tambah = "0" + tambah;
  } else {
    tambah = tambah;
  }
  return tambah;
}

void kirimData() {
  if (flag == true) {
    curah_hujan += milimeter_per_tip;
    jumlah_tip++;
    delay(500);
    flag = false;  // reset flag
  }
  curah_hujan = jumlah_tip * milimeter_per_tip;
  if ((jumlah_tip != temp_jumlah_tip)) {
    printSerial();
  }
  temp_jumlah_tip = jumlah_tip;
  DateTime now = rtc.now();
  jam = String(now.hour(), DEC);
  menit = String(now.minute(), DEC);
  detik = String(now.second(), DEC);
  waktu = tambahangka(jam) + ":" + tambahangka(menit) + ":" + tambahangka(detik);
  if (waktu == "07:00:00") {
    funcReset();  // fungsi resett nilai
  }

  dataKirim = String(waktu) + " # " + String(curah_hujan);
}
void printSerial() {
  Serial.print(dataKirim);
  Serial.print('\n');
  delay(1000);
  if (curah_hujan >= 10) {
    digitalWrite(buzz, HIGH);
    delay(50);
    digitalWrite(buzz, LOW);
  }
}