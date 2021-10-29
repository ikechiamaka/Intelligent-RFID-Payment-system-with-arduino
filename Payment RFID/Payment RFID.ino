#include <SPI.h>
#include <Servo.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


const byte ROWS = 4; // Define the number of rows on the keypad
const byte COLS = 3; // Define the number of columns on the keypad
char keys[ROWS][COLS] = { // Matrix defining character to return for each key
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pins (R0-R3) of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pins (C0-C2) of the keypad
//initialize an instance of class
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RST_PIN   9
#define SS_PIN    10
#define buzzer    2
#define redLed    6
#define greenLed  5

#define INPUT_PASSWORD 0
#define INPUT_AMOUNT 1

#define DRIVER_INDEX 0
#define CARD1_INDEX 1
#define CARD2_INDEX 2
#define CARD3_INDEX 3
#define CARD4_INDEX 4
#define CARD5_INDEX 5


String content;
byte letter;
Servo Servo;
String user;
String cards[] = {
  " 25201 228 56", " 197 57 176 35", " 102 254 175 37",
  " 21302 212 35", " 197 136 41 35", " 115 2460004",
};
long password[] = {
  0000, 1234, 2345, 3456, 4567, 5678
};
long money[] = {
  0, 1000, 2000, 5000, 10000, 1500
};

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
MFRC522::MIFARE_Key key;
void setup() {
  Serial.begin(9600);    // Initialize serial communications with the PC
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  lcd.begin();
  lcd.backlight();
  //  lcd.print("Transportation");
  //  lcd.setCursor(0, 1);
  //  lcd.print("Payment System.");
  //  delay(3000);
  //  lcd.clear();
  //  lcd.print("Initializing...");
  //  delay(3000);
  //  lcd.clear();
  //  lcd.print("Welcome.");

}
void Swipe() {
  /* this loop will only break when a card is read */
  while (true) {
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      continue;
    }
    else
      Serial.println("A New Card is Present");

    if ( ! mfrc522.PICC_ReadCardSerial()) {
      continue;
    }
    else
      Serial.println("A Card was raed");
    break;
  }
  content = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], DEC));
  }

  lcd.clear();
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);
  long pass = GetNumber(INPUT_PASSWORD);
  Serial.print("PASS = ");
  Serial.println(pass);

  if (content == cards[DRIVER_INDEX]) {
    lcd.clear();
    if (pass == password[DRIVER_INDEX]) {
      lcd.print("WELCOME driver");
      lcd.setCursor(0, 1);
      lcd.print(money[DRIVER_INDEX]);
      while (GetKey() != '*');
    }
    else {
      lcd.print("Wrong pin");
      delay(3000);
      return;
    }
  }
  else {
    lcd.clear();
    lcd.print("1.Check Balance");
    lcd.setCursor(0, 1);
    lcd.print("2.Pay Fee");
    while (true) {
      switch (GetKey()) {
        case '1':
          /* BALANCE ENQIURY */
          lcd.clear();
          if (content == cards[CARD1_INDEX] ) {
            if (pass == password[CARD1_INDEX]) {
              lcd.print("WELCOME card1");
              lcd.setCursor(0, 1);
              lcd.print(money[CARD1_INDEX]);
            } else {
              lcd.print("Wrong pin");
              delay(3000);
              return;
            }
          }
          else if (content == cards[CARD2_INDEX] ) {
            if ( pass == password[CARD2_INDEX]) {
              lcd.print("WELCOME card2");
              lcd.setCursor(0, 1);
              lcd.print(money[CARD2_INDEX]);
            } else {
              lcd.print("Wrong pin");
              delay(3000);
              return;
            }
          }
          else if (content == cards[CARD3_INDEX]  ) {
            if (pass == password[CARD3_INDEX]) {
              lcd.print("WELCOME card3");
              lcd.setCursor(0, 1);
              lcd.print(money[CARD3_INDEX]);
            } else {
              lcd.print("Wrong pin");
              delay(3000);
              return;
            }
          }
          else if (content == cards[CARD4_INDEX]) {
            if (pass == password[CARD4_INDEX]) {
              lcd.print("WELCOME card4");
              lcd.setCursor(0, 1);
              lcd.print(money[CARD4_INDEX]);
            } else {
              lcd.print("Wrong pin");
              delay(3000);
              return;
            }
          }
          else if (content == cards[CARD5_INDEX]) {
            if (pass == password[CARD5_INDEX]) {
              lcd.print("WELCOME card5");
              lcd.setCursor(0, 1);
              lcd.print(money[CARD5_INDEX]);
            } else {
              lcd.print("Wrong pin");
              delay(3000);
              return;
            }
          }
          else {
            lcd.print("BAD card");
            delay(3000);
            return;
          }

          Serial.println("waiting for *");
          char _key;
          while ((_key = GetKey()) != '*')
            Serial.println(_key);
          Serial.println("seen *");
          Serial.println("returning");
          return;

        case '2':
          /* PAY FEE */
          lcd.clear();
          if (content == cards[CARD1_INDEX]) {
            if (pass == password[CARD1_INDEX]) {
              lcd.print("WELCOME card1");
              lcd.setCursor(0, 1);
              long amount = GetNumber(INPUT_AMOUNT);
              if (amount > money[CARD1_INDEX]) {
                lcd.clear();
                lcd.print("Insufficient");
                lcd.setCursor(0,1);
                lcd.print("funds!");
              }
              else {
                money[CARD1_INDEX] -= amount;
                money[DRIVER_INDEX] += amount;
                lcd.clear();
                lcd.print("PAYMENT ");
                lcd.setCursor(0,1);
                lcd.print("CONFIRMED!");
              }
            } else {
              lcd.print("Wrong pin");
              delay(3000);
              return;
            }
          }
          else if (content == cards[CARD2_INDEX]) {
            if (pass == password[CARD2_INDEX]) {
              lcd.print("WELCOME card2");
              lcd.setCursor(0, 1);
              long amount = GetNumber(INPUT_AMOUNT);
              if (amount > money[CARD2_INDEX]) {
                lcd.clear();
                lcd.print("Insufficient");
                lcd.setCursor(0,1);
                lcd.print("funds!");
              }
              else {
                money[CARD2_INDEX] -= amount;
                money[DRIVER_INDEX] += amount;
                lcd.clear();
                lcd.print("PAYMENT ");
                lcd.setCursor(0,1);
                lcd.print("CONFIRMED!");
              }
            } else {
              lcd.print("Wrong pin");
              delay(3000);
              return;
            }
          }
          else if (content == cards[CARD3_INDEX]) {
            if (pass == password[CARD3_INDEX]) {
              lcd.print("WELCOME card3");
              lcd.setCursor(0, 1);
              long amount = GetNumber(INPUT_AMOUNT);
              if (amount > money[CARD3_INDEX]) {
                lcd.clear();
                lcd.print("Insufficient");
                lcd.setCursor(0,1);
                lcd.print("funds!");
              }
              else {
                money[CARD3_INDEX] -= amount;
                money[DRIVER_INDEX] += amount;
                lcd.clear();
                lcd.print("PAYMENT ");
                lcd.setCursor(0,1);
                lcd.print("CONFIRMED!");
              }
            } else {
              lcd.print("Wrong pin");
              delay(3000);
              return;
            }
          }
          else if (content == cards[CARD4_INDEX]) {
            if (pass == password[CARD4_INDEX]) {
              lcd.print("WELCOME card4");
              lcd.setCursor(0, 1);
              long amount = GetNumber(INPUT_AMOUNT);
              if (amount > money[CARD4_INDEX]) {
                lcd.clear();
                lcd.print("Insufficient");
                lcd.setCursor(0,1);
                lcd.print("funds!");
              }
              else {
                money[CARD4_INDEX] -= amount;
                money[DRIVER_INDEX] += amount;
                lcd.clear();
                lcd.print("PAYMENT ");
                lcd.setCursor(0,1);
                lcd.print("CONFIRMED!");
              }
            } else {
              lcd.print("Wrong pin");
              delay(3000);
              return;
            }
          }
          else if (content == cards[CARD5_INDEX]) {
            if (pass == password[CARD5_INDEX]) {
              lcd.print("WELCOME card5");
              lcd.setCursor(0, 1);
              long amount = GetNumber(INPUT_AMOUNT);
              if (amount > money[CARD5_INDEX]) {
                lcd.clear();
                lcd.print("Insufficient");
                lcd.setCursor(0,1);
                lcd.print("funds!");
              }
              else {
                money[CARD5_INDEX] -= amount;
                money[DRIVER_INDEX] += amount;
                lcd.clear();
                lcd.print("PAYMENT ");
                lcd.setCursor(0,1);
                lcd.print("CONFIRMED!");
              }
            } else {
              lcd.print("Wrong pin");
              delay(3000);
              return;
            }
          }
          else {
            lcd.print("BAD card");
            delay(3000);
            return;
          }

          delay(3000);
          return;
        default:
          continue;
      }

    }
  }
}

long GetNumber(int Type) {
  char count = 0, key;
  char keyArray[10];
  long number = 0;
  do {
    key = GetKey();
    keyArray[count++] = key - '0';
    if ('#' != key) {
      if (INPUT_AMOUNT == Type) {
        Serial.print(key);
        lcd.print(key);
      }
      else {
        lcd.print('*');
        if (4 == count)
          break;
      }
    }
  }
  while (key >= '0' && key <= '9' && count < 10);
  if (key == '*')
    return -1; // cancel
  else if (key == '#') {
    count--; // ignore #
  }
  else {
    // limit reached, wait for #
    do
      key = GetKey();
    while (key != '#');
  }

  char i = 0;
  while (count--) {
    number += keyArray[i++] * powerOfTen(count);
  }
  return number;
}

// this is a blocking call
char GetKey(void) {
  char key;
  do
    key = keypad.getKey();
  while (key == 0);
  Serial.println("A key is pressed");
  return key;
}

long powerOfTen(char power) {
  long res = 1;
  if (!power)
    return 1;
  while (power--) {
    res *= 10;
  }
  return res;
}

void loop() {
  lcd.clear();
  lcd.print("Swipe card");
  Swipe();
}
