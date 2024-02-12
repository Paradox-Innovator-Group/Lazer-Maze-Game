#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
int track = 10; // for define the trck name//track 0 = ambience; track 3 = start; track 5 = game over ; track 2 = too slow; track 4 = congratualtion; track 4 = welcome
#define vol 1  // fpr defining volume
int laserRelayPin = 24; //in2
int redLight = 26;//in3
int blueLight = 28;//in4
int greenLight = 22;//in1
int buzzer = 6;
int rst = 20;
static const uint8_t PIN_MP3_TX = 10;
static const uint8_t PIN_MP3_RX = 11;
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;
int button = 15;
int buttonStart = 21;
int buttonStartState = 1;
int buttonState =  1;
const int ldrPins[] = {31, 33,  35,37, 39, 41, 43, 45, 47, 49, 51, 53};  
const int numberOfLasers = 10;
unsigned long startTime = 0.0;
unsigned long currentTime = 0.0;
unsigned long elapsedTime = 0.0;
unsigned long Time = 0.0;
void MP3()
{

  if (player.begin(softwareSerial))
  {
    Serial.println("OK");
    player.volume(vol);
    player.play(track);
  }
  else
  {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
  
}

void game()
{
  currentTime = millis();
  digitalWrite(laserRelayPin, LOW);
  digitalWrite(blueLight, LOW);
  digitalWrite(redLight, HIGH);
  digitalWrite(greenLight, HIGH);
  delay(50); 
  if (detectNoLaser()) 
  {
    Serial.println("OBSTACLE DETECTED");
    track = 2;// for Game Over
    delay(10);
    //MP3();
    digitalWrite(laserRelayPin, HIGH);
    digitalWrite(redLight, LOW);
    digitalWrite(blueLight, HIGH);
    delay(3000);
    track = 0;
    //MP3();
    but();
  }
  buttonState = digitalRead(button);
  if(buttonState == LOW)
  {
    Serial.println("Button PRESSED");
    track = 4;// for Congratulation
    //MP3();
    digitalWrite(laserRelayPin, HIGH);
    digitalWrite(buzzer, HIGH);
    digitalWrite(redLight, HIGH);
    digitalWrite(blueLight, HIGH);
    digitalWrite(greenLight, LOW);
    delay(3000);
    digitalWrite(buzzer, LOW);
    buttonState = HIGH;
    track = 0;
    //MP3();
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
    track = 3;
    //MP3();
    digitalWrite(laserRelayPin, HIGH);
    digitalWrite(redLight, LOW);
    digitalWrite(blueLight, HIGH);
    delay(3000);
     track = 0;
    //MP3();
    but();
  }

  game();
}

void setup() {
  Serial.begin(9600);
  softwareSerial.begin(9600);
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
  track = 5;
  delay(10);
  //MP3();
  delay(3000);
  track = 0; // for ambience music
  delay(10);
 // MP3();
  Serial.println("Waiting for Start Button");
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
    delay(000);
    track = 1;
    delay(10);
    //MP3();
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
    if (digitalRead(ldrPins[i]) == 1) {
      Serial.println("OBSTACLE");
      return true;  // Laser detected
      
    }
  }
  return false;  // No laser detected
}
