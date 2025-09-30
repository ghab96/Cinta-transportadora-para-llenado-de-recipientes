int speedPin=11;
int dirPin1=12;
int dirPin2=13;
int speedMotor= 70;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(speedPin, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(dirPin1,0);
  digitalWrite(dirPin2,1);
  analogWrite(speedPin, speedMotor);
}
