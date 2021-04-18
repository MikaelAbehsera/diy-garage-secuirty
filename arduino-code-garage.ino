#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//Parameters: (rs, enable, d4, d5, d6, d7)
int button = 28;
int buttonColor = 29;
int ledG = 9;
int ledY = 10;
int ledR = 13;
int relay = 7;
const byte ROWS = 4;
const byte COLS = 3;
const int debounceTime = 20; // number of milliseconds for switch to be stable
// keymap defines the character returned when the corresponding key is pressed
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
char passwordList[10][4] = {
  {'1', '2', '3', '4'},
  {'4', '6', '6', '4'},
};

// this array determines the pins used for rows and columns
// 40 41 42 43 44 45 46
// c1 r0 c0 r3 c2 r2 r1
// 41 43 45 46
// r0 r3 r2 r1
// 40 42 44
// c1 c0 c2
byte rowPins[ROWS] = { 41, 46, 45, 43 };
// r0 r1 r2 r3
// 41 46 45 43
byte colPins[COLS] = { 42, 40, 44 };
// c0 c1 c2
// 42 40 44
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1);
uint8_t id;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  pinMode(button, INPUT);
  pinMode(buttonColor, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledY, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(relay, OUTPUT);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");
  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(500);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
  lcd.clear();
  lcd.print("press * to begin");
  Serial.println("press * to begin");
  digitalWrite(buttonColor, LOW);
  digitalWrite(relay, LOW);
}

void loop()
{
  //  char key = keypad.getKey();
  //  //  if (key)
  //  //  {
  //  //    Serial.println(key);
  //  //  }
  //  if (key == '*') {
  //  Serial.print("Key Pressed : ");
  //  Serial.println(key);
  delay(100);
  lcd.clear();
  lcd.print("1 to enter passcode");
  delay(1000);
  lcd.clear();
  lcd.print("2 to scan finger");
  delay(1000);
  lcd.clear();
  lcd.print("3 to scan face");
  delay(1000);
  lcd.clear();
  lcd.print("4 to scan voice");
  Serial.println("press 1 to enter passcode");
  Serial.println("press 2 to scan finger");
  Serial.println("press 3 to scan face");
  Serial.println("press 4 to scan voice");
  //    Serial.println("Enter key: ");
  char key2 = keypad.getKey();
  while (key2 == NO_KEY) key2 = keypad.getKey();
  Serial.println("Enter key: ");
  switch (key2) {
    case '1':
      Serial.print("Key Pressed - : ");
      Serial.println(key2);
      checkPassword();
      break;
    case '2':
      Serial.print("Key Pressed : ");
      Serial.println(key2);
      lcd.clear();
      lcd.print("Scan Fingerprint: ");
      Serial.println("Scan Fingerprint: ");
      delay(500);
      while (checkFingerLocally());
      break;
    case '3':
      Serial.print("Key Pressed : ");
      Serial.println(key2);
      lcd.clear();
      lcd.print("Scan Face: ");
      Serial.println("Scan Face: ");
      delay(2000);
      lcd.clear();
      lcd.print("Face Scanned");
      Serial.println("Face Scanned");
      delay(500);
      lcd.clear();
      lcd.print("Processing");
      Serial.println("Processing");
      delay(3000);
      lcd.clear();
      lcd.print("Welcome Mike");
      Serial.println("Welcome Mike");
      break;
    case '4':
      Serial.print("Key Pressed : ");
      Serial.println(key2);
      lcd.clear();
      lcd.print("Say Voiceline: ");
      Serial.println("Say Voiceline: ");
      delay(4000);
      lcd.print("Voiceline Confirmed");
      Serial.println("Voiceline Confirmed");
      break;
    //      case '5':
    //        Serial.print("Key Pressed : ");
    //        Serial.println(key2);
    //        createNewPassword();
    //        break;
    case '6':
      Serial.print("Key Pressed : ");
      Serial.println(key2);
      enrollFingerLocally();
      break;
  }
  //  }
  delay(2000);

}

void openGarage()
{
  Serial.println("You can open garage now.");
  digitalWrite(buttonColor, HIGH);
  while (true) {
    Serial.println("Garage Door");
    if (digitalRead(button) == HIGH) {
      Serial.println("Garage Door Triggered");
      // activate relay and open/close
      delay(250);
      digitalWrite(relay, LOW);
      delay(250);
      digitalWrite(relay, HIGH);
      delay(250);
      digitalWrite(relay, LOW);
      digitalWrite(buttonColor, LOW);
      break;
    }
    char key = keypad.getKey();
    if (key == '#')
    {
      break;
    }
  }
  digitalWrite(buttonColor, LOW);
  digitalWrite(relay, LOW);

}

//void createNewPassword()
//{
//  if (checkPasswordAdmin()) {
//    char input[4] = {};
//    int num = 0;
//    while (true) {
//      char key = keypad.getKey();
//      input[num] = key;
//      num++;
//      if (key == "*") {
//        break;
//      }
//    }
//    passwords.push_back(input);
//    lcd.clear();
//    lcd.print("New Password Created");
//    Serial.print("New Password Created");
//    delay(1000);
//  }
//}

bool checkPassword()
{
  lcd.clear();
  lcd.print("Enter Passcode: ");
  Serial.println("Enter Passcode: ");

  char input[4] = {};
  int num = 0;
  while (true) {
    char key = keypad.getKey();
    while (key == NO_KEY) key = keypad.getKey();
    Serial.println(key);
    input[num] = key;
    num++;
    if (key == '*' || num == 4) {
      Serial.println("gotta break");
      break;
    }
  }

  Serial.println("im out mf");
  Serial.println(input);


  bool match = false;
  //  for (int password = 0; password <= sizeof(passwordList); password++) {
  for (int password = 0; password <= 1; password++) {
    if (sizeof(passwordList[password]) == 4) {
      for (int index = 0; index <= 3; index++) {
        Serial.print("input: ");
        Serial.println(input[index]);
        Serial.print("reference: ");
        Serial.println(passwordList[password][index]);
        Serial.print("index: ");
        Serial.println(index);
        if (input[index] == passwordList[password][index]) {
          match = true;
        } else {
          match = false;
          break;
        }
      }
    }
    Serial.print("password is? :");
    Serial.println(match ? "Correct" : "Incorrect");
    if (match) {
      openGarage();
      return match;
    }
  }

}

bool checkPasswordAdmin()
{
  //  char input[4] = {};
  //  int num = 0;
  //  while (true) {
  //    char key = keypad.getKey();
  //    input[num] = key;
  //    num++;
  //    if (key == '*') {
  //      break;
  //    }
  //  }
  bool match = false;
  //
  //  for (int key = 0; key <= sizeof(passwords[0]); key++) {
  //    if (input[key] == passwords[0][key]) {
  //      match = true;
  //    } else {
  //      match = false;
  //    }
  //  }
  if (match) {
    return match;
  }
}

bool checkFingerLocally()
{
  uint8_t p = finger.getImage();
  switch (p)
  {
    case FINGERPRINT_OK:
      lcd.clear();
      Serial.println("Image taken");
      lcd.print("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      Serial.println("No finger detected");
      lcd.print("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      Serial.println("Communication error");
      lcd.print("Communication error");
      delay(2000);
      return p;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      Serial.println("Imaging error");
      lcd.print("Imaging error");
      delay(2000);
      return p;
    default:
      lcd.clear();
      Serial.println("Unknown error");
      lcd.print("Unknown error");
      delay(2000);
      return p;
  } // OK success!

  p = finger.image2Tz();
  switch (p)
  {
    case FINGERPRINT_OK:
      lcd.clear();
      Serial.println("Image converted");
      lcd.print("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      Serial.println("Image too messy");
      lcd.print("Image too messy");
      delay(2000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      Serial.println("Communication error");
      lcd.print("Communication error");
      delay(2000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      Serial.println("Could not find fingerprint features");
      lcd.print("Could not find");
      delay(2000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      Serial.println("Could not find fingerprint features");
      lcd.print("Could not find");
      delay(2000);
      return p;
    default:
      lcd.clear();
      Serial.println("Unknown error");
      lcd.print("Unknown error");
      delay(2000);
      return p;
  } // OK converted!

  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK)
  {
    lcd.clear();
    Serial.println("Found a print match!");
    lcd.print("Found a match");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    lcd.clear();
    Serial.println("Communication error");
    lcd.print("Communication error");
    delay(2000);
    return p;
  }
  else if (p == FINGERPRINT_NOTFOUND)
  {
    lcd.clear();
    Serial.println("Did not find a match");
    lcd.print("Did not find");
    delay(2000);
    return p;
  }
  else
  {
    lcd.clear();
    Serial.println("Unknown error");
    lcd.print("Unknown error");
    delay(2000);
    return p;
  } // found a match!

  lcd.clear();
  lcd.print("Match to ID # ");
  lcd.print(finger.fingerID);
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  //  return finger.fingerID;
  openGarage();
  return false;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
    return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
    return -1;
  // found a match!
  lcd.clear();
  lcd.print("Match to ID # " + finger.fingerID);
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  return finger.fingerID;
}

void enrollFingerLocally()
{
  lcd.clear();
  lcd.print("Ready to enroll a fingerprint!");
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  id = readnumber(); // GET THIS NUMBER FROM KEYPAD, NOT SURE HOW MAYBE A WHILE LOOP ON KEYPRESS
  if (id == 0)
  { // ID #0 not allowed, try again!
    return;
  }
  lcd.clear();
  lcd.print("Enrolling ID # " + id);
  Serial.print("Enrolling ID #");
  Serial.println(id);
  while (!getFingerprintEnroll());
}

uint8_t readnumber(void)
{
  uint8_t num = 0;
  while (num == 0)
  {
    while (!Serial.available())
      ;
    num = Serial.parseInt();
  }
  return num;
}

uint8_t getFingerprintEnroll()
{
  if (checkPasswordAdmin()) {
    int p = -1;
    lcd.clear();
    lcd.print("Waiting for valid finger to enroll as # " + id);
    Serial.print("Waiting for valid finger to enroll as #");
    Serial.println(id);
    while (p != FINGERPRINT_OK)
    {
      p = finger.getImage();
      switch (p)
      {
        case FINGERPRINT_OK:
          lcd.clear();
          Serial.println("Image taken");
          lcd.print("Image taken");
          break;
        case FINGERPRINT_NOFINGER:
          lcd.clear();
          Serial.println(".");
          lcd.print(".");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          lcd.clear();
          Serial.println("Communication error");
          lcd.print("Communication error");
          delay(2000);
          break;
        case FINGERPRINT_IMAGEFAIL:
          lcd.clear();
          Serial.println("Imaging error");
          lcd.print("Imaging error");
          delay(2000);
          break;
        default:
          lcd.clear();
          Serial.println("Unknown error");
          lcd.print("Unknown error");
          delay(2000);
          break;
      }
    } // OK success!

    p = finger.image2Tz(1);
    switch (p)
    {
      case FINGERPRINT_OK:
        lcd.clear();
        Serial.println("Image converted");
        lcd.print("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        lcd.clear();
        Serial.println("Image too messy");
        lcd.print("Image too messy");
        delay(2000);
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        lcd.clear();
        Serial.println("Communication error");
        lcd.print("Communication error");
        delay(2000);
        return p;
      case FINGERPRINT_FEATUREFAIL:
        lcd.clear();
        Serial.println("Could not find fingerprint features");
        lcd.print("Could not find");
        delay(2000);
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        lcd.clear();
        Serial.println("Could not find fingerprint features");
        lcd.print("Could not find");
        delay(2000);
        return p;
      default:
        lcd.clear();
        Serial.println("Unknown error");
        lcd.print("Unknown error");
        delay(2000);
        return p;
    }

    lcd.clear();
    lcd.print("Remove finger");
    Serial.println("Remove finger");
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER)
    {
      p = finger.getImage();
    }
    lcd.clear();
    lcd.print("ID " + id);
    Serial.print("ID ");
    Serial.println(id);
    p = -1;
    lcd.setCursor(0, 1); // bottom left
    lcd.print("Place same finger again");
    Serial.println("Place same finger again");
    while (p != FINGERPRINT_OK)
    {
      p = finger.getImage();
      switch (p)
      {
        case FINGERPRINT_OK:
          lcd.clear();
          Serial.println("Image taken");
          lcd.print("Image taken");
          break;
        case FINGERPRINT_NOFINGER:
          lcd.clear();
          Serial.println(".");
          lcd.print(".");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          lcd.clear();
          Serial.println("Communication error");
          lcd.print("Communication error");
          delay(2000);
          break;
        case FINGERPRINT_IMAGEFAIL:
          lcd.clear();
          Serial.println("Imaging error");
          lcd.print("Imaging error");
          delay(2000);
          break;
        default:
          lcd.clear();
          Serial.println("Imaging error");
          lcd.print("Imaging error");
          delay(2000);
          break;
      }
    } // OK success!

    p = finger.image2Tz(2);
    switch (p)
    {
      case FINGERPRINT_OK:
        lcd.clear();
        Serial.println("Image converted");
        lcd.print("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        lcd.clear();
        Serial.println("Image too messy");
        lcd.print("Image too messy");
        delay(2000);
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        lcd.clear();
        Serial.println("Communication error");
        lcd.print("Communication error");
        delay(2000);
        return p;
      case FINGERPRINT_FEATUREFAIL:
        lcd.clear();
        Serial.println("Could not find fingerprint features");
        lcd.print("Could not find");
        delay(2000);
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        lcd.clear();
        Serial.println("Could not find fingerprint features");
        lcd.print("Could not find");
        delay(2000);
        return p;
      default:
        lcd.clear();
        Serial.println("Unknown error");
        lcd.print("Unknown error");
        delay(2000);
        return p;
    } // OK converted!
    lcd.clear();
    lcd.print("Creating model for # " + id);
    Serial.print("Creating model for #");
    Serial.println(id);

    p = finger.createModel();
    if (p == FINGERPRINT_OK)
    {
      lcd.clear();
      Serial.println("Prints matched!");
      lcd.print("Prints matched");
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
      lcd.clear();
      Serial.println("Communication error");
      lcd.print("Communication error");
      delay(2000);
      return p;
    }
    else if (p == FINGERPRINT_ENROLLMISMATCH)
    {
      lcd.clear();
      Serial.println("Fingerprints did not match");
      lcd.print("did not match");
      delay(2000);
      return p;
    }
    else
    {
      lcd.clear();
      Serial.println("Unknown error");
      lcd.print("Unknown error");
      delay(2000);
      return p;
    }
    lcd.clear();
    lcd.print("ID " + id);
    Serial.print("ID ");
    Serial.println(id);
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK)
    {
      lcd.clear();
      Serial.println("Stored!");
      lcd.print("Image Stored");
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
      lcd.clear();
      Serial.println("Communication error");
      lcd.print("Communication error");
      delay(2000);
      return p;
    }
    else if (p == FINGERPRINT_BADLOCATION)
    {
      lcd.clear();
      Serial.println("Could not store in that location");
      lcd.print("Could not store");
      delay(2000);
      return p;
    }
    else if (p == FINGERPRINT_FLASHERR)
    {
      lcd.clear();
      Serial.println("Error writing to flash");
      lcd.print("Error writing");
      delay(2000);
      return p;
    }
    else
    {
      lcd.clear();
      Serial.println("Unknown error");
      lcd.print("Unknown error");
      delay(2000);
      return p;
    }

    return true;
  }
}
