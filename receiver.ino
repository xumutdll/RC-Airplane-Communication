#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(9,10); //(ce, csn)
const byte adres[6] = "00001";


byte gelen_paket_u8[5];
byte i;
const byte flaps_kapali_u8 = 178; // ölçüme göre değer atanacak
const byte flaps_acik_u8 = 58;

int throttle_int;

Servo throttle;
Servo roll; // TERS YÖN
Servo pitch;
Servo yaw;
Servo flaps; // AYNI YÖN

void setup()

{
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, adres);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();

  
  throttle.attach(2, 1000, 2000);
  roll.attach(3); //mavi D3 siyah bantlı
  pitch.attach(4);
  yaw.attach(5);
  flaps.attach(6);
  
  roll.write(90);
  pitch.write(90);
  yaw.write(90); 
  flaps.write(flaps_kapali_u8);
}

void loop()
{
  if (radio.available())
  {
    radio.read(gelen_paket_u8, 5);

    throttle_int = map(gelen_paket_u8[0], 0, 255, 1000, 2000);
    throttle.write(throttle_int);
    roll.write(gelen_paket_u8[1]);
    pitch.write(gelen_paket_u8[2]);
    yaw.write(gelen_paket_u8[3]);
    
    if (gelen_paket_u8[4] == 0)
    {
      flaps.write(flaps_kapali_u8);
    }
    else
    {
      flaps.write(flaps_acik_u8);
    }
    

    //float x = *(float*)&gelen_paket;
    //Serial.println(x);
    
    Serial.println("paket geldi");
    Serial.println(throttle_int);
    for ( i = 1 ; i < 5 ; i++)
    {
      Serial.println(gelen_paket_u8[i]);
    }
    
    //delay(50);

  }
}
