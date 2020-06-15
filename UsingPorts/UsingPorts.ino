/* 
 *  Visual representation of the ports and respective 
 *  pins for the Arduino Mega 
 */
//-------------------------------------------------------
//     LSB  P0  P1  P2   P3   P4   P5,  P6,  P7  MSB
//-------------------------------------------------------
// PORTA = {22, 23, 24,  25,  26,  27,  28,  29};       
// PORTF = {A0, A1, A2,  A3,  A4,  A5,  A6,  A7};         
// PORTK = {A8, A9, A10, A11, A12, A13, A14, A15}; 
// PORTC = {37, 36, 35,  34,  33,  32,  31,  30};       
// PORTL = {49, 48, 47,  46,  45,  44,  43,  42}; 
// PORTB = {53, 52, 51,  50,  10,  11,  12,  13};
// PORTG = {41, 40, 39,  xx,  xx,  4,   xx,  xx};
// PORTD = {21, 20, 19,  18,  xx,  xx,  xx,  38};
// PORTE = {0,  1,  xx,  5,   2,   3,   xx,  xx};
// PORTH = {17, 16, xx,  6,   7,   8,   9,   xx};
// PORTJ = {15, 14, xx,  xx,  xx,  xx,  xx,  xx};     
//-------------------------------------------------------

int pos = 0;
int selection;
bool swapRunning = false;
long timer = millis();

void setup() {
  Serial.begin(9600);

  DDRA = 0xFF; // define all PORTA pins as outputs
  DDRC = 0x80; // define pin 30 on PORTC as an output
}

void blinkLED() {
  PORTC |= _BV(PC7); // _BV gets the bit value of pin C
  delay(500);
  //Serial.println(PINC >> PC7); // read the state of the pin
  PORTC &= ~_BV(PC7);
  delay(500);
  //Serial.println(PINC >> PC7); // read the state of the pin
}

void loop() {
  readSerialInput();

  //Serial.println(PINA); // read the state of the pins at PORTA
  
  switch(selection) {
    case 0:
      PORTA = 0;
      blinkLED();
      break;
    case 1:
      chaseLEDForward();
      break;
    case 2:
      chaseLEDBackward();
      break;
    case 3:
      if (!swapRunning) {
        swapLEDPlaces();
      }
      break;
    case 4:
      binaryCountUp();
      break;
  }
}

void readSerialInput() {
  if (Serial.available() > 0) {
    char incomingByte = Serial.read();
    
    switch(incomingByte) {
      case 'b':         // chase backwards
        selection = 1;
        break;
      case 'f':         // chase forwards
        selection = 2;
        break;
      case 's':         // swap led places 
        selection = 3;
        break;
      case 'c':
        selection = 4;  // binary count up
        break;
      case 'x':         // stop and blink LED
        selection = 0;
        break;
    }
  }
}

void binaryCountUp() {
  if (millis() - timer > 500) {
    PORTA &= 0;
    timer = millis();
    PORTA |= pos;
    pos++;
  }
}

// PORTA 0 - 7 == Digital pins 22 - 29
void chaseLEDForward() {
  PORTA |= (1 << pos); // turn on LED at pos
  delay(50);
  pos = (pos + 1) & 0x7;
  PORTA &= (1 << pos - 1); // turn off last LED
}

// PORTA 0 - 7 == Digital pins 22 - 29
void chaseLEDBackward() {
  pos = (pos - 1) & 0x7;
  PORTA |= (1 << pos); // turn on LED at pos
  delay(50);
  PORTA &= (1 << pos + 1); // turn off last LED
}

void swapLEDPlaces() {
  swapRunning = true;

  int mask = 0x55;
  PORTA &= mask;
  delay(200);
  PORTA |= mask;
  
  delay(200);
  PORTA &= ~mask;
  delay(200);
  PORTA |= ~mask;
  delay(200);
  
  swapRunning = false;
}
