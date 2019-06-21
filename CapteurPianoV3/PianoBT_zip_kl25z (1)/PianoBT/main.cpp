#include "mbed.h"
#include "LSM303D.h"

//DigitalOut myled(LED1);
Serial pc1(USBTX, USBRX);
Serial bt(PTE0,PTE1,115200);

SPI capteur(PTD2, PTD3, PTD1); // mosi, miso, sclk
SPI capteur2(PTD6, PTD7, PTD5); // 2ème liaison SPI disponible sur le microcontrôeur mosi, miso, sclk

LSM303D lsm1(capteur2,PTE31);//pouce haut
/*LSM303D lsm6(capteur2,PTA17);//pouce m
LSM303D lsm11(capteur2,PTA16);//pouce b */

LSM303D lsm2(capteur2,PTC17);//index h
/*LSM303D lsm7(capteur2,PTC16);//index m
LSM303D lsm12(capteur2,PTC13);//index b*/

LSM303D lsm3(capteur,PTC12);//majeur h
/*LSM303D lsm8(capteur2,PTC11);//majeur m
LSM303D lsm13(capteur2,PTC10);//majeur b*/

LSM303D lsm4(capteur,PTC6);//annulaire h
/*LSM303D lsm9(capteur,PTC5);//annulaire m
LSM303D lsm14(capteur,PTC4);//annulaire b*/

LSM303D lsm5(capteur,PTC3);//auriculaire h
/*LSM303D lsm10(capteur,PTC0);//auriculaire m
LSM303D lsm15(capteur,PTC7);//auriculaire b*/

float acc(LSM303D l); 
float accz(LSM303D l);
vector a1,m1;
int i = 0;

// Cette fonction permet de calculer la moyenne des 5 dernières valeurs transmises par les capteurs. 
// Moyenne des 5 dernières valeurs d'accélération sur l'axe X pour les 4 doigts(except le pouce)
float acc(LSM303D l) 
{
    float mx, moyx = 0;
    for (i = 0; i < 5; i++)
    {
        moyx += l.reada(a1);
    }   
    mx = moyx/5;    
    return mx;
}

// Cette fonction permet de calculer la moyenne des 5 dernières valeurs transmises par les capteurs. 
// Moyenne des 5 dernières valeurs d'accélération sur l'axe Z pour le pouce 
float accz(LSM303D l) 
{
    float mz, moyz = 0;
    for (i = 0; i < 5; i++)
    {
        l.reada(a1);
        moyz += a1.z;
    }   
    mz = moyz/5; 
    return mz;
}

int main() {    
    
    char note[5];
    
    lsm1.initialize();
    lsm2.initialize();
    lsm3.initialize();
    lsm4.initialize();
    lsm5.initialize();
   /* lsm6.initialize();
    lsm7.initialize();
    lsm8.initialize();
    lsm9.initialize();
    lsm10.initialize();
    lsm11.initialize();
    lsm12.initialize();
    lsm13.initialize();
    lsm14.initialize();
    lsm15.initialize(); */
   
    while(1) {
       // pouce haut DO
         if ((accz(lsm1)) < 0.1 ){        
            /*bt.printf("A57\r\n");  
            pc1.printf("A57/r/n");*/
            note[0]='A';
            note[1]='5';
            note[2]='7';
            note[3]='\r';
            note[4]='\n';            
            //wait(0.5);
         }
         
         // pouce bas 
         if ((accz(lsm1)) > 0.4  ){
            /*bt.printf("R57\r\n"); 
            pc1.printf("R57\r\n"); 
            wait(0.5);*/
         }
         
         // pouce et index haut DOd
         if ((accz(lsm1))<0.1 && (acc(lsm2))>0.5){        
           /* bt.printf("A58\r\n"); 
            wait(0.5);*/
            note[0]='A';
            note[1]='5';
            note[2]='8'; 
             note[3]='\r';
            note[4]='\n';
         }
         
         // pouce et index bas
         if ((accz(lsm1))>0.4 && (acc(lsm2))<0.1){        
           // bt.printf("R58\r\n");  
            //wait(0.5);
         }        
         
         // index haut RE
         if ((acc(lsm2)) > 0.5 ){
           // bt.printf("A59\r\n");
            //wait(0.5);  
            note[0]='A';
            note[1]='5';
            note[2]='9'; 
             note[3]='\r';
            note[4]='\n';
           // pc1.printf("A59\r\n"); 
         }
         
         // index bas
         if ((acc(lsm2)) < 0.1 ){
            //bt.printf("R59\r\n"); 
            //pc1.printf("R59\r\n");
            //wait(0.5);  
         }
         
         // Index et majeur haut REd
         if ((acc(lsm2)) > 0.5 && (acc(lsm3) > 0.5)){        
            //bt.printf("A60\r\n");
            //wait(0.5); 
            note[0]='A';
            note[1]='6';
            note[2]='0'; 
             note[3]='\r';
            note[4]='\n'; 
         }
         
         // index et majeur bas 
         if ((acc(lsm3)) < 0.1 && (acc(lsm2)) < 0.1){        
            //bt.printf("R60\r\n");
            //wait(0.5);  
         }
         
         // majeur haut MI
         if (acc(lsm3) > 0.5){
            //bt.printf("A61\r\n");  
            //pc1.printf("A61\r\n");
            //wait(0.5); 
            note[0]='A';
            note[1]='6';
            note[2]='1'; 
             note[3]='\r';
            note[4]='\n';
         }
         // majeur bas
         if ((acc(lsm3)) < 0.1 ){
            //bt.printf("R61\r\n"); 
            //pc1.printf("R61\r\n");
            //wait(0.5);  
         }         
         
         //annulaire haut FA
         if ((acc(lsm4)) > 0.5  ){
            //bt.printf("A62\r\n"); 
            //pc1.printf("A62\r\n");
            //wait(0.5);
            note[0]='A';
            note[1]='6';
            note[2]='2'; 
             note[3]='\r';
            note[4]='\n';  
         }
         // annulaire bas
         if ((acc(lsm4)) < 0.1  ){
            //bt.printf("R62\r\n");  
            //pc1.printf("R62\r\n");
            //wait(0.5); 
         }
         
          // majeur et ann haut FAd
         if ((acc(lsm4)) > 0.5 && (acc(lsm3) > 0.5)){        
            //bt.printf("A63\r\n"); 
            //wait(0.5);
            note[0]='A';
            note[1]='6';
            note[2]='3'; 
             note[3]='\r';
            note[4]='\n'; 
         }
         
         // majeur et ann bas 
         if ((acc(lsm4) < 0.1 && (acc(lsm3)) < 0.1)){        
            //bt.printf("R63\r\n"); 
            //wait(0.5); 
         }
         // auriculaire haut SOL
         if ((acc(lsm5) > 0.4 ) ){
            //bt.printf("A64\r\n"); 
            //pc1.printf("A64\r\n");
            //wait(0.5);
            note[0]='A';
            note[1]='6';
            note[2]='4'; 
             note[3]='\r';
            note[4]='\n';  
         }
         // auriculaire bas
         if ((acc(lsm5) < 0.1 ) ){
            //bt.printf("R64\r\n"); 
            //pc1.printf("R64\r\n");
            //wait(0.5);  
         }
         
           // auri et pouce haut SOLd
         if ((acc(lsm5) > 0.4 && (accz(lsm1)) < 0.1)){        
            //bt.printf("A65\r\n");
            //wait(0.5);
            note[0]='A';
            note[1]='6';
            note[2]='5'; 
             note[3]='\r';
            note[4]='\n';  
         }
         
         // auri et pouce bas 
         if ((acc(lsm5) < 0.1 && (accz(lsm1)) > 0.4)){        
            //bt.printf("R65\r\n");
            //wait(0.5);
          }
          // Ann et pouce haut LA
         if ((acc(lsm4) > 0.5 && (accz(lsm1)) < 0.1)){        
            //bt.printf("A66\r\n");  
            //wait(0.5);
            note[0]='A';
            note[1]='6';
            note[2]='6'; 
             note[3]='\r';
            note[4]='\n';
         }
         
         // Ann et pouce bas 
         if ((acc(lsm4) < 0.1 && (accz(lsm1)) > 0.4)){        
            //bt.printf("R66\r\n"); 
            //wait(0.5); 
         }   
         // Maj et pouce haut LAd
         if ((acc(lsm3) > 0.5 && (accz(lsm1)) < 0.1)){        
            //bt.printf("A67\r\n");  
            //wait(0.5);
            note[0]='A';
            note[1]='6';
            note[2]='7'; 
             note[3]='\r';
            note[4]='\n';
         }
         
         // Maj et pouce bas 
         if ((acc(lsm3) < 0.1 && (accz(lsm1)) > 0.4)){        
            //bt.printf("R67\r\n");
            //wait(0.5);
         }
            
         // index et auri haut SI
         if ((acc(lsm2) > 0.5 && (acc(lsm5)) > 0.4 )){        
            //bt.printf("A68\r\n"); 
            //wait(0.5);
            note[0]='A';
            note[1]='6';
            note[2]='8'; 
             note[3]='\r';
            note[4]='\n'; 
         }
         
         // index et auri bas 
         if ((acc(lsm2) < 0.1 && (acc(lsm4)) < 0.1 )){        
            //bt.printf("R68\r\n");     
            //wait(0.5);   
         } 
         bt.printf("%c%c%c%c%c",note[0],note[1],note[2],note[3],note[4]);
         wait(0.5);
         note[0]= 'R';  
         bt.printf("%c%c%c%c%c",note[0],note[1],note[2],note[3],note[4]);
      }
}



