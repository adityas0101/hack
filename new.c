#include <Servo.h>

// Servo Motor Declarations
Servo baseServo, joint1Servo, joint2Servo, joint3Servo, endEffectorServo;
Servo missileDirectionServo, missileAngleServo;

// Ultrasonic Sensor Pins
const int trigPin = 9;
const int echoPin = 10;

// Laser Light Pin
const int laserPin = 13;

// Variables for Ultrasonic Sensor
long duration;
int distance;
const int detectionThreshold = 30; // Object detection range in cm

// Initial Positions
int basePosition = 90;
int joint1Position = 90;
int joint2Position = 90;
int joint3Position = 90;
int endEffectorPosition = 0;

void setup()
{
    // Initialize Serial Communication
    Serial.begin(9600);

    // Attach Servo Motors
    baseServo.attach(3);
    joint1Servo.attach(4);
    joint2Servo.attach(5);
    joint3Servo.attach(6);
    endEffectorServo.attach(7);
    missileDirectionServo.attach(8);
    missileAngleServo.attach(11);

    // Ultrasonic Sensor Pins
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Laser Pin
    pinMode(laserPin, OUTPUT);
    digitalWrite(laserPin, LOW);

    // Initialize Servo Positions
    baseServo.write(basePosition);
    joint1Servo.write(joint1Position);
    joint2Servo.write(joint2Position);
    joint3Servo.write(joint3Position);
    endEffectorServo.write(endEffectorPosition);
    missileDirectionServo.write(90); // Neutral direction
    missileAngleServo.write(45);     // Neutral angle
}

void loop()
{
    // Continuous movement of the ultrasonic sensor
    for (int angle = 0; angle <= 270; angle += 5)
    {
        endEffectorServo.write(angle);
        delay(100); // Smooth scanning

        // Read ultrasonic sensor
        distance = readUltrasonicSensor();

        // Display distance on the Serial Monitor
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

        // If object is detected
        if (distance > 0 && distance <= detectionThreshold)
        {
            Serial.println("Object Detected!");

            // Adjust missile direction and angle
            missileDirectionServo.write(map(distance, 10, 30, 70, 110)); // Adjust direction based on distance
            missileAngleServo.write(map(distance, 10, 30, 30, 60));      // Adjust angle based on distance

            delay(1000); // Wait for servos to move

            // Activate laser light
            activateLaser();

            // Reset scanning
            endEffectorServo.write(0);
            delay(500);
            return;
        }
    }

    // Reset end effector position
    endEffectorServo.write(0);
    delay(500);

    // Change robotic arm position every 10 seconds
    static unsigned long lastPositionChange = 0;
    if (millis() - lastPositionChange > 10000)
    {
        basePosition = random(45, 135);
        joint1Position = random(45, 135);
        joint2Position = random(45, 135);
        joint3Position = random(45, 135);

        baseServo.write(basePosition);
        joint1Servo.write(joint1Position);
        joint2Servo.write(joint2Position);
        joint3Servo.write(joint3Position);

        lastPositionChange = millis();
    }
}

int readUltrasonicSensor()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure the echo pulse duration
    duration = pulseIn(echoPin, HIGH);

    // Calculate distance in cm
    return duration * 0.034 / 2;
}

void activateLaser()
{
    Serial.println("Laser Activated!");
    digitalWrite(laserPin, HIGH); // Turn on laser
    delay(2000);                  // Keep laser on for 2 seconds
    digitalWrite(laserPin, LOW);  // Turn off laser
    Serial.println("Laser Deactivated!");
}