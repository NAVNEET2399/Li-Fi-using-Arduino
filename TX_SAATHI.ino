#include <Wire.h>
#include <MPU6050.h>

#define TRANSMIT_LED 4
#define SAMPLING_TIME 20

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;

char* text = "ON";
unsigned int start = 0;
unsigned int current = 0;
bool laser_state = false;
bool transmit_data = true;
int bytes_counter = 20;
int total_bytes;
int second = 0;



int angleToDistance(int a)
{
  if (a < -80)
  {
    return -40;
  }
  else if (a < -65) {
    return -20;
  }
  else if (a < -50) {
    return -10;
  }
  else if (a < -15) {
    return -5;
  }
  else if (a < -5) {
    return -1;
  }
  else if (a > 80) {
    return 40;
  }
  else if (a > 65) {
    return 20;
  }
  else if (a > 15) {
    return 10;
  }
  else if (a > 5) {
    return 1;
  }
}

void setup() 
{
  
  Serial.begin(9600);    // initialize serial communications at 9600 bps:
  Wire.begin();          
  
  pinMode(TRANSMIT_LED, OUTPUT);
  total_bytes = strlen(text);
  mpu.initialize();
  if (!mpu.testConnection()) {
    while (1);
  }
}

void loop() {
  // Fetch the values from Gyro sensor
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  int vx = map(ax, -16000, 16000, 90, -90);
  int vy = map(ay, -16000, 16000, 90, -90);

  // START LASER GESTURE - TILT TO LEFT ,then TILT TO RIGHT
  Serial.print("WAITING FOR LEFT");
  if (vx < -40)
  { 
    while(1)
    {
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        int vx = map(ax, -16000, 16000, 90, -90);
        int vy = map(ay, -16000, 16000, 90, -90);
        Serial.print("WAITING FOR RIGHT");
        Serial.print(vx);
        Serial.print(" ");
        Serial.println(vy);           
        if(vx > 10)
           break;
    }
    
    // From here , data will be transmitted
    Serial.print("DATA SENDING MODE");
    Serial.println(start);
    int i = 0;
    while(i < 5)
    {
      while(transmit_data)
      {
        transmit_byte(text[total_bytes - bytes_counter]);
        bytes_counter--;
        if(bytes_counter == 0)
        {
          transmit_data = false;
        }
      }
      transmit_data = true;
      bytes_counter = total_bytes;
      
      delay(1000);
      Serial.print("iteration");
      Serial.println(i);
      i++;
   }
    digitalWrite(TRANSMIT_LED, LOW);
 }
  Serial.print(vx);
  Serial.print(" ");
  Serial.print(vy);
  Serial.print(" -------");
  Serial.print(angleToDistance(vx));
  Serial.print(": ");  
  Serial.println(angleToDistance(vy));
 
  delay(20);
}

void transmit_byte(char data_byte)
{
  digitalWrite(TRANSMIT_LED,LOW); 
  delay(SAMPLING_TIME);
  for(int i = 0; i < 8; i++)
  {
    digitalWrite(TRANSMIT_LED,(data_byte >> i) & 0x01); 
    delay(SAMPLING_TIME);
  }
  digitalWrite(TRANSMIT_LED,HIGH);            //Return to IDLE state
  delay(SAMPLING_TIME);
}
