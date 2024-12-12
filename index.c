import processing.serial.*;

Serial myPort;             // The serial port
float distance = 0;        // Distance variable

void setup() {
  size(600, 400);          // Window size
  println(Serial.list());  // List all available serial ports
  myPort = new Serial(this, Serial.list()[0], 9600); // Connect to Arduino
  myPort.bufferUntil('\n'); // Read data until newline
}

void draw() {
  background(0);          // Black background
  fill(0, 255, 0);        // Green bar
  rect(50, height - 50, distance * 5, 30); // Bar based on distance
  
  fill(255);              // White text
  textSize(20);
  text("Distance: " + distance + " cm", 50, height - 70);
}

void serialEvent(Serial myPort) {
  String data = myPort.readStringUntil('\n'); // Read data from Arduino
  if (data != null) {
    data = trim(data); // Clean the data
    distance = float(data); // Convert to float
  }
}