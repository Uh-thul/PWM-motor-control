#define MOTOR_PWM_PIN 3
#define RELAY_PIN     7  


#define RELAY_ON  HIGH
#define RELAY_OFF LOW

bool emergencyStop = false;
int currentPwm = 0;

void setup() {
  pinMode(MOTOR_PWM_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  analogWrite(MOTOR_PWM_PIN, 0);
  digitalWrite(RELAY_PIN, RELAY_OFF);

  Serial.begin(9600);
  Serial.println("--- System Controller Ready ---");
  Serial.println("Commands:");
  Serial.println("  0-255 : Set motor speed");
  Serial.println("  'v'   : Open valve (Relay ON)");
  Serial.println("  'c'   : Close valve (Relay OFF)");
  Serial.println("  'e'   : Trigger EMERGENCY STOP (Cuts motor & valve)");
  Serial.println("  'r'   : RESET / Clear E-stop");
  Serial.println("--------------------------------");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.peek();

    
    if (cmd == 'e' || cmd == 'E') {
      Serial.read();
      emergencyStop = true;
      currentPwm = 0;


      analogWrite(MOTOR_PWM_PIN, 0);
      digitalWrite(RELAY_PIN, RELAY_OFF);

      Serial.println("\n[ALERT] E-STOP TRIGGERED! Motor stopped & Valve closed.");
    }

    else if (cmd == 'r' || cmd == 'R') {
      Serial.read();
      emergencyStop = false;
      Serial.println("\n[OK] E-STOP Cleared. Ready for input.");
    }

    else if (cmd == 'v' || cmd == 'V') {
      Serial.read();
      if (emergencyStop) {
        Serial.println("\n[BLOCKED] Cannot open valve - E-Stop is active!");
      } else {
        digitalWrite(RELAY_PIN, RELAY_ON);
        Serial.println("Valve OPENED (Relay ON).");
      }
    }

  
    else if (cmd == 'c' || cmd == 'C') {
      Serial.read();
      digitalWrite(RELAY_PIN, RELAY_OFF);
      Serial.println("Valve CLOSED (Relay OFF).");
    }

    else if (isDigit(cmd)) {
      int targetPwm = Serial.parseInt();

      if (emergencyStop) {
        Serial.println("\n[BLOCKED] Cannot set speed - E-Stop is active!");
      } else {
        currentPwm = constrain(targetPwm, 0, 255);
        analogWrite(MOTOR_PWM_PIN, currentPwm);
        Serial.print("Speed updated: PWM = ");
        Serial.println(currentPwm);
      }
    }

    else {
      Serial.read();
    }
  }
}
