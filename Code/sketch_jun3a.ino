#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "pitches.h" // ⚠️ Pastikan file pitches.h ada di tab sebelah kodingan ini

// Inisialisasi LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Inisialisasi Servo
Servo servoKran;
const int pinServo = 11; // Pin servo didefinisikan di sini

// Definisi PIN untuk Sensor Ultrasonik
const int pinTrigOrang = 8;
const int pinEchoOrang = 9;
const int pinTrigSampah = 6;
const int pinEchoSampah = 7;

// Definisi PIN untuk Komponen Output
const int pinBuzzer = 5; 
const int pinLED = 4;

// Variabel sudut aman untuk mencegah krek-krek akibat mentok fisik
const int sudutTutup = 15; // Menggantikan sudut 0 agar tidak terlalu mentok
const int sudutBuka  = 150; // Menggantikan sudut 90 (sesuaikan jika kurang membuka)

// Variabel sensor
long durasiOrang, jarakOrang;
long durasiSampah, jarakSampah;

#define REST 0

// Data Nada Rick Roll (Never Gonna Give You Up)
int melody4[] = {   
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, REST, NOTE_D5, REST,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_D5, NOTE_D5, REST, NOTE_C5, REST, NOTE_B4, NOTE_A4, REST,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, REST, NOTE_B4, NOTE_A4, NOTE_G4, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_C5, REST,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, REST, NOTE_D5, REST,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_G5, NOTE_B4, REST, NOTE_C5, REST, NOTE_B4, NOTE_A4, REST,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, REST, NOTE_B4, NOTE_A4, NOTE_G4, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_C5, REST,
  NOTE_C5, REST, NOTE_D5, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_E5, REST, NOTE_G5, NOTE_F5, NOTE_E5, REST,
  NOTE_C5, REST, NOTE_D5, REST, NOTE_G4, REST
};

// Durasi Nada Rick Roll
int durations4[] = { 
  8, 8, 8, 8, 2, 8, 8, 2, 8,
  8, 8, 8, 8, 2, 8, 8, 4, 8, 8, 8, 8,
  8, 8, 8, 8, 2, 8, 8, 2, 8, 4, 8, 8, 8, 8, 8, 1, 4,
  8, 8, 8, 8, 2, 8, 8, 2, 8,
  8, 8, 8, 8, 2, 8, 8, 2, 8, 8, 8, 8,
  8, 8, 8, 8, 2, 8, 8, 4, 8, 3, 8, 8, 8, 8, 8, 1, 4,
  2, 6, 2, 6, 4, 4, 2, 6, 2, 3, 8, 8, 8, 8,
  2, 6, 2, 6, 2, 1
};

void setup() {
  Serial.begin(9600);

  // Mengaktifkan layar LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  SMART BIN  ");
  lcd.setCursor(0, 1);
  lcd.print(" RICKROLL MODE ");
  
  // Menggerakkan servo ke posisi awal awal (tutup) lalu dilepas dayanya
  servoKran.attach(pinServo);
  servoKran.write(sudutTutup); 
  delay(1000);
  servoKran.detach(); 

  delay(1000);
  lcd.clear();

  // Pengaturan jenis Pin
  pinMode(pinTrigOrang, OUTPUT);
  pinMode(pinEchoOrang, INPUT);
  pinMode(pinTrigSampah, OUTPUT);
  pinMode(pinEchoSampah, INPUT);

  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinLED, OUTPUT);
}

// Fungsi khusus untuk memainkan melodi Rick Roll + Kelap-kelip LED
void playRickRoll() {
  int size = sizeof(durations4) / sizeof(int);
  for (int note = 0; note < size; note++) {
    int duration = 1000 / durations4[note];
    
    // Trik Kelap-Kelip: LED berganti status (ON/OFF) setiap berganti nada lagu
    if (note % 2 == 0) {
      digitalWrite(pinLED, HIGH); // Ketukan genap -> LED Menyala
    } else {
      digitalWrite(pinLED, LOW);  // Ketukan ganjil -> LED Mati
    }
    
    if (melody4[note] != REST) {
      tone(pinBuzzer, melody4[note], duration);
    }

    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    noTone(pinBuzzer);
  }
  digitalWrite(pinLED, LOW); // Matikan LED setelah lagu selesai
}

void loop() {
  // 1. MEMBACA SENSOR 1 (ORANG)
  digitalWrite(pinTrigOrang, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrigOrang, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrigOrang, LOW);
  durasiOrang = pulseIn(pinEchoOrang, HIGH);
  jarakOrang = durasiOrang * 0.034 / 2;

  // 2. MEMBACA SENSOR 2 (KAPASITAS SAMPAH)
  digitalWrite(pinTrigSampah, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrigSampah, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrigSampah, LOW);
  durasiSampah = pulseIn(pinEchoSampah, HIGH);
  jarakSampah = durasiSampah * 0.034 / 2;

  // Monitor jarak di laptop
  Serial.print("Orang: "); Serial.print(jarakOrang);
  Serial.print("cm | Sampah: "); Serial.print(jarakSampah); Serial.println("cm");

  // 3. LOGIKA JIKA TONG SAMPAH PENUH (Jarak sampah < 5 cm)
  if (jarakSampah > 0 && jarakSampah < 5) {
    lcd.setCursor(0, 0);
    lcd.print("Status: PENUH!  ");
    lcd.setCursor(0, 1);
    lcd.print("Silakan Buang!  ");
    
    // Pastikan posisi servo menutup penuh, lalu detach agar tidak menyedot daya selama lagu berputar
    servoKran.attach(pinServo);
    servoKran.write(sudutTutup);           
    delay(500); 
    servoKran.detach(); 
    
    // Memutar musik Rick Roll + Mengedipkan LED sampai selesai
    playRickRoll(); 
    
    // JEDA setelah lagu selesai sebelum mengecek kondisi sensor lagi
    delay(5000); 
  } 
  
  // 4. LOGIKA JIKA TONG SAMPAH NORMAL / KOSONG
  else {
    digitalWrite(pinLED, LOW);   
    noTone(pinBuzzer); // Pastikan buzzer mati total
    
    if (jarakOrang > 0 && jarakOrang < 15) {
      lcd.setCursor(0, 0);
      lcd.print("Status: TERBUKA ");
      lcd.setCursor(0, 1);
      lcd.print("Silakan Masuk   ");
      
      servoKran.attach(pinServo);
      servoKran.write(sudutBuka); 
      delay(500); 
      servoKran.detach(); 
      
      delay(2500);        
    } 
    else {
      lcd.setCursor(0, 0);
      lcd.print("Status: STANDBY ");
      lcd.setCursor(0, 1);
      lcd.print("Dekatkan Tangan ");
      
      servoKran.attach(pinServo);
      servoKran.write(sudutTutup); 
      delay(500);
      servoKran.detach(); 
    }
  }
  delay(100); 
}