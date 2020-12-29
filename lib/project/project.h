// **** Project code definition here ...
#define SW_1 13                 // Switch 1 INPUT pin
#define SW_2 12                 // Switch 2 INPUT pin
#define Switch  14              // Lamp 1 OUTPUT pin
#define Switch2 15              // Lamp 2 OUTPUT pin
bool SWITCH2 = false;
bool SWITCH2_Last = false;
volatile unsigned int ROLL1_Count = 0;
volatile unsigned int ROLL2_Count = 0;
unsigned int ROLL1_Count_Last = 0;
unsigned int ROLL2_Count_Last = 0;
static bool ROLL1 = false;
static bool ROLL1_Last = false;
static bool ROLL2 = false;
static bool ROLL2_Last = false;
volatile unsigned long time_ROLL1 = 0;   // last time the rolling_1() function was called
volatile unsigned long time_ROLL2 = 0;   // last time the rolling_1() function was called
unsigned long prj_poll = 0;
unsigned long prj_poll_last = 0;
unsigned long TIMER2 = 0;                // [0-7200]  Minutes                 
unsigned long starting_timer = 0;
unsigned long starting_timer2 = 0;
int timer_current = 0;
int timer2_current = 0;

#include <buttons.h>

// **** Project code functions here ...


void ICACHE_RAM_ATTR rolling_1() {
    ROLL1_Count +=1;
    time_ROLL1 = millis();
}

void ICACHE_RAM_ATTR rolling_2() {
    ROLL2_Count +=1;
    time_ROLL2 = millis();
}

void project_hw() {
 // Output GPIOs
    pinMode(Switch, OUTPUT);
    digitalWrite(Switch, LOW);   // initialize SWITCH off
    pinMode(Switch2, OUTPUT);
    digitalWrite(Switch2, LOW);   // initialize SWITCH2 off


 // Input GPIOs

}


void button_loop() {
    if (A_COUNT == 1 && (millis() - Last_A > Butt_Interval)) {
        telnet_println("Button A pressed ");
        Buzz(A_COUNT);
        A_COUNT = 0;
    }
    if (A_COUNT ==2 && (millis() - Last_A > Butt_Interval)) {
        telnet_println("Button A pressed " + String(A_COUNT) + "times!");
        Buzz(A_COUNT);
        A_COUNT = 0;
    }
    if (A_COUNT == 5 && (millis() - Last_A > Butt_Interval)) {
        telnet_println("Button A pressed " + String(A_COUNT) + "times!");
        Buzz(A_COUNT);
        A_COUNT = 0;
        ESPRestart();
    }

    prj_poll = millis();
    if (prj_poll_last > prj_poll) prj_poll_last = 0;

    if (prj_poll - prj_poll_last > 10 * Butt_Bounce_Time) {
        if ( ROLL1_Count - ROLL1_Count_Last > uint((prj_poll - prj_poll_last) / (2*10) ) ) ROLL1 = true;
        else ROLL1 = false;
        if ( ROLL2_Count - ROLL2_Count_Last > uint((prj_poll - prj_poll_last) / (2*10) ) ) ROLL2 = true;
        else ROLL2 = false;
        ROLL1_Count_Last = ROLL1_Count;
        ROLL2_Count_Last = ROLL2_Count;
        prj_poll_last = prj_poll;
    }
}

void project_loop() {

    button_loop();

    //if ((millis() % 1000) < 30) telnet_println("ROLL1= " + String(ROLL1_Count) + "\tROLL2= " + String(ROLL2_Count)); 


    if (ROLL1 != ROLL1_Last) {
        if (SWITCH_Last == false) {
            TIMER = 60;
            mqtt_publish(mqtt_pathtele, "Timer", String(TIMER));
            telnet_println("Button B flipped (timer = " + String(TIMER) + "min)");
        }
        else {
            SWITCH = false;
            telnet_println("Button B flipped");
        }
        ROLL1_Last = ROLL1;
    }


/*
    if (ROLL1 != ROLL1_Last) {
        SWITCH = !SWITCH;
        ROLL1_Last = ROLL1;
        telnet_println("Button B flipped");
    }
*/

    if (ROLL2 != ROLL2_Last) {
        SWITCH2 = !SWITCH2;
        ROLL2_Last = ROLL2;
        telnet_println("Button C flipped");
    }

    if (TIMER > 0 && (SWITCH_Last == false)) {
            SWITCH = true;
            starting_timer = millis();
            timer_current = TIMER - 1;
            Buzz(1);
    }

    if (TIMER > 0 && (millis() % 60000 < 15)) {
        timer_current = int((starting_timer + (TIMER * 60000) - millis()) / (60000));
        if (timer_current <= 0) {
            SWITCH = false;                             // Switch OFF as timer expired!
            Buzz(2);
        }
        mqtt_publish(mqtt_pathtele, "Timer", String(timer_current));
        delay(15);                                      // Use the same value as used in the if condition
    }


    if (TIMER2 > 0 && (SWITCH2_Last == false)) {
            SWITCH2 = true;
            starting_timer2 = millis();
            timer2_current = TIMER2 - 1;
            Buzz(1);
    }

    if (TIMER2 > 0 && ((millis() - 250) % 60000 < 15)) {
        timer2_current = int((starting_timer2 + (TIMER2 * 60000) - millis()) / (60000));
        if (timer2_current <= 0) {
            SWITCH2 = false;                             // Switch OFF as timer expired!
            Buzz(2);
        }
        mqtt_publish(mqtt_pathtele, "Timer2", String(timer2_current));
        delay(15);                                      // Use the same value as used in the if condition
    }


    if (SWITCH != SWITCH_Last) {
            detachInterrupt(SW_1);
            detachInterrupt(SW_2);
            digitalWrite(Switch, SWITCH);               // Touggle Switch RELAY
            delay(Butt_Bounce_Time);
            attachInterrupt(SW_1, rolling_1, CHANGE);
            attachInterrupt(SW_2, rolling_2, CHANGE);
            SWITCH_Last = SWITCH;
            if (SWITCH == false) {
                TIMER = 0;                              // Switch is OFF it must disable timer feature.
                timer_current = 0;
                mqtt_publish(mqtt_pathtele, "Timer", String(TIMER));
            }             
		    mqtt_publish(mqtt_pathtele, "Exterior", String(SWITCH));
    }

    if (SWITCH2 != SWITCH2_Last) {
            detachInterrupt(SW_1);
            detachInterrupt(SW_2);
            digitalWrite(Switch2, SWITCH2);             // Touggle Switch RELAY
            delay(Butt_Bounce_Time);
            attachInterrupt(SW_1, rolling_1, CHANGE);
            attachInterrupt(SW_2, rolling_2, CHANGE);
            SWITCH2_Last = SWITCH2;
            if (SWITCH2 == false) {
                TIMER2 = 0;                              // Switch is OFF it must disable timer feature.
                timer2_current = 0;
                mqtt_publish(mqtt_pathtele, "Timer2", String(TIMER2));
            }             
		    mqtt_publish(mqtt_pathtele, "Interior", String(SWITCH2));
    }

}


void project_setup() {
    buttons_setup();

    if (SW_1>=0) {
        pinMode(SW_1, INPUT_PULLUP);
        attachInterrupt(SW_1, rolling_1, CHANGE);
    }
    if (SW_2>=0) {
        pinMode(SW_2, INPUT_PULLUP);
        attachInterrupt(SW_2, rolling_2, CHANGE);
    }

    unsigned long Hold_ME = millis();
    while (millis() - Hold_ME < Butt_Interval)
    {
        button_loop();
    }

    Buzz(2);
    ROLL1_Last = ROLL1;
    ROLL2_Last = ROLL2;
    SWITCH = false;
    SWITCH2 = false;
    SWITCH_Last = false;
    SWITCH2_Last = false;
    digitalWrite(Switch, LOW);   // initialize SWITCH off
    digitalWrite(Switch2, LOW);   // initialize SWITCH2 off

    mqtt_publish(mqtt_pathtele, "Exterior", String(SWITCH));    // To update the MQTT Broker about the initial Switch status
    mqtt_publish(mqtt_pathtele, "Interior", String(SWITCH2));  // To update the MQTT Broker about the initial Switch2 status
}