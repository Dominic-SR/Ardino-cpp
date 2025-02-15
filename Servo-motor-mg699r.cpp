#include <Servo.h>

Servo myServo;  // Create servo object

void setup() {
    myServo.attach(D4);  // Connect MG996R signal wire to D4 (GPIO2)
}

void loop() {
    myServo.write(0);    // Rotate to 0 degrees
    delay(1000);         // Wait 1 second
    myServo.write(90);   // Rotate to 90 degrees
    delay(1000);         
    myServo.write(180);  // Rotate to 180 degrees
    delay(1000);
}
