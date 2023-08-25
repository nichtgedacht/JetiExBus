
#include "JetiExBusProtocol.h"
#include "DemoSensor.h"

JetiExBusProtocol exBus;
DemoSensor        demoSensor;

enum
{
  ID_VOLTAGE = 1,
  ID_ALTITUDE,
  ID_TEMP,
  ID_CLIMB,
  ID_FUEL,
  ID_RPM,
  ID_GPSLON,
  ID_GPSLAT,
  ID_DATE,
  ID_TIME,
  ID_VAL11, ID_VAL12, ID_VAL13, ID_VAL14, ID_VAL15, ID_VAL16, ID_VAL17, ID_VAL18,
};

// sensor definition (max. 31 for DC/DS-16)
// name plus unit must be < 20 characters
// precision = 0 --> 0, precision = 1 --> 0.0, precision = 2 --> 0.00
JETISENSOR_CONST sensors[] PROGMEM =
{
  // id             name          unit         data type             precision  priority
  { ID_VOLTAGE,    "Voltage",    "V",         JetiSensor::TYPE_14b, 1, 0 },
  { ID_ALTITUDE,   "Altitude",   "m",         JetiSensor::TYPE_14b, 0, 0 },
  { ID_TEMP,       "Temp",       "\xB0\x43",  JetiSensor::TYPE_14b, 0, 0 }, // °C
  { ID_CLIMB,      "Climb",      "m/s",       JetiSensor::TYPE_14b, 2, 0 },
  { ID_FUEL,       "Fuel",       "%",         JetiSensor::TYPE_14b, 0, 0 },
  { ID_RPM,        "RPM x 1000", "/min",      JetiSensor::TYPE_14b, 1, 0 },

  { ID_GPSLON,     "Longitude",  " ",         JetiSensor::TYPE_GPS, 0, 0 },
  { ID_GPSLAT,     "Latitude",   " ",         JetiSensor::TYPE_GPS, 0, 0 },
  { ID_DATE,       "Date",       " ",         JetiSensor::TYPE_DT,  0, 0 },
  { ID_TIME,       "Time",       " ",         JetiSensor::TYPE_DT,  0, 0 },

  { ID_VAL11,      "V11",        "U11",       JetiSensor::TYPE_14b, 0, 0 },
  { ID_VAL12,      "V12",        "U12",       JetiSensor::TYPE_14b, 0, 0 },
  { ID_VAL13,      "V13",        "U13",       JetiSensor::TYPE_14b, 0, 0 },
  { ID_VAL14,      "V14",        "U14",       JetiSensor::TYPE_14b, 0, 0 },
  { ID_VAL15,      "V15",        "U15",       JetiSensor::TYPE_14b, 0, 0 },
  { ID_VAL16,      "V16",        "U16",       JetiSensor::TYPE_14b, 0, 0 },
  { ID_VAL17,      "V17",        "U17",       JetiSensor::TYPE_14b, 0, 0 },
  { ID_VAL18,      "V18",        "U18",       JetiSensor::TYPE_14b, 0, 0 },
  { 0 } // end of array
};


void setup()
{
  // do not use these statements without serial monitor connected
  //Serial.begin( 115200 );
  //while (!Serial)
  //  ;

  exBus.SetDeviceId(0x76, 0x32); // 0x3276
  //exBus.Start("EX Bus", sensors, 2 ); // com port: 1..3 for Teeny, 0 or 1 for AtMega328PB UART0/UART1, others: not used
  exBus.Start ("mini_vario", sensors, 0);

  exBus.SetJetiboxText(0, "Line1");
  exBus.SetJetiboxText(1, "Line2");
}

void loop()
{

  char buf[30];

  // channel data
  if (false)
  // if ( exBus.HasNewChannelData() )
  {
    int i;
    for (i = 0; i < exBus.GetNumChannels(); i++)
    {
      sprintf(buf, "chan-%d: %d", i, exBus.GetChannel(i));
      Serial.println(buf);
    }
  }
  // get JETI buttons
  uint8_t bt = exBus.GetJetiboxKey();
  if( bt )
  {
    Serial.print( "bt - "); Serial.println(bt);
    }

  if (exBus.IsBusReleased())
  {
    // exBus is currently sending an ex packet
    // do time consuming stuff here (20-30ms)
    delay( 30 );
  }

  // sensor data, last parameter is valid flag
  exBus.SetSensorValue(ID_VOLTAGE, demoSensor.GetVoltage(),true);
  exBus.SetSensorValue(ID_ALTITUDE, demoSensor.GetAltitude(),true);
  exBus.SetSensorValue(ID_TEMP, demoSensor.GetTemp(), true);
  exBus.SetSensorValue(ID_CLIMB, demoSensor.GetClimb(), true);
  exBus.SetSensorValue(ID_FUEL, demoSensor.GetFuel(), true);
  exBus.SetSensorValue(ID_RPM, demoSensor.GetRpm(), true);

  exBus.SetSensorValueGPS(ID_GPSLON, true, 11.55616f, true); // E 11° 33' 22.176"
  exBus.SetSensorValueGPS(ID_GPSLAT, false, 48.24570f, true); // N 48° 14' 44.520"
  exBus.SetSensorValueDate(ID_DATE, 29, 12, 2015, true);
  exBus.SetSensorValueTime(ID_TIME, 19, 16, 37, true);

  exBus.SetSensorValue(ID_VAL11, demoSensor.GetVal(4), true);
  exBus.SetSensorValue(ID_VAL12, demoSensor.GetVal(5), true);
  exBus.SetSensorValue(ID_VAL13, demoSensor.GetVal(6), true);
  exBus.SetSensorValue(ID_VAL14, demoSensor.GetVal(7), true);
  exBus.SetSensorValue(ID_VAL15, demoSensor.GetVal(8), true);
  exBus.SetSensorValue(ID_VAL16, demoSensor.GetVal(9), true);
  exBus.SetSensorValue(ID_VAL17, demoSensor.GetVal(10), true);
  exBus.SetSensorValue(ID_VAL18, demoSensor.GetVal(11), true);

  // run protocol state machine
  exBus.DoJetiExBus();
}
