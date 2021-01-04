#include "ESC.h"

#define ARM_MIN 1000
#define ARM_MAX 1500

#define N 4

ESC* ESCs[N] = {
  &ESC (3, 1000, 2000, 500),
  &ESC (6, 1000, 2000, 500),
  &ESC (9, 1000, 2000, 500),
  &ESC (11, 1000, 2000, 500)
};

/*ESC myESC1 (3, 1000, 2000, 500);                 // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
ESC myESC2 (6, 1000, 2000, 500);                 // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
ESC myESC3 (9, 1000, 2000, 500);                 // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
ESC myESC4 (11, 1000, 2000, 500);                 // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)*/
int oESC;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Started arming"));
  /*myESC1.arm();
    myESC2.arm();
    myESC3.arm();
    myESC4.arm();*/
  for (uint8_t i = 0; i < N; i++) {
    ESCs[i]->arm();
  }

  for (oESC = ARM_MIN; oESC <= ARM_MAX; oESC += 10) {
    for (uint8_t i = 0; i < N; i++) {
      ESCs[i]->speed(oESC);
    }
    delay(10);
  }

  for (oESC = ARM_MAX; oESC > ARM_MIN; oESC -= 10) {
    for (uint8_t i = 0; i < N; i++) {
      ESCs[i]->speed(oESC);
    }
    delay(10);
  }
  delay(7000);

  for (uint8_t i = 0; i < N; i++) {
    ESCs[i]->speed(1200);
  }

  Serial.println(F("Done"));
}

void loop() {
  while (!Serial.available()) {}
  oESC = Serial.parseInt();
  delay(100);
  Serial.read();

  for (uint8_t i = 0; i < N; i++) {
    ESCs[i]->speed(oESC);
  }


  Serial.print(F("Speed now: "));
  Serial.println(oESC);

  delay(2000);

}
