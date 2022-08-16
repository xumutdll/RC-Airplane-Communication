#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(9,10);
const byte adres[6] = "00001";

byte paket[5];
boolean flaps_bool;
byte throttle_u8, roll_u8, pitch_u8, yaw_u8, flaps_u8; 
float throttle_val_f32, roll_val_f32, pitch_val_f32, yaw_val_f32;
#define throttle_pin A0
#define roll_pin A2
#define pitch_pin A4
#define yaw_pin A6
#define flaps_pin 2



void setup()
{
  //Serial.begin(9600);
  
  radio.begin();
  radio.openWritingPipe(adres);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}

int pot_haritalama(int deger, int alt, int orta, int ust, bool degistir)
{
  
  deger = constrain(deger, alt, ust);
  
  if ( deger < orta )
    deger = map(deger, alt, orta, 0, 90);
  else
    deger = map(deger, orta, ust, 90, 180);

    
  return ( degistir ? 255 - deger : deger );
}

void loop()
{
  throttle_val_f32 = analogRead(throttle_pin);
  throttle_u8 = map(throttle_val_f32, 0, 1023, 0, 255);
  //throttle_u8 = pot_haritalama( analogRead(throttle_pin), 0,  420, 876, false );

  roll_val_f32 = analogRead(roll_pin);
  roll_u8 = map(roll_val_f32, 0, 1023, 20, 160); 
  //roll_u8     = pot_haritalama( analogRead(roll_pin),     0,  408, 795, false );
  
  pitch_val_f32 = analogRead(pitch_pin);
  pitch_u8    = pot_haritalama(pitch_val_f32,    0,  700, 1023, false );
  //pitch_u8 = map(pitch_val_f32, 0, 1023, 0, 180); 
  
  yaw_val_f32 = analogRead(yaw_pin);
  yaw_u8 = map(yaw_val_f32, 0, 1023, 0, 180);
  //yaw_u8      = pot_haritalama( analogRead(yaw_pin),      50, 451, 848, false );

  flaps_bool = digitalRead(flaps_pin);
  flaps_u8 = map(flaps_bool, 0, 1, 0, 255);

  
  paket[0] = throttle_u8;
  paket[1] = roll_u8;
  paket[2] = pitch_u8;
  paket[3] = yaw_u8;
  paket[4] = flaps_u8;

  
  //float x = *(float*)&throttle_f32;
  //Serial.println(paket[0]);
  
  radio.write(paket, 5);
}
