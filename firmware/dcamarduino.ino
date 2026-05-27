#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

// =====================================
// LCD
// RS, E, D4, D5, D6, D7
// =====================================
LiquidCrystal lcd(22, 23, 24, 25, 26, 27);

// =====================================
// OUTPUT PINS
// =====================================
#define RELAY_PIN 7
#define RED_LED 5
#define GREEN_LED 6
#define BLUE_LED 8
#define BUZZER 12

// =====================================
// RFID
// =====================================
#define RFID_SS 53
#define RFID_RST 49

MFRC522 rfid(RFID_SS, RFID_RST);

// =====================================
// RFID MEMORY BLOCK
// =====================================
byte block = 1;

// =====================================
// RFID KEY
// =====================================
MFRC522::MIFARE_Key key;

String incoming = "";

// =====================================
// LCD READY SCREEN
// =====================================
void showReadyScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("RFID Door Lock");

  lcd.setCursor(0, 1);
  lcd.print("Scan Your Card");
}

// =====================================
// TURN OFF LEDS
// =====================================
void ledsOff() {

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
}

// =====================================
// BUZZER
// =====================================
void beep(int durationMs, int count = 1) {

  for (int i = 0; i < count; i++) {

    tone(BUZZER, 2000);

    delay(durationMs);

    noTone(BUZZER);

    delay(70);
  }
}

// =====================================
// READ NAME FROM RFID CARD
// =====================================
String readCardName() {

  MFRC522::StatusCode status;

  byte buffer[18];

  byte size = sizeof(buffer);

  // Authenticate
  status = rfid.PCD_Authenticate(
    MFRC522::PICC_CMD_MF_AUTH_KEY_A,
    block,
    &key,
    &(rfid.uid)
  );

  if (status != MFRC522::STATUS_OK) {

    Serial.print("AUTH FAILED: ");

    Serial.println(rfid.GetStatusCodeName(status));

    return "";
  }

  // Read block
  status = rfid.MIFARE_Read(block, buffer, &size);

  if (status != MFRC522::STATUS_OK) {

    Serial.print("READ FAILED: ");

    Serial.println(rfid.GetStatusCodeName(status));

    return "";
  }

  // Convert bytes to string
  String name = "";

  for (uint8_t i = 0; i < 16; i++) {

    if (buffer[i] != 32 && buffer[i] != 0) {

      name += (char)buffer[i];
    }
  }

  name.trim();

  return name;
}

// =====================================
// ACCESS APPROVED
// =====================================
void onApproved(String personName) {

  Serial.println("ACCESS APPROVED");

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Access Granted");

  lcd.setCursor(0, 1);
  lcd.print(personName);

  digitalWrite(GREEN_LED, HIGH);

  digitalWrite(RED_LED, LOW);

  digitalWrite(BLUE_LED, LOW);

  beep(80, 2);

  digitalWrite(RELAY_PIN, HIGH);

  delay(3000);

  digitalWrite(RELAY_PIN, LOW);

  ledsOff();

  showReadyScreen();
}

// =====================================
// ACCESS DENIED
// =====================================
void onDenied() {

  Serial.println("ACCESS DENIED");

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Access Denied");

  digitalWrite(RELAY_PIN, LOW);

  for (int i = 0; i < 3; i++) {

    digitalWrite(RED_LED, HIGH);

    beep(180);

    digitalWrite(RED_LED, LOW);

    delay(120);
  }

  ledsOff();

  delay(1500);

  showReadyScreen();
}

// =====================================
// SERIAL COMMANDS
// =====================================
void processCommand(String cmd) {

  cmd.trim();

  cmd.toLowerCase();

  Serial.print("COMMAND RECEIVED: ");

  Serial.println(cmd);

  if (cmd == "ok") {

    onApproved("Remote User");
  }
  else {

    onDenied();
  }
}

// =====================================
// READ SERIAL
// =====================================
void readCommands() {

  while (Serial.available()) {

    char c = Serial.read();

    if (c == '\n') {

      processCommand(incoming);

      incoming = "";
    }
    else {

      incoming += c;
    }
  }
}

// =====================================
// RFID READER
// =====================================
void pollRfid() {

  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Read name from RFID memory
  String personName = readCardName();

  Serial.println();

  Serial.print("CARD NAME: ");

  Serial.println(personName);

  // If name exists -> approved
  if (personName.length() > 0) {

    onApproved(personName);
  }
  else {

    onDenied();
  }

  rfid.PICC_HaltA();

  rfid.PCD_StopCrypto1();

  delay(1000);
}

// =====================================
// SETUP
// =====================================
void setup() {

  pinMode(RELAY_PIN, OUTPUT);

  pinMode(RED_LED, OUTPUT);

  pinMode(GREEN_LED, OUTPUT);

  pinMode(BLUE_LED, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);

  ledsOff();

  Serial.begin(115200);

  // LCD
  lcd.begin(16, 2);

  showReadyScreen();

  // RFID
  SPI.begin();

  rfid.PCD_Init();

  // Default RFID key
  for (byte i = 0; i < 6; i++) {

    key.keyByte[i] = 0xFF;
  }

  Serial.println();

  Serial.println("==================================");

  Serial.println("RFID DOOR LOCK SYSTEM READY");

  Serial.println("==================================");

  Serial.println();

  Serial.println("SCAN CARD...");

  Serial.println();
}

// =====================================
// LOOP
// =====================================
void loop() {

  readCommands();

  pollRfid();
}