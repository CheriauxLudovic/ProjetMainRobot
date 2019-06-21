#include "mbed.h"
#include "LSM303D.h"


//Serial pc(USBTX, USBRX);
Serial xbee(PTE0,PTE1);  //Initalise XBEE serial
char Key, reception;


SPI capteur(PTD2, PTD3, PTD1); // mosi, miso, sclk
SPI capteur2(PTD6, PTD7, PTD5); // 2ème liaison SPI disponible sur le microcontrôeur mosi, miso, sclk

LSM303D lsm1(capteur2,PTE31);//pouce haut
LSM303D lsm6(capteur2,PTA17);//pouce m
LSM303D lsm11(capteur2,PTA16);//pouce b

LSM303D lsm2(capteur2,PTC17);//index h
LSM303D lsm7(capteur2,PTC16);//index m
LSM303D lsm12(capteur2,PTC13);//index b

LSM303D lsm3(capteur,PTC12);//majeur h
LSM303D lsm8(capteur2,PTC11);//majeur m
LSM303D lsm13(capteur2,PTC10);//majeur b

LSM303D lsm4(capteur,PTC6);//annulaire h
LSM303D lsm9(capteur,PTC5);//annulaire m
LSM303D lsm14(capteur,PTC4);//annulaire b

LSM303D lsm5(capteur,PTC3);//auriculaire h
LSM303D lsm10(capteur,PTC0);//auriculaire m
LSM303D lsm15(capteur,PTC7);//auriculaire b

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
    
   xbee.baud(9600);
   // init des capteurs et vérification de leur état initial
   lsm1.initialize();//p1
   lsm2.initialize();//i1
   lsm3.initialize();//m1
   lsm4.initialize();//an1
   lsm5.initialize();//au1
   lsm6.initialize();//p2
   lsm7.initialize();//i2
   lsm8.initialize();//m2
   lsm9.initialize();//an2
   lsm10.initialize();//au2
   lsm11.initialize();//p3
   lsm12.initialize();//i3
   lsm13.initialize();//m3
   lsm14.initialize();//an3
   lsm15.initialize();//au3
    
    // Définition des commandes à transmettre via Xbee au robot. Ces commandes ne prennent en compte que les cas extrêmes : doigt tendu/plié.
    while(1)
     {
        // pouce1 haut
         if ((accz(lsm1)) < 0.1 ){        
            xbee.printf("a");  
         }
         //pouce1 milieu
         if ((accz(lsm1))>0.1 && (accz(lsm1))<0.4){
             xbee.printf("am");
             }
         // pouce1 bas
         if ((accz(lsm1)) > 0.4  ){
            xbee.printf("ab");  
         }
         // pouce2 haut
         if ((accz(lsm6)) < 0.1 ){        
            xbee.printf("b");  
         }
         //pouce2 milieu
         if ((accz(lsm6))>0.1 && (accz(lsm6))<0.4){
             xbee.printf("bm");
             }
         // pouce2 bas
         if ((accz(lsm6)) > 0.4  ){
            xbee.printf("bb");  
         }
         // pouce3 haut
         if ((accz(lsm11)) < 0.1 ){        
            xbee.printf("c");  
         }
         //pouce3 milieu
         if ((accz(lsm11))>0.1 && (accz(lsm1))<0.4){
             xbee.printf("cm");
             }
         // pouce3 bas
         if ((accz(lsm11)) > 0.4  ){
            xbee.printf("cb");  
         }
         
         
         // index1 haut
         if ((acc(lsm2)) > 0.5 ){
            xbee.printf("d");  
         }
         //index1 milieu
         if ((acc(lsm2))>0.1 && (acc(lsm2))<0.5){
             xbee.printf("dm");
             }
         // index1 bas
         if ((acc(lsm2)) < 0.1 ){
            xbee.printf("db");  
         }
         // index2 haut
         if ((acc(lsm7)) > 0.5 ){
            xbee.printf("e");  
         }
         //index2 milieu
         if ((acc(lsm7))>0.1 && (acc(lsm7))<0.5){
             xbee.printf("em");
             }
         // index2 bas
         if ((acc(lsm7)) < 0.1 ){
            xbee.printf("eb");  
         }
         // index3 haut
         if ((acc(lsm12)) > 0.5 ){
            xbee.printf("f");  
         }
         //index3 milieu
         if ((acc(lsm12))>0.1 && (acc(lsm12))<0.5){
             xbee.printf("fm");
             }
         // index3 bas
         if ((acc(lsm12)) < 0.1 ){
            xbee.printf("fb");  
         }
         
         
         // majeur1 haut
         if (acc(lsm3) > 0.5){
            xbee.printf("g");  
         }
         //majeur1 milieu
         if ((acc(lsm3))>0.1 && (acc(lsm3))<0.5){
             xbee.printf("gm");
             }
         // majeur1 bas
         if ((acc(lsm3)) < 0.1 ){
            xbee.printf("gb");  
         }
         // majeur2 haut
         if (acc(lsm8) > 0.5){
            xbee.printf("h");  
         }
         //majeur2 milieu
         if ((acc(lsm8))>0.1 && (acc(lsm8))<0.5){
             xbee.printf("hm");
             }
         // majeur2 bas
         if ((acc(lsm8)) < 0.1 ){
            xbee.printf("hb");  
         }
         // majeur3 haut
         if (acc(lsm13) > 0.5){
            xbee.printf("i");  
         }
         //majeur3 milieu
         if ((acc(lsm13))>0.1 && (acc(lsm13))<0.5){
             xbee.printf("im");
             }
         // majeur3 bas
         if ((acc(lsm13)) < 0.1 ){
            xbee.printf("ib");  
         }
         
         
         //annulaire haut
         if ((acc(lsm4)) > 0.5  ){
            xbee.printf("j");  
         }
         //annulaire milieu 
         if ((acc(lsm4))>0.1 && (acc(lsm4))<0.5){
             xbee.printf("jm");
             }
         // annulaire bas
         if ((acc(lsm4)) < 0.1  ){
            xbee.printf("jb");  
         }
         //annulaire haut
         if ((acc(lsm9)) > 0.5  ){
            xbee.printf("k");  
         }
         //annulaire milieu 
         if ((acc(lsm9))>0.1 && (acc(lsm9))<0.5){
             xbee.printf("km");
             }
         // annulaire bas
         if ((acc(lsm9)) < 0.1  ){
            xbee.printf("kb");  
         }
         //annulaire3 haut
         if ((acc(lsm14)) > 0.5  ){
            xbee.printf("l");  
         }
         //annulaire3 milieu 
         if ((acc(lsm14))>0.1 && (acc(lsm14))<0.5){
             xbee.printf("lm");
             }
         // annulaire3 bas
         if ((acc(lsm14)) < 0.1  ){
            xbee.printf("lb");  
         }
         
         
         // auriculaire1 haut
         if ((acc(lsm5) > 0.4 ) ){
            xbee.printf("m");  
         }
         //auriculaire1 milieu
         if ((acc(lsm5))>0.1 && (acc(lsm5))<0.4){
             xbee.printf("mm");
             }
         // auriculaire1 bas
         if ((acc(lsm5) < 0.1 ) ){
            xbee.printf("mb");  
         }
         // auriculaire2 haut
         if ((acc(lsm10) > 0.4 ) ){
            xbee.printf("n");  
         }
         //auriculaire2 milieu
         if ((acc(lsm10))>0.1 && (acc(lsm10))<0.4){
             xbee.printf("nm");
             }
         // auriculaire3 bas
         if ((acc(lsm10) < 0.1 ) ){
            xbee.printf("nb");  
         }
         // auriculaire3 haut
         if ((acc(lsm15) > 0.4 ) ){
            xbee.printf("o");  
         }
         //auriculaire3 milieu
         if ((acc(lsm15))>0.1 && (acc(lsm15))<0.4){
             xbee.printf("om");
             }
         // auriculaire3 bas
         if ((acc(lsm15) < 0.1 ) ){
            xbee.printf("ob");  
         }
     
         
         
         
    }   
}   
   
  

              
  