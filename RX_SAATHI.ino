// This code belongs to SAATHI 2.0 Project - HOME AUTOMATION FOR ALS PATIENTS THROUGH Li-Fi

// Library declarations
#include <SPI.h>
#include <Wire.h>

// Macros- for receiver
#define SOLAR_CELL_PIN A2
#define Sampling_delay 20


//  OLED Display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


//Declaration - for threshold setting using interrupt
const byte ledPin = 13;
const byte interruptPin1 = 2;
const byte interruptPin2 = 3;
volatile byte state = LOW;
volatile int change = 0;
int threshold = 600;

// Declaration - for reciever unit

const int RELAY_PIN = 5;
bool led_state = false;
bool previous_state = true;
bool current_state = true;
bool relay_state = true;
char buff[64];
char prev_char;
char curr_char;
unsigned int analog;

void setup() 
{
  // setup for receiver
  pinMode(SOLAR_CELL_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // setup for threshold
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("SAATHI 2.0");
  display.display();
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), increase, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), decrease, FALLING);
  
  Serial.begin(115200);  
}

void loop() 
{
  // ************************************************************
  //      CHANGE THRESHOLD - Depends upon lighting conditions 
  // ************************************************************
  if(change == 1)
  {
    threshold += 50;
    displaySaathi(threshold,analog);
  }
  else if(change == 2)
  {
    threshold -= 50; 
    displaySaathi(threshold, analog);  
  }
  change = 0;

  
  // ************************************************************
  //                            Data decode phase 
  // ************************************************************
  
  current_state = read_solar();   // read the solar value
  if(!current_state && previous_state)
  {
    curr_char = get_byte();
    sprintf(buff, "%c",curr_char);   
    if(curr_char == 'N' && prev_char == 'O')
    { 
      //Serial.println("GOT");
      if(relay_state)
      {
       digitalWrite(RELAY_PIN, HIGH);
       relay_state=false;
       delay(3000);
      }
      else
      {
       digitalWrite(RELAY_PIN, LOW);
       relay_state= true;
       delay(3000); 
      }
    }                       // successfully code RECEIVED
    prev_char = curr_char;
  }  // negative edge
  previous_state = current_state;
}


bool read_solar()
{
  int analog = analogRead(SOLAR_CELL_PIN);
  Serial.println(analog);
  bool val = analog > threshold ? true : false;
  return val;
}

char get_byte()
{
  char data_byte = 0;
  delay(Sampling_delay * 1.5);
  for(int i = 0; i < 8; i++)
  {
    data_byte = data_byte | (char)read_solar() << i;
    delay(Sampling_delay);
  }
  return data_byte;
}


void displaySaathi(int threshold, int analog)
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("SAATHI 2.0");
  display.setTextSize(1);
  display.setCursor(0,20);
  display.println("Threshold");
  display.setCursor(60,20);
  display.print(threshold);
  display.print(" ");
  display.print(analog);
  display.display();
}

void increase() {
  state = !state;
  change = 1;
}

void decrease() {
  state = !state;
  change = 2;
}
