//Cyril Dineen R00173956 CS3

//In this assessment I learnt how to use the simulator to run a temperature sensor to make LED's flash based on a potentiometer value
//I learnt that you have to save the values read from a peripheral to a variable so you can use it later on in the code
//I leant how to use sound alarms to indicate a certain threshold had been passed
//I unfortunately leant it is very important to keep copies of your code in a txt file in case the simulator crashes as it did multiple times when doing this assingment
//As we are using microncontrollers we had to keep the code as minimal as possible so I thought it was best practise to have all my code in the main function with the peripherals 
//(Temperature sensor, LED's) declared just before it
//I understood we had to make the yellow LED flash from 2 times a second to 5 times a second as it approached the high threshold value so a linear approach should have been followed
//I attempted to follow the linear approach by reducing the yellow LED flash delay by 60 ms everytime the for loop was run
//I did this by sketching a linear graph which demonstrated as time increased and the flash increased the delay should decrease hence why I tried to reduce the delay each time
//Unfortunatly I was unable to do this but left my effort commented in 

#include "mbed.h"
#include "Sht31.h"
#include <stdio.h>

//I add 2 Potentiometers at pin 15 and 16
AnalogIn Ain1(p15);
AnalogIn Ain2(p16);

//I add a temperature sensor
Sht31 sht31(I2C_SDA, I2C_SCL);

//I add 3 LED's Blue, Yellow and Red
DigitalOut ledBlue(p5);
DigitalOut ledYellow(p6);
DigitalOut ledRed(p7);

//I add a speaker at pin 21 for the alarm sounds
PwmOut speaker(p21);

///I create a play_tone method with parameters like volume and frequency 
void play_tone(float frequency, float volume, int interval, int rest) {
    speaker.period(1.0 / frequency);
    speaker = volume;
    wait(interval);
    speaker = 0.0;
    wait(rest);
}

int main() {
    while(1) {
        
        //I create a low and high threshold float variable based on the potentiometer value
        float lowerThreshold = Ain1.read();
        float higherThreshold = Ain2.read();

        //I convert the temperature value on the sensor to a float variable
        float actualTemp = sht31.readTemperature();

        //I divide it by 50 to scale it to the float values on the potentiometers
        //50 degrees is the max so when it is scaled down its value is 1.000
        float temp = actualTemp / 50;
        
        //I create a float variable for when the temperature is close to the high threshold
        float nearHighThreshold = higherThreshold - 0.250;
        
        //I create 4 float variables for when the temperature is getting closer to the high threshold so I can have different flash delay values
        float near1 = higherThreshold - 0.200;
        float near2 = higherThreshold - 0.150;
        float near3 = higherThreshold - 0.100;
        float near4 = higherThreshold - 0.050;
        
        //If the temperature is lower than the value on the lower potentiometer turn on the blue LED
        if(temp < lowerThreshold){
            //Leave the Yellow and Red LED's off
            ledBlue = 1;
            ledYellow = 0;
            ledRed = 0;
        }

        //If the temperature is higher than the lower potentiometer value but 
        // lower than the higher potentiometer value turn on the yellow LED with different delays in the if/else statements
        else if(temp >= lowerThreshold && temp <= higherThreshold){

            //Leave the Blue and Red LED's off
            ledBlue = 0;
            ledRed = 0;
            
            //Sound the alarm to indicate that the lowerThreshold has been breached
            play_tone(500.0, 0.01, 1, 2);
            
            //If the temp is above low value and less than 0.250 away from high value
            // flash the yellow LED 2 times a second
            if(temp >= lowerThreshold && temp < nearHighThreshold){
                    ledYellow = 1;
                    wait_ms(250);
                    ledYellow = 0;
                    
            }
            
            //If the temp is above low value and less than 0.200 away from high value
            // flash the yellow LED with a 212 millisecond delay
            else if(temp >= nearHighThreshold && temp < near1){
                ledYellow = 1;
                wait_ms(212);
                ledYellow = 0;
            }
            
            //If the temp is above low value and less than 0.050 away from high value
            // flash the yellow LED with a 175 millisecond delay
            else if(temp >= near1 && temp < near2){
                ledYellow = 1;
                wait_ms(175);
                ledYellow = 0;
            }
            
            //If the temp is above low value and less than 0.050 away from high value
            // flash the yellow LED with a 137 millisecond delay 
            else if(temp >= near2 && temp < near3){
                ledYellow = 1;
                wait_ms(137);
                ledYellow = 0;
            }  
            
            //If the temp is above low value and less than 0.050 away from high value
            // flash the yellow LED 5 times a second
            else if(temp >= near3 && temp < near4){
                ledYellow = 1;
                wait_ms(115);
                ledYellow = 0;
            }

            //If the temp is above low value and less than 0.050 away from high value
            // flash the yellow LED 5 times a second
            else if(temp >= near4 && temp < higherThreshold){
                ledYellow = 1;
                wait_ms(100);
                ledYellow = 0;
            } 
                
            //I attempted use a for loop to do the above steps to flash the led from 2 times a second to 5 times a second linearly
            //It failed but I am leaving my code commented to show my effort
                // int i;
                // for(i = 250; i > 100; i = i - 60){
                //     ledYellow = 1;
                //     wait_ms(i);
                //     ledYellow = 0;
                // }

            
            //if the temperature is lower than the lower threshold or higher than the higher threshold break out of this if/else
            else if(temp < lowerThreshold || temp >= higherThreshold){
                    ledYellow = 0;
                    break;
            }
            
        }

        //If the temp is higher than the higher threshold blink the Red LED will flash once every 2 seconds
        else if(temp >= higherThreshold){

                //Leave the Blue and Yellow LED's off
                ledBlue = 0;
                ledYellow = 0;
                
                //Sound the alarm to indicate that the higherThreshold has been breached
                play_tone(700.0, 0.01, 1, 0.5);
                
                ledRed = 0;
                wait_ms(2000);
                ledRed = 1;
                
                //if the temperature is lower than the higher threshold break out of this if/else
                if(temp < higherThreshold){
                    ledRed = 0;
                    break;
                }
        }
        wait(0.5);
    }
}
