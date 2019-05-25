#include <LiquidCrystal_I2C.h>

int btn_Go;
int btn1;
int btn2;
int btn3;
int btn4;
int btn5;
int vite;
int tempo;
int partite;
int punteggio;
int btn;

LiquidCrystal_I2C lcd(0x27, 16, 2);

//Creo 3 byte, il cuore per le vite e i bonus, il PiGreco e la X per il malus
byte Vita[8]   = { b00000,b01010,b10101,b10001,b10001,b01010,b00100,b00000 };
byte PiGreco[8] = { b00000,b11111,b01010,b01010,b01010,b01010,b01001,b00000 };
byte Malus[8]   = { b00000,b10001,b10001,b01010,b00100,b01010,b10001,b10001 };

void setup() {
  // put your setup code here, to run once:
  btn_Go = 8;
  btn1      = 6;
  btn2      = 4;
  btn3      = 12;
  btn4      = 10;
  btn5      = 2;
  pinMode(btn_Go, INPUT);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  pinMode(btn4, INPUT);
  pinMode(btn5, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, Vita);
  lcd.createChar(1, PiGreco);
  lcd.createChar(2, Malus);
  start();
}

void ciclo(int cella, int simbolo){
  if(cella < 3)
    btn = btn1;
  else if(cella >= 3 && cella < 7)
    btn = btn2;
  else if(cella >= 7 && cella < 10)
    btn = btn3;
  else if(cella >= 10 && cella < 13)
    btn = btn4;
  else if(cella >= 13 && cella <= 16)
    btn = btn5;
  delay(2000);
  lcd.setCursor(cella, 1);
  lcd.write(simbolo);
  punteggio = 0;
  while(digitalRead(btn) == LOW && punteggio < tempo){
    punteggio++;
    delay(1);
    
    if((btn != btn1 && digitalRead(btn1) == HIGH) || (btn != btn2 && digitalRead(btn2) == HIGH) || (btn != btn3 && digitalRead(btn3) == HIGH) || (btn != btn4 && digitalRead(btn4) == HIGH) || (btn != btn5 && digitalRead(btn5) == HIGH)){
      vite--;
      break;
    }
  }
  //Ciclo per togliere la seconda riga del display
  for(int i = 0; i < 16; i++){
    lcd.setCursor(i, 1);
    lcd.print(" ");
  }
  //Se esce il malus e il giocatore non preme alcun bottone non perde una vita, altrimenti -1 vite
  if(punteggio == tempo && simbolo != byte(2))
    vite--;
  else{
    //Se esce il bonus e il giocatore lo prende e guadagnerà una vita, altrimenti il gioco prosegue
    if(simbolo == byte(0))
      vite++;
    tempo -= 100;
    partite++;
  }
  lcd.setCursor(4, 0);
  lcd.print(vite);
  lcd.setCursor(14, 0);
  lcd.print(partite);
}

//ciclo d'inizio che verrà richiamato solamente all'inizio e quando il giocatore non ha più vite
void Go(){
  lcd.clear();
  tempo = 4050;
  vite      = 3;
  partite    = 0;
  while(digitalRead(btn_Go) == LOW){
    lcd.setCursor(0, 0);
    lcd.print("INIZIA IL GIOCO");
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print(" = "+ (String)vite);
  lcd.setCursor(7, 0);
  lcd.print("PARTITA = " + (String)partite);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(vite > 0){
    //Estraggo 7 numeri casuali, se il numero va da 1 a 5, parte il ciclo con il PiGreco e la rispettiva posizione, se è 6 o 7 parte lo stesso ciclo con rispettivamente malus e/o bonus e una
    //posizione casuale nella seconda riga del display
    int r = random(7);
    if(r == 1)
      ciclo(0, byte(1));
    else if(r == 2)
      ciclo(4, byte(1));
    else if(r == 3)
      ciclo(8, byte(1));
    else if(r == 4)
      ciclo(12, byte(1));
    else if(r == 5)
      ciclo(16, byte(1));
    else if(r == 6)
      ciclo(random(16), byte(0));
    else if(r == 7)
      ciclo(random(16), byte(2));
  }
  else{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("PUNTI:");
    lcd.setCursor(7, 1);
    lcd.print(partite);
    delay(3000);
    GO();
  } 
}
