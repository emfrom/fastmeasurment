/* Simple sketch to read voltage in a useful manner for monitoring batteries and solar power circuits
 * 
 * Output works fine in serial plotter.
 *
 * It reads A0 as fast as possible (with analogRead that is) and print an average over a cycle
 * of length MILLIS_PER_CYCLE milliseconds. It also prints maximum and minimum for this period
 * as well as the global max and min. To reset the global min and max, reset the arduino.
 *
 * DONE for now
 * 
 * TODO: Reimplement to use ADC free running mode. The 2ms delay in analogRead slows things down a bit to much.
 */

#include "Arduino.h"

/*
 * Assuming a resitor divider is connected to A0
 *
 * Set both HIGH_ and LOW_ to 1 if source is to be measured directly
 *
 * IMPORTANT
 * Remember to use 10k protection resistor for direct connection
 * OR 
 * that the high resistor in the divider is >= 10k
 *
 */
#define HIGH_RESISTOR 10000
#define LOW_RESISTOR 1000

/* 
 * Macros to compute actual measured voltage
 */
#define AVCC 5.0f
#define ADC_RANGE 1024
#define MEASURMENT_CONVERSION (( ( AVCC / ADC_RANGE) * ( LOW_RESISTOR + HIGH_RESISTOR ) / LOW_RESISTOR ))

/*
 * How many milliseconds to measure over.
 */
#define MILLIS_PER_CYCLE 250


/*
 * setup doing it's arduino thing
 */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // 1337 speed
}

/*
 * The global min and max voltages encountered
 */
float global_max = 0;
float global_min = ADC_RANGE; //Return from analogRead() always smaller than 1024

/*
 * loop doing it's arduino thing
 */
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long voltage = 0;
  unsigned int voltage_max = 0;
  unsigned int voltage_min = ADC_RANGE; //Return from analogRead() always smaller than 1024

  //Counts the amount of measurments made per cycle
  unsigned int i = 0;
  
  //Targe time for this measurment cycle
  unsigned long stoptime = millis() + MILLIS_PER_CYCLE;

  //Reads A0 until time runs out
  while(millis() < stoptime) {
    unsigned int volt_reading = analogRead(A0);

    //Add the reading to voltage storage variable
    //Increment i to use for average later
    i++;
    voltage += volt_reading;

    //Check for new max
    if(volt_reading > voltage_max)
      voltage_max = volt_reading;

    //Check for new min
    if(volt_reading < voltage_min)
      voltage_min = volt_reading;
  }

  //Compute average over all the measurments
  voltage = voltage / i;

  //Check for new global max
  if(voltage_max > global_max)
    global_max = voltage_max;

  //Check for new global min
  if(voltage_min < global_min)
    global_min = voltage_min;


  //Print everything in a plotter friendly format
  Serial.print("Average:");
  Serial.print(voltage * MEASURMENT_CONVERSION);

  Serial.print("\tMin:");
  Serial.print(voltage_min * MEASURMENT_CONVERSION);

  Serial.print("\tMax:");
  Serial.print(voltage_max * MEASURMENT_CONVERSION);

  Serial.print("\tGlobal_Min:");
  Serial.print(global_min * MEASURMENT_CONVERSION);

  Serial.print("\tGlobal_Max:");
  Serial.print(global_max * MEASURMENT_CONVERSION);

  Serial.print("\n");

}
