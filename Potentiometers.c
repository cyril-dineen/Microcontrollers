#include "mbed.h"
#include "Sht31.h"

//Temperature and Humidity scale set as inputs
//Sht31 sht31(I2C_SDA, I2C_SCL);

AnalogIn lowerPotentiometer(p15);
AnalogIn higherPotentiometer(p16);

PwmOut ledBlue(p20);
PwmOut ledYellow(p21);
PwmOut ledRed(p22);



int main() {
    //Print statements to explain what lights should be on depending on temperature
    printf("If Temp < 30 degrees Blue LED is continuously on\n");
    printf("If Temp > 30 degrees : Low threshold warning LED triggered\n");
    printf("If Temp near 45 degrees : Proximity to high threshold warning LED triggered\n");
    printf("If Temp > 45 degrees : High threshold warning LED triggered\n");
    
    while(1) {
        lowerPotentiometer.read();
        higherPotentiometer.read();
        
        float lowerThreshold = 0.3;
        float higherThreshold = 0.45;
        
        
        //The Blue LED is on whenever the threshold is not crossed
        //No other LED's are on at this stage
        if(lowerPotentiometer < lowerThreshold){
            ledBlue = 1;
            ledYellow = 0;
            ledRed = 0;
        }
        
        //The Yellow LED is on whenever the lower threshold is crossed (Above 30 degrees)
        //(There are 2 yellow LED settings: Flash twice per sec or flash 5 times per sec)
        else if(lowerPotentiometer > lowerThreshold && lowerPotentiometer < higherThreshold){
            //When the passes the lower threshold the LED flashes 2 times per second
            if(lowerPotentiometer >= lowerThreshold && lowerPotentiometer < 0.40){

                ledYellow = !ledYellow;
                wait(0.5f);
            }
            //When the temp nears the high threshold(45) the LED flashes 5 times per second
            else if (lowerPotentiometer >= 0.40 && lowerPotentiometer < 0.45){
                ledYellow = !ledYellow;
                wait(0.2f);
            }
            ledBlue = 0;
            ledRed = 0;
    
        }
        //The Red LED is on whenever the higher threshold is crossed (Above 45 degrees)
        //(The Red LED is set to flash 2 times per sec)
        else if(lowerPotentiometer >= higherThreshold){
            ledRed = !ledRed;
            ledYellow = 0;
            ledBlue = 0;
            wait(0.5f);
        }
            
        wait(0.5f);
    }
}



    

