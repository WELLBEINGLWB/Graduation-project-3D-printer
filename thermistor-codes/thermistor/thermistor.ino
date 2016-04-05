#include <math.h>

#define SERIES_RESISTOR		10000.0		//the value of the resistor in series with the thermistor in ohms

#define AVERGAE_SAMPLES		200			//used to hold the number of samples that are averaged

#define BETA_PARAMETER		3950.0	

#define	NOMINLA_THERMISTOR_VALUE	100000.0
#define NOMINAL_TEMPERATURE			298.15		//kelvin, 25 Celsius

void setup() {
  Serial.begin(115200);

}

void loop() {
	
	Serial.print("Thermistor temp :"); 
	Serial.println(temperature_measurment());
  	delay(100);

}


double thermistor_measurment()
{
	int ADC_value;		//used to hold the value reading from the ADC
	double thermistor_resistance_value;		//used to hold the value of the resistance of the thermistor 
	long int average = 0;		//used to hold the averaged value
	int pin = A0;

	for (int i = 0; i < AVERGAE_SAMPLES; i++ )
	{
		ADC_value = analogRead(pin);
		//Serial.print("Analog reading :"); 
	  	//Serial.println(ADC_value);
	
		average += ADC_value;	
	}
	average = average / AVERGAE_SAMPLES;
	thermistor_resistance_value = (SERIES_RESISTOR * (float)average) / (1023.0 - (float)average);
	return thermistor_resistance_value;
}

float temperature_measurment()
{
	double resistance;
	float temperature;
	resistance = thermistor_measurment();
	temperature = (1.0/NOMINAL_TEMPERATURE) + ((1.0/BETA_PARAMETER) * log(resistance / NOMINLA_THERMISTOR_VALUE));
	temperature = (1.0 / temperature) - 273.15;	//Celsius
	return temperature;
}

