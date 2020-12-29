// This code belongs to Navneet Kumar, ECE, PEC CHANDIGARH

#define TRANSMIT_LED 4
#define SAMPLING_TIME 20


char* text = "ON";                        // text that you want to send
unsigned int start = 0;
unsigned int current = 0;
bool laser_state = false;
bool transmit_data = true;
int bytes_counter = 20;
int total_bytes;
int second = 0;

void setup() 
{
  Serial.begin(9600);                 // initialize serial communications at 9600 bps:
  pinMode(TRANSMIT_LED, OUTPUT);
  total_bytes = strlen(text);
}

void loop() {
   
    // From here , data will be transmitted
    Serial.print("DATA SENDING MODE");
    Serial.println(start);
    int i = 0;                   // i denotes the number of times you want to send the data in one loop iteration
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
  delay(20);
}

void transmit_byte(char data_byte)
{
  digitalWrite(TRANSMIT_LED,LOW); 
  delay(SAMPLING_TIME);
  for(int i = 0; i < 8; i++)
  {
    digitalWrite(TRANSMIT_LED,(data_byte >> i) & 0x01);           // modulate the laser depending upon bit
    delay(SAMPLING_TIME);
  }
  digitalWrite(TRANSMIT_LED,HIGH);        
  delay(SAMPLING_TIME);
}
