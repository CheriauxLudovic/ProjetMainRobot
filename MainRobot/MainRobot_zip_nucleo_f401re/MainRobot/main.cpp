#include "mbed.h"
#include "PCA9685.h"

Serial pc(USBTX, USBRX);

I2C i2c_PCA(PB_9, PB_8); // Utilisation et déclaration d'un port I2C pour communiquer avec la carte PWM Adafruit
PCA9685 PCA_SERVO(ADDR, i2c_PCA, 50); 
Serial xbee(PA_9, PA_10); // Initialise XBEE serial carte NUCLEO

int i = 0, j = 0;
char data;

int reading[15];

int main() {

//pc.baud(9600);

    
PCA_SERVO.init(); //Initialisation des servomoteurs
    
for(i=0;i<=15;i++){ // On met tous les servomoteurs dans une position qui équivaut à la position de repos
     PCA_SERVO.set_pwm_output_on_0(i, 0);
}
 
xbee.baud(9600); // Initialise le baud à 9600
char reception;
xbee.printf("Bonjour XBEE !"); // Envoie d’une chaine de caractères pour verifier le bon établissement de la communication  
 
  while(1) 
  { 
  
         if(xbee.readable()==1) //Si un caractère peut être lu
        {
            reception = xbee.getc(); //la variable reception prend la valeur du caractère   
            pc.printf("reception : %c\n", reception);
            
            
            if(reception=='p'){           //Si le caractère reçu est égal à p               
               PCA_SERVO.set_pwm_pw(1,2200);
               PCA_SERVO.set_pwm_pw(2,2200);
               PCA_SERVO.set_pwm_pw(3,2400);               
            }
            
            if(reception=='o'){           //Si le caractère reçu est égal à o
                //PCA_SERVO.set_pwm_pw(1,1700);       
                PCA_SERVO.set_pwm_pw(2,1700);
                //PCA_SERVO.set_pwm_pw(3,1800);
                }  
                    
            if(reception=='q'){           //Si le caractère reçu est égal à q
               PCA_SERVO.set_pwm_pw(1,1300);
               PCA_SERVO.set_pwm_pw(2,1300);
               PCA_SERVO.set_pwm_pw(3,1300);  
            }   
            
            if(reception=='i'){           //Si le caractère reçu est égal à i               
               PCA_SERVO.set_pwm_pw(4,2400);
               PCA_SERVO.set_pwm_pw(5,2400);
               PCA_SERVO.set_pwm_pw(6,2400);               
            }
            
            if(reception=='h'){           //Si le caractère reçu est égal à h
                //PCA_SERVO.set_pwm_pw(4,1800);       
                PCA_SERVO.set_pwm_pw(5,1800);
               // PCA_SERVO.set_pwm_pw(6,1800);
                }  
                            
            if(reception=='j'){           //Si le caractère reçu est égal à j
               PCA_SERVO.set_pwm_pw(4,1300);
               PCA_SERVO.set_pwm_pw(5,1300);
               PCA_SERVO.set_pwm_pw(6,1300);               
            }   
            
            if(reception=='m'){           //Si le caractère reçu est égal à m               
               PCA_SERVO.set_pwm_pw(7,2400);
               PCA_SERVO.set_pwm_pw(8,2400);
               PCA_SERVO.set_pwm_pw(9,2400);               
            }
            
            if(reception=='l'){           //Si le caractère reçu est égal à l
                //PCA_SERVO.set_pwm_pw(7,1800);       
                PCA_SERVO.set_pwm_pw(8,1800);
               // PCA_SERVO.set_pwm_pw(9,1800);
                }  
                            
            if(reception=='n'){           //Si le caractère reçu est égal à n
               PCA_SERVO.set_pwm_pw(7,1300);
               PCA_SERVO.set_pwm_pw(8,1300);
               PCA_SERVO.set_pwm_pw(9,1300);          
            }            
            
            if(reception=='a'){           //Si le caractère reçu est égal à a               
               PCA_SERVO.set_pwm_pw(10,2400);
               PCA_SERVO.set_pwm_pw(11,2400);
               PCA_SERVO.set_pwm_pw(12,2400);               
            }
            
            if(reception=='c'){           //Si le caractère reçu est égal à c
                //PCA_SERVO.set_pwm_pw(10,1800);       
                PCA_SERVO.set_pwm_pw(11,1800);
                //PCA_SERVO.set_pwm_pw(12,1800);
                } 
             
            if(reception=='b'){           //Si le caractère reçu est égal à b
               PCA_SERVO.set_pwm_pw(10,1300);
               PCA_SERVO.set_pwm_pw(11,1300);
               PCA_SERVO.set_pwm_pw(12,1300);               
            }   
            
           if(reception=='u'){           //Si le caractère reçu est égal à u               
               PCA_SERVO.set_pwm_pw(13,2400);
               PCA_SERVO.set_pwm_pw(14,2400);
               PCA_SERVO.set_pwm_pw(15,2400);                
            }
            
            if(reception=='w'){           //Si le caractère reçu est égal à w
               // PCA_SERVO.set_pwm_pw(13,1800);       
                PCA_SERVO.set_pwm_pw(14,1800);
                //PCA_SERVO.set_pwm_pw(15,1800);
                }
             
            if(reception=='v'){           //Si le caractère reçu est égal à v
               PCA_SERVO.set_pwm_pw(13,1300);
               PCA_SERVO.set_pwm_pw(14,1300);
               PCA_SERVO.set_pwm_pw(15,1300);               
            }   
   
      
        }
        
    }
        
}




