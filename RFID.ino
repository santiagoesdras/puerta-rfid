#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>    // libreria para LCD por I2C
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN  9      // constante para referenciar pin de reset
#define SS_PIN  10      // constante para referenciar pin de slave select
MFRC522 mfrc522(SS_PIN, RST_PIN); // crea objeto mfrc522 enviando pines de slave select y reset
LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7); // DIR, E, RW, RS, D4, D5, D6, D7
byte LecturaUID[4];         // crea array para almacenar el UID leido
byte tarjeta[4]= {0xA2, 0xD7, 0xEB, 0x1C} ;    // UID de tarjeta leido en programa 1
byte llavero[4]= {0x3B, 0x6F, 0x93, 0x15} ;    // UID de llavero leido en programa 1

void setup() {
  Serial.begin(9600);     
  SPI.begin();
  mfrc522.PCD_Init();     // inicializa modulo lector
  Serial.println("Listo");    // Muestra texto Listo
  delay(500);
lcd.setBacklightPin(3,POSITIVE);  // puerto P3 de PCF8574 como positivo
    lcd.setBacklight(HIGH);   // habilita iluminacion posterior de LCD
    lcd.begin(16, 2);     // 16 columnas por 2 lineas para LCD 1602A
    lcd.clear();      // limpia pantalla
  lcd.setCursor(1,0);
  lcd.print("Cerradura RFID");
  delay(2750);
  lcd.clear();
}

void loop() {
RFID();
delay(350);
}
             


void RFID()
{
   
   if ( ! mfrc522.PICC_IsNewCardPresent()) {  // si no hay una tarjeta presente
     lcd.clear();
     lcd.setCursor(4,1);
     lcd.print("Cerrado");
     return;
          // retorna al loop esperando por una tarjeta
  }
  if ( ! mfrc522.PICC_ReadCardSerial()){     // si no puede obtener datos de la tarjeta
    return;          } // retorna al loop esperando por otra tarjeta
       // muestra texto UID:
      // retorna al loop esperando por otra tarjeta        
       
for (byte i = 0; i < mfrc522.uid.size; i++) {  // bucle recorre de a un byte por vez el UID
      if (mfrc522.uid.uidByte[i] < 0x10){   // si el byte leido es menor a 0x10
        Serial.print(" 0");       // imprime espacio en blanco y numero cero
        }
        else{           // sino
          Serial.print(" ");        // imprime un espacio en blanco
          }
          Serial.print(mfrc522.uid.uidByte[i], HEX);    // imprime el byte del UID leido en hexadecimal
          LecturaUID[i]=mfrc522.uid.uidByte[i];     // almacena en array el byte del UID leido      
          }
                       
          if(comparaUID(mfrc522.uid.uidByte, tarjeta)==true) { // llama a funcion comparaUID con tarjeta
            lcd.setCursor(1,1);
            lcd.print("Acceso tarjeta");
            delay(1000);
       
          }
            // si retorna verdadero muestra texto bienvenida

          else if(comparaUID(LecturaUID, llavero)==true)
          {
            lcd.setCursor(1,1);
            lcd.print("Acceso llavero");
            delay(1000);  
          }
            // si retorna verdadero muestra texto bienvenida

}


boolean comparaUID(byte lectura[],byte usuario[]) // funcion comparaUID
{
  for (byte i=0; i < mfrc522.uid.size; i++){    // bucle recorre de a un byte por vez el UID
  if(lectura[i] != usuario[i])        // si byte de UID leido es distinto a usuario
    return(false);          // retorna falso
  }
  return(true);           // si los 4 bytes coinciden retorna verdadero
}
