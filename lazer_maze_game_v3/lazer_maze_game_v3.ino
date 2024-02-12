#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
int track = 10; // for define the trck name//track 0 = ambience; track 1 = start; track 2 = game over ; track 3 = too slow; track 4 = congratualtion; track 5 = welcome
#define vol 1  // fpr defining volume
int laserRelayPin = 24; //in2
int redLight = 26;//in3
int blueLight = 28;//in4
int greenLight = 22;//in1
int buzzer = 6;
int rst = 20;
static const uint8_t PIN__TX = 10;
static const uint8_t PIN__RX = 11;
SoftwareSerial mySerial(PIN__RX, PIN__TX);
DFRobotDFPlayerMini player;
int button = 15;
int buttonStart = 21;
int buttonStartState = 1;
int buttonState =  1;
const int ldrPins[] = {31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53};  
const int numberOfLasers = 1;
unsigned long startTime = 0.0;
unsigned long currentTime = 0.0;
unsigned long elapsedTime = 0.0;
unsigned long Time = 0.0;

  


void game()
{
  currentTime = millis();
  digitalWrite(laserRelayPin, LOW);
  digitalWrite(blueLight, LOW);
  digitalWrite(redLight, HIGH);
  digitalWrite(greenLight, HIGH);
  
  if (detectNoLaser()) 
  {
    Serial.println("OBSTACLE DETECTED");
    playTrack(2);
    digitalWrite(laserRelayPin, HIGH);
    digitalWrite(redLight, LOW);
    digitalWrite(blueLight, HIGH);
    delay(3000);
    track = 0;
    //();
    but();
  }
  buttonState = digitalRead(button);
  if(buttonState == LOW)
  {
    Serial.println("Button PRESSED");
    playTrack(4);
    digitalWrite(laserRelayPin, HIGH);
    digitalWrite(buzzer, HIGH);
    digitalWrite(redLight, HIGH);
    digitalWrite(blueLight, HIGH);
    digitalWrite(greenLight, LOW);
    delay(3000);
    digitalWrite(buzzer, LOW);
    buttonState = HIGH;
    playTrack(0);
    but();
  }
  elapsedTime = currentTime - startTime;
  Time = elapsedTime/1000 ;
  Serial.print("Elapsed Time: ");
  Serial.print(Time);  // Convert milliseconds to seconds
  Serial.println(" seconds");
  if(Time > 60)
  {
    Serial.println("Too Slow game ovr");
    playTrack(3);
    digitalWrite(laserRelayPin, HIGH);
    digitalWrite(redLight, LOW);
    digitalWrite(blueLight, HIGH);
    delay(3000);
    playTrack(0);
    but();
  }

  game();
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  sendCommand(0x3F); // Send "initialize" command
  delay(500);
  sendCommand(0x11); // Send "volume up" command
  delay(500);
  pinMode(laserRelayPin, OUTPUT);
  pinMode(redLight, OUTPUT);
  pinMode(blueLight, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(greenLight, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(buttonStart, INPUT_PULLUP);

  
  for (int i = 0; i < numberOfLasers; i++) {
    pinMode(ldrPins[i], INPUT);
  }
  digitalWrite(laserRelayPin, HIGH);
  digitalWrite(blueLight, HIGH);
  digitalWrite(redLight, HIGH);
  digitalWrite(greenLight, HIGH);
  digitalWrite(buzzer, LOW);
  digitalWrite(16, HIGH);
  digitalWrite(17, LOW);
  digitalWrite(17, LOW);
  playTrack(5);
  delay(3000);
  playTrack(0);
  Serial.println("Waiting for Start Button");
}

void sendCommand(uint8_t command) {
  mySerial.write(0x7E); // Start code
  mySerial.write(0xFF); // Version
  mySerial.write(0x06); // Command length
  mySerial.write(command);
  mySerial.write((uint8_t)0x00); // End code
}

void playTrack(uint16_t trackNumber) {
  sendCommand(0x12); // Command for playing a specific track
  mySerial.write((byte)(trackNumber >> 8)); // High byte of track number
  mySerial.write((byte)(trackNumber & 0xFF)); // Low byte of track number
  mySerial.write((uint8_t)0x00); // End code
}


void but()
{
  digitalWrite(laserRelayPin, HIGH);
  digitalWrite(blueLight, HIGH);
  digitalWrite(redLight, HIGH);
  digitalWrite(greenLight, HIGH);
  digitalWrite(buzzer, LOW);
  digitalWrite(16, HIGH);
  digitalWrite(17, LOW);  
    
  buttonStartState = digitalRead(buttonStart);
  if( buttonStartState == LOW)
  {
    Serial.println("pressed Start Button");
    buttonStartState = HIGH;
    delay(5000);
    playTrack(1);
    delay(1500);
    startTime = millis();
    game();
  }
  but();
}


void loop() 
{
  but();
}

bool detectNoLaser() {
  for (int i = 0; i < numberOfLasers; i++) {
    if (digitalRead(ldrPins[i]) == 0) {
      Serial.println("NO OBSTACLE");
      return false;  // Laser detected
      
    }
  }
  return true;  // No laser detected
}
