#include <NeoSWSerial.h>

#include <SoftwareSerial.h>
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.




SoftwareSerial serialgps(50,51); // pin 4 to GPS Tx y 3 to GPS Rx

int x, y;  //To save value written on serial monitor
//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value.

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"


void setup()
{
  pinMode(7,OUTPUT); //Pulse sensor voltage wire connected to pin 7
  serialgps.begin(9600);//GPS serial port begins
  Serial.begin(9600);//Serial Initialized
  Serial.println( F("Inicio de Programa\nIngresa un 1 para acceder al GPS\nIngresa un 2 para acceder al Sensor de pulso\nIngresa 3 para no realizar ninguna accion\n\n") );
                    // Program initialized\n Type 1 to access GPS\n Type 2 to access Pulse Sensor \n Type 3 to do nothing

// Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);
}

void loop()
{
  // Constantly read GPS characters.
  if(serialgps.available())
  {
    // Save new GPS data when it is available, once per second.
    fix = gps.read();
    Serial.println("New data");
  }

  // Check for a command
  //To acces the functions just one time when you type a number
  /*if (Serial.available()) {  
    char c=Serial.read();
    //Serial.println(c);

    switch (c) {
      case '1':
        digitalWrite(7, LOW);
        Serial.println( F(" GPS\n---Buscando senal--- ") ); //GPS\m Searching for signal
        localizar();
        break;

      case '2':
        digitalWrite(7, HIGH);
        Serial.println( F("Sensor de Pulso\nObteniendo Datos\n") ); //Pulse Sensor/n Obtaining data
        pulso();
        break;

      default:
        break;
    }
  }*/ 
  
  //Keep in the loop until another number is written
  x=Serial.read();
  //Serial.println(y);
  if(x==49)   // In this case 1=49
  {
    y=x;
    digitalWrite(7, LOW); //We set pin 7 to low so the Pulse Sensor dont consume energy
    Serial.println(" GPS\n---Buscando senal--- ");   //GPS\m Searching for signal
   }
    else if(x==50){ //2=50
    y=x;
    digitalWrite(7, HIGH);  // Pin 7 to High only when we access to Pulse Sensor Function
    Serial.println("Sensor de Pulso\nObteniendo Datos\n");}   //Pulse Sensor/n Obtaining data
    else if(x==48) // 3 = 48
    y=x;
    
  switch (y)
  {
    case 49: localizar();break; //GPS function begins  
    case 50: pulso();break; // Pulse Sensor function begins
  }
}

void localizar()
{
  // Print the current GPS data fields

  Serial.print( F("Latitud/Longitud: ") );
  if (fix.valid.location) {
    Serial.print( fix.latitude(),5 );
    Serial.print(", ");
    Serial.print( fix.longitude(),5);
  }
  Serial.println();

  Serial.print( F("Fecha: ") );
  if (fix.valid.date) {
    Serial.print(fix.dateTime.date); Serial.print("/");
    Serial.print(fix.dateTime.month); Serial.print("/");
    Serial.print(fix.dateTime.year);
  }
  Serial.println();

  Serial.print( F("Hora: ") );
  if (fix.valid.time) {
    Serial.print(fix.dateTime.hours); Serial.print(":");
    Serial.print(fix.dateTime.minutes); Serial.print(":");
    Serial.print(fix.dateTime.seconds);
    if (fix.dateTime_cs < 10)
      Serial.print( '0' ); // leading zero when .04 hundredths
    Serial.print('.');
    Serial.print(fix.dateTime_cs);
  }
  Serial.println();

  Serial.print( F("Velocidad(kmph): ") );
  if (fix.valid.speed)
    Serial.print(fix.speed_kph());
  Serial.println();

  Serial.println();
  delay(1000);
}


void pulso()
{
  pulseSensor.begin();
  int myBPM = pulseSensor.getBeatsPerMinute(); // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 

  if (pulseSensor.sawStartOfBeat()) {    // Constantly test to see if "a beat happened".
    Serial.println( F("♥  A HeartBeat Happened ! ") );   // If test is "true", print a message "a heartbeat happened".
    Serial.print("BPM: ");   // Print phrase "BPM: " 
    Serial.println(myBPM);     // Print the value inside of myBPM.
  }
}
