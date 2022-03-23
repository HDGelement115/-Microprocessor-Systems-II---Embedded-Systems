
#include<Wire.h>
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int VRx = A0;
int VRy = A1;
//int SW = 2;


int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;

int eaton;
int buzzer = 3;
//char tmp_str[7];

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(250000);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  //pinMode(SW, INPUT_PULLUP); 
  pinMode(buzzer, OUTPUT);
}
void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);    //each measurement has 2 registers
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();
  //Tmp = Wire.read()<<8 | Wire.read();  
  //GyX=Wire.read()<<8|Wire.read();  
  //GyY=Wire.read()<<8|Wire.read();  
  //GyZ=Wire.read()<<8|Wire.read();  

  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  //SW_state = digitalRead(SW);

  // Maps joystick input from -5 to 5 with 0 as the origin (joystick not used)
  mapX = map(xPosition + 18, 0, 1023, -5, 5);     
  mapY = map(yPosition + 18, 0, 1023, -5, 5);

  if (mapX || mapY ){   // Stops Joystick input to Serial if both mapX and mapY are 0 (Joystick not used)
    Serial.print("Joystick: ");
    Serial.print("X: ");
    Serial.print(mapX);
    Serial.print(" | Y: ");
    Serial.print(mapY);
    Serial.print("\n");
    //Serial.print(" | Button: ");
    //Serial.println(SW_state);
  }

  // Accelerometer data used for tilt detection. Ranges are defined based on MPU6050 raw data when used
  if (AcX < 2000 && AcY > 1000)
    Serial.println("MPU: 2"); //Down
  else if (AcX < 2000 && AcY < -1000) 
    Serial .println("MPU: 8"); //Up
  else if (AcX < -1000 && AcY > -1000) 
    Serial .println("MPU: 6"); //right
  else if (AcX > 2000 && AcY > -1000) 
    Serial .println("MPU: 4"); //left
/*
  //Serial.print("Tempature: ");
  //Serial.println(Tmp/340.00+36.53);
  
  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.println(GyZ);
  
  */
  
  //using a single flag to turn the buzzer on and off in the same go
  //~Ryans idea
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    eaton = Serial.read();
    // if it's a capital H (ASCII 72), turn on the Buzzer;
    if (eaton == 'H') {
      tone(buzzer, 1000,500);
    }

  }
  delay(200);
}
