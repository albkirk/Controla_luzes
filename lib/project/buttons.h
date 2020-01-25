#define BUTTONS_H true

void ICACHE_RAM_ATTR A_pressed ();
void ICACHE_RAM_ATTR B_pressed ();
void ICACHE_RAM_ATTR C_pressed ();
void ICACHE_RAM_ATTR D_pressed ();
void ICACHE_RAM_ATTR E_pressed ();


// **** Normal code definition here ...
#define BUT_A 2                 // Button A INPUT pin
#define BUT_B -1                // Button B INPUT pin
#define BUT_C -1                // Button C INPUT pin
#define BUT_D -1                // Button D INPUT pin
#define BUT_E -1                // Button E INPUT pin
#define interval 50             // time interval window to ignore bouncing
bool A_STATUS = false;          // status of button A (true = pressed, false = released)
bool B_STATUS = false;          // status of button B (true = pressed, false = released)
bool C_STATUS = false;          // status of button C (true = pressed, false = released)
bool D_STATUS = false;          // status of button D (true = pressed, false = released)
bool E_STATUS = false;          // status of button E (true = pressed, false = released)
volatile unsigned int A_COUNT = 0; // to count number of times button A is pressed within interval
volatile unsigned int B_COUNT = 0; // to count number of times button B is pressed within interval
volatile unsigned int C_COUNT = 0; // to count number of times button C is pressed within interval
volatile unsigned int D_COUNT = 0; // to count number of times button D is pressed within interval
volatile unsigned int E_COUNT = 0; // to count number of times button E is pressed within interval
unsigned int Last_A_COUNT = 0;  // to count number of times button A is pressed within interval
unsigned int Last_B_COUNT = 0;  // to count number of times button B is pressed within interval
unsigned int Last_C_COUNT = 0;  // to count number of times button C is pressed within interval
unsigned int Last_D_COUNT = 0;  // to count number of times button D is pressed within interval
unsigned int Last_E_COUNT = 0;  // to count number of times button E is pressed within interval
unsigned long Previous_A = 0;   // timer var to avoid function call trigger due contact bounce
unsigned long Previous_B = 0;   // timer var to avoid function call trigger due contact bounce
unsigned long Previous_C = 0;   // timer var to avoid function call trigger due contact bounce
unsigned long Previous_D = 0;   // timer var to avoid function call trigger due contact bounce
unsigned long Previous_E = 0;   // timer var to avoid function call trigger due contact bounce

// BUTTON related functions
void A_pressed () {
  detachInterrupt(BUT_A);                            // to avoid this function call be retriggered (or watch dog bites!!)
  unsigned long NOW = millis();
  if (NOW - Previous_A > interval / 2) {             // respect minimum time between 2 consecutive function calls
      while (millis() - NOW < interval / 2) {};      // loop to allow button status be stable before reading it
      if (digitalRead(BUT_A) == LOW) {
          A_STATUS = true;
          if (NOW - Previous_A < 6 * interval) {
              A_COUNT += 1;
              Serial.println("Button A pressed " + String(A_COUNT) + "times!");
          }
          else {
              A_COUNT = 1;
              Serial.println("Button A pressed");
          };
      }
      else {
          A_STATUS = false;
          Serial.println("Button A released");
      };
  }
  Previous_A = NOW;
  attachInterrupt(BUT_A, A_pressed, CHANGE);
}

void B_pressed () {
  detachInterrupt(BUT_B);                            // to avoid this function call be retriggered (or watch dog bites!!)
  unsigned long NOW = millis();
  if (NOW - Previous_B > interval / 2) {             // respect minimum time between 2 consecutive function calls
      while (millis() - NOW < interval / 2) {};      // loop to allow button status be stable before reading it
      if (digitalRead(BUT_B) == LOW) {
          B_STATUS = true;
          if (NOW - Previous_B < 6 * interval) {
              B_COUNT += 1;
              Serial.println("Button B pressed " + String(B_COUNT) + "times!");
          }
          else {
              B_COUNT = 1;
              Serial.println("Button B pressed");
          };
      }
      else {
          B_STATUS = false;
          Serial.println("Button B released");
      };
  }
  Previous_B = NOW;
  attachInterrupt(BUT_B, B_pressed, CHANGE);
}

void C_pressed () {
  detachInterrupt(BUT_C);                            // to avoid this function call be retriggered (or watch dog bites!!)
  unsigned long NOW = millis();
  if (NOW - Previous_C > interval / 2) {             // respect minimum time between 2 consecutive function calls
      while (millis() - NOW < interval / 2) {};      // loop to allow button status be stable before reading it
      if (digitalRead(BUT_C) == LOW) {
          C_STATUS = true;
          if (NOW - Previous_C < 6 * interval) {
              C_COUNT += 1;
              Serial.println("Button C pressed " + String(C_COUNT) + "times!");
          }
          else {
              C_COUNT = 1;
              Serial.println("Button C pressed");
          };
      }
      else {
          C_STATUS = false;
          Serial.println("Button C released");
      };
  }
  Previous_C = NOW;
  attachInterrupt(BUT_C, C_pressed, CHANGE);
}

void D_pressed () {
  detachInterrupt(BUT_D);                            // to avoid this function call be retriggered (or watch dog bites!!)
  unsigned long NOW = millis();
  if (NOW - Previous_D > interval / 2) {             // respect minimum time between 2 consecutive function calls
      while (millis() - NOW < interval / 2) {};      // loop to allow button status be stable before reading it
      if (digitalRead(BUT_D) == LOW) {
          D_STATUS = true;
          if (NOW - Previous_D < 6 * interval) {
              D_COUNT += 1;
              Serial.println("Button D pressed " + String(D_COUNT) + "times!");
          }
          else {
              D_COUNT = 1;
              Serial.println("Button D pressed");
          };
      }
      else {
          D_STATUS = false;
          Serial.println("Button D released");
      };
  }
  Previous_D = NOW;
  attachInterrupt(BUT_D, D_pressed, CHANGE);
}

void E_pressed () {
  detachInterrupt(BUT_E);                            // to avoid this function call be retriggered (or watch dog bites!!)
  unsigned long NOW = millis();
  if (NOW - Previous_E > interval / 2) {             // respect minimum time between 2 consecutive function calls
      while (millis() - NOW < interval / 2) {};      // loop to allow button status be stable before reading it
      if (digitalRead(BUT_E) == LOW) {
          E_STATUS = true;
          if (NOW - Previous_E < 6 * interval) {
              E_COUNT += 1;
              Serial.println("Button E pressed " + String(E_COUNT) + "times!");
          }
          else {
              E_COUNT = 1;
              Serial.println("Button E pressed");
          };
      }
      else {
          E_STATUS = false;
          Serial.println("Button E released");
      };
  }
  Previous_E = NOW;
  attachInterrupt(BUT_E, E_pressed, CHANGE);
}


void buttons_setup() {
  if (BUT_A>=0) {
        pinMode(BUT_A, INPUT_PULLUP);
        attachInterrupt(BUT_A, A_pressed, CHANGE);
  }
  if (BUT_B>=0) {
        pinMode(BUT_B, INPUT_PULLUP);
        attachInterrupt(BUT_B, B_pressed, CHANGE);
  }
  if (BUT_C>=0) {
        pinMode(BUT_C, INPUT_PULLUP);
        attachInterrupt(BUT_C, C_pressed, CHANGE);
  }
  if (BUT_D>=0) {
        pinMode(BUT_D, INPUT_PULLUP);
        attachInterrupt(BUT_D, D_pressed, CHANGE);
  }
  if (BUT_E>=0) {
        pinMode(BUT_E, INPUT_PULLUP);
        attachInterrupt(BUT_E, E_pressed, CHANGE);
  }

}
