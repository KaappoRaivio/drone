#include "ESC.h"

#define ARM_MIN 1000
#define ARM_MAX 1500

ESC myESC1 (3, 1000, 2000, 500);                 // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
ESC myESC2 (6, 1000, 2000, 500);                 // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
ESC myESC3 (9, 1000, 2000, 500);                 // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
ESC myESC4 (11, 1000, 2000, 500);                 // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
int oESC;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Started arming"));
  myESC1.arm();
  myESC2.arm();
  myESC3.arm();
  myESC4.arm();

  for (oESC = ARM_MIN; oESC <= ARM_MAX; oESC += 10) {
    myESC1.speed(oESC);
    myESC2.speed(oESC);
    myESC3.speed(oESC);
    myESC4.speed(oESC);
    delay(10);
  }

  for (oESC = ARM_MAX; oESC > ARM_MIN; oESC -= 10) {
    myESC1.speed(oESC);
    myESC2.speed(oESC);
    myESC3.speed(oESC);
    myESC4.speed(oESC);
    delay(10);
  }
  delay(7000);

  myESC1.speed(1000);
  myESC2.speed(1000);
  myESC3.speed(1000);
  myESC4.speed(1000);

  delay(1000);

  myESC1.speed(1200);
  myESC2.speed(1200);
  myESC3.speed(1200);
  myESC4.speed(1200);
  Serial.println(F("Done"));

}

void loop() {
  while (!Serial.available()) {}
  oESC = Serial.parseInt();
  delay(100);
  Serial.read();

  if (oESC == 1000) {
    myESC1.stop();
    myESC2.stop();
    myESC3.stop();
    myESC4.stop();
  }

  myESC1.speed(oESC);
  myESC2.speed(oESC);
  myESC3.speed(oESC);
  myESC4.speed(oESC);


  Serial.print(F("Speed now: "));
  Serial.println(oESC);

  delay(2000);

}
