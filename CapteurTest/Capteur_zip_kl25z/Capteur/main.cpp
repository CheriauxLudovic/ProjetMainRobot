#include "mbed.h"
#include "math.h"

// déclaration des prototypes
void Init_Cs(void);
void Write_Register(int cap, int addr, int valeur);
int Read_Register(int cap, int addr);
void Read_x(int cap);
void Read_y(int cap);
void Read_z(int cap);
void lecture(void);
void Capteur_present(void);

// programme extrait de V04OK
// le nbCapteur avril 2018
// La fréquence du bus SPI est fixée à 5MHz (elle vaut 1MHz par défaut)

#define nbCapteur 15

//Virtual serial port over USB
Serial pc(USBTX,USBRX,9600);
Serial xbee(PTE0,PTE1,9600);

// déclaration de la liaison SPI
// PTD1 est l'horloge de la 1ère liaison SPI   PTD5 de la 2ème liaison SPI
SPI capteur(PTD2, PTD3, PTD1); // mosi, miso, sclk
SPI capteur2(PTD6, PTD7, PTD5); // 2ème liaison SPI disponible sur le microcontrôeur mosi, miso, sclk

// declaration d'un timer
Ticker acquisition; //  creation d'un timer permettant de cadencer les acquisitions

// Variables globales

int acc_x[2];
int acc_y[2];
int acc_z[2];
char data[21];
float angleX, angleY, angleZ, total, angleX_old;

//Phalanges doigt 0
DigitalOut D0C1(PTA16);// Pouce bout
DigitalOut D0C2(PTA17);// Pouce millieu
DigitalOut D0C3(PTE31);// Pouce base

//Phalanges doigt 1
DigitalOut D1C1(PTC13);// Index bout
DigitalOut D1C2(PTC16);// Index millieu
DigitalOut D1C3(PTC17);// Index base

//Phalanges doigt 2
DigitalOut D2C1(PTC10);// Majeur bout
DigitalOut D2C2(PTC11);// Majeur millieu
DigitalOut D2C3(PTC12);// Majeur base

//Phalanges doigt 3
DigitalOut D3C1(PTC4);// Annulaire bout
DigitalOut D3C2(PTC5);// Annulaire millieu
DigitalOut D3C3(PTC6);// Annulaire base

//Phalanges doigt 4
DigitalOut D4C1(PTC7);// Auriculaire bout
DigitalOut D4C2(PTC0);// Auriculaire millieu
DigitalOut D4C3(PTC3);// Auriculaire base

// Structure pour mémoriser les 3 données des nbCapteur capteurs
struct S_accel{
    int x;
    int y;
    int z;
    DigitalOut *CS;  // pointeur pour les CS
}a[nbCapteur];
//
int main() {
   pc.baud(9600);  // débit liaison série virtuelle utilisée avec teraterm ou hyperterminal
  
    //  Configuration bus spi
    // Pour les accéléromètres, au repos, l'horloge est au niveau haut et la donnée est lue sur front montant de l'horloge (mode 3)
    // La donnée est codée sur 8 bits
    capteur.frequency(5000000);// Fréquence du bus SPI 5MHz - les capteurs supportent jusqu'à 10 MHz
    capteur.format(8,3);// Mode 3, donnée sur 8 bits
    capteur2.frequency(5000000);// Fréquence du bus SPI 5MHz - les capteurs supportent jusqu'à 10 MHz
    capteur2.format(8,3);// Mode 3 donnée sur 8 bits
    
    Init_Cs();//  Assignation des chip select
    
    Capteur_present();//  Verification de la presence des capteurs
    
    for (int i=0;i <6;i++){//  Autorisation des capteurs et réglage de la fréquence d'émission des données
                            // Ici 25 mesures par seconde (doc capteur page 37)
       Write_Register(i, 0x20, 0x47);}// registre CTRL_REG1_A AODR = 50Hz
      
    for (int i=6;i < nbCapteur;i++){//  Autorisation des capteurs et réglage de la fréquence d'émission des données
                              // Ici 25 mesures par seconde (doc capteur page 37)
       Write_Register(i, 0x20, 0x47);}// registre CTRL_REG1_A AODR = 50Hz - 50 mesures par seconde 
     pc.printf("\n");
   
    acquisition.attach(&lecture, 1.0);  // Lecture des données toutes les 1s
    
    while(1) {  
       /* int j;
        for( j=0;j < nbCapteur;j++){
           //  Lecture des axes
            Read_x(j);
            
            Read_y(j);
            
            Read_z(j);
            wait(3);
           }*/
           lecture();
           
           
             
    }
    return 0;
}

// détail des fonctions

// Déclarations des CS
void Init_Cs(void){
    //Phalanges doigt 0
    a[0].CS = &D0C1;
    a[1].CS = &D0C2;
    a[2].CS = &D0C3;

    //Phalanges doigt 1
    a[3].CS = &D1C1;
    a[4].CS = &D1C2;
    a[5].CS = &D1C3;

    //Phalanges doigt 2
    a[6].CS = &D2C1;
    a[7].CS = &D2C2;
    a[8].CS = &D2C3;

    //Phalanges doigt 3
    a[9].CS = &D3C1;
    a[10].CS = &D3C2;
    a[11].CS = &D3C3;

    //Phalanges doigt 4
    a[12].CS = &D4C1;
    a[13].CS = &D4C2;
    a[14].CS = &D4C3;
    
    // tous les esclaves de la liaison en haute impédance
    for (int i=0;i < nbCapteur;i++){// Tous les CS a 1
        *(a[i].CS) = 1;
    }
 }

// Ecriture dans un registre d'un accéléromètre
// cap=numéro du capteur de 0 à 14
// addr= adresse du registre (voir documentation)
// valeur = donnée à inscrire dans le registre
// le CS du capteur doit être mis à 0 pour accéder au capteur
void Write_Register(int cap, int addr, int valeur){
    //Ecriture dans un registre
    // cette partie si 2 bus SPI
    if(cap <6){
        *(a[cap].CS) = 0;
        capteur2.write(addr);
        capteur2.write(valeur|0x80); // dans le protocole SPI le bit de poids fort est mis à 1
        *(a[cap].CS) = 1;
    }
    else{
        //Ecriture dans un registre
        *(a[cap].CS) = 0;  // mise en basse impédance
        capteur.write(addr);
        capteur.write(valeur |0x80);
        *(a[cap]).CS = 1;  // mise en haute impedance
}
}
//Lecture d'un registre et affichage
// L'adresse de chaque registre s'écrit sur 6 bits 5..0 le bit N°7 doit être mis à 1 en lecture, le bit N°6 est à 0
int Read_Register(int cap, int addr){
    // Lecture d'un registre et affichage sur port comm
    // Cette partie si 2 bus SPI
    int term;
    if(cap <6){
        *(a[cap].CS) = 0;
        capteur2.write(addr | 0x80);
        term = capteur2.write(0x00);
        pc.printf("Registre = 0x%X\r\n", term);
        *(a[cap].CS) = 1;
        
    }else{
        *(a[cap].CS) = 0;
        capteur.write(addr | 0x80);  // écriture sur MOSI
        term = capteur.write(0x00);   // Lecture de MISO l'adresse est bidon
        pc.printf("Registre = 0x%X\r\n", term);
        *(a[cap]).CS = 1;
        
    }
    return term;
    }
    

//Lecture de la valeur de X pour un capteur
void Read_x(int cap){
   if(cap <6){    
        *(a[cap].CS) = 0;
        capteur2.write(0x28| 0x80);
        acc_x[0]= capteur2.write(0x00);
        //pc.printf("X_L = 0x%X\r\n", acc_x[0]);  //Affichacge LSB axe X
        *(a[cap].CS) = 1;

        *(a[cap].CS) = 0;
        capteur2.write(0x29| 0x80);
        acc_x[1] = capteur2.write(0x00);
       // pc.printf("X_H = 0x%X\r\n", acc_x[1]);  // Affichage MSB axe X
        *(a[cap].CS) = 1;

        a[cap].x = ((acc_x[1] <<8) | acc_x[0]);  //  Mise en forme valeur
       // xbee.printf("X = 0x%d\r\n", a[cap].x);  //  Affichage valeur axe x
        pc.printf("cap= %d  X = 0x%d\r\n",cap ,a[cap].x);  //  Affichage valeur axe x
        xbee.printf("cap= %d  X= %d\r\n",cap,a[cap].x);  //  Affichage valeur axe x
    }else{
    
        *(a[cap]).CS = 0;
        capteur.write(0x28 | 0x80);  // registre OUT_X_L_A  0x80 = 0b10000000
        acc_x[0]= capteur.write(0x00);   // lecture de la valeur
       // pc.printf("X_L = 0x%X\r\n", acc_x[0]);  //Affichacge LSB axe X
        *(a[cap].CS) = 1;
    
        *(a[cap].CS) = 0;
        capteur.write(0x29 | 0x80);  // registre OUT_X_H_A
        acc_x[1] = capteur.write(0x00);
        //pc.printf("X_H = 0x%X\r\n", acc_x[1]);  // Affichage MSB axe X
        *(a[cap].CS) = 1;
        
        a[cap].x = (acc_x[1] <<8)| acc_x[0];  //  Mise en forme valeur sur 16 bits
        pc.printf("cap= %d  X = %d\r\n", cap, a[cap].x);  //  Affichage valeur axe x
        xbee.printf("cap= %d  X=  %d\r\n",cap,a[cap].x);  //  Affichage valeur axe x
}
}

//Affichage Capteur Y
void Read_y(int cap){
        if(cap <6){    
            *(a[cap].CS) = 0;
            capteur2.write(0x2A| 0x80);// registre OUT_Y_L_A de l'accélerometre
            acc_y[0]= capteur2.write(0x00);
            //pc.printf("Y_L = 0x%X\r\n", acc_y[0]);  //Affichacge LSB axe Y
            *(a[cap].CS) = 1;

            *(a[cap].CS) = 0;
            capteur2.write(0x2B| 0x80); // registre OUT_Y_H_A
            acc_y[1] = capteur2.write(0x00);
            //pc.printf("Y_H = 0x%X\r\n", acc_y[1]);  // Affichage MSB axe Y
            *(a[cap].CS) = 1;

            a[cap].y = (acc_y[1] <<8)+ acc_y[cap];  //  Mise en forme valeur sur 16 bits
            xbee.printf("cap= %d  Y=  %d\r\n",cap,a[cap].y);  //  Affichage valeur axe Y
            pc.printf("cap= %d  Y = 0x%X\r\n",cap, a[cap].y);  //  Affichage valeur axe Y
    }else{
    
        *(a[cap].CS) = 0;
        capteur.write(0x2A|0x80); // registre OUT_Y_L_A
        acc_y[0]= capteur.write(0x00);
       // pc.printf("Y_L = 0x%X\r\n", acc_y[0]);  //Affichacge LSB axe Y
        *(a[cap].CS) = 1;

        *(a[cap].CS) = 0;
        capteur.write(0x2B|0x80); // registre OUT_Y_H_A
        acc_y[1] = capteur.write(0x00);
        //pc.printf("Y_H = 0x%X\r\n", acc_y[1]);  //Affichage MSB axe Y
        *(a[cap].CS) = 1;

        a[cap].y = (acc_y[1] <<8)| acc_y[cap];  //  Mise en forme valeur sur 16 bits
        pc.printf("cap= %d  Y = 0x%X\r\n",cap, a[cap].y);  //  Affichage valeur axe Y
       xbee.printf("cap= %d  Y=  %d\r\n",cap,a[cap].y);;  //  Affichage valeur axe Y
    }
    }

//Affichage Capteur Z
void Read_z(int cap){
   if(cap <6){    
        *(a[cap].CS) = 0;
        capteur2.write(0x2C|0x80); // registre OUT_Z_L_A
        acc_z[0]= capteur2.write(0x00);
        //pc.printf("Z_L = 0x%X\r\n", acc_z[0]);  //Affichacge LSB axe Z
        *(a[cap].CS) = 1;

        *(a[cap].CS) = 0;
        capteur2.write(0xAD);
        acc_z[1] = capteur2.write(0x00);
        //pc.printf("Z_H = 0x%X\r\n", acc_z[1]);  // Affichage MSB axe Z
        *(a[cap].CS) = 1;

        a[cap].z = (acc_z[1] <<8)| acc_z[0];  //  Mise en forme valeur
        xbee.printf("cap= %d  Z=  %d\r\n",cap,a[cap].z);  //  Affichage valeur axe Z
        pc.printf("cap= %d  Z = 0x%X\r\n",cap, a[cap].z);  //  Affichage valeur axe Z
    }else{
    
        *(a[cap].CS) = 0;
        capteur.write(0x2C|0x80); // registre OUT_Z_L_A
        acc_z[0]= capteur.write(0x00);
        //pc.printf("Z_L = 0x%X\r\n", acc_z[0]);  //Affichacge LSB axe Z
        *(a[cap].CS) = 1;

        *(a[cap].CS) = 0;
        capteur.write(0x2D|0x80);  // registre OUT_Z_H_A
        acc_z[1] = capteur.write(0x00);
        //pc.printf("Z_H = 0x%X\r\n", acc_z[1]);  // Affichage MSB axe Z
        *(a[cap].CS) = 1;

        a[cap].z = (acc_z[1] <<8)| acc_z[0];  //  Mise en forme valeur sur 16 bits
        pc.printf("cap= %d  Z = 0x%X\r\n",cap,a[cap].z);  //  Affichage valeur axe Z
        xbee.printf("cap= %d  Z=  %d\r\n",cap,a[cap].z);  //  Affichage valeur axe Z
    }
    }


void Capteur_present(void){// verification presence capteurs
int present;
    for (int i=0;i < 6;i++){
        present = Read_Register(i, 0x0F);  // registre "Who am I" de l'accéléromètre
        if( present == 0b01001001){      // valeur contenue par defaut 0x49
          pc.printf("capteur2 %X = OK\r\n", i);
        } else{
           pc.printf("capteur2 %X = Error\r\n", i);
        }
    }
    for (int i=6;i < nbCapteur;i++){
        present = Read_Register(i, 0x0F);
        if( present == 0b01001001){
           pc.printf("capteur_1 %X = OK\r\n", i);
        } else{
           pc.printf("capteur_1 %X = Error\r\n", i);
        }
    }
}
//
void lecture(void){
       int i;
        for( i=0;i < nbCapteur;i++){
            //  Lecture des axes
            Read_x(i);
            wait(0.1);
            Read_y(i);
            wait(0.1);
            Read_z(i);
            wait(0.1);
           }
            
          /* a[i].x = (a[i].x + 32768)/2;
            a[i].y = (a[i].y + 32768)/2;
            a[i].z = (a[i].z + 32768)/2;
            
            a[i].x = a[i].x /100;
            
            if(a[i].x >= 240)
            {
                a[i].x = 240;
            }
            if(a[i].x <= 180)
            {
                a[i].x = 180;
            }
            
            a[i].x = 180 - (a[i].x);
            a[i].x = -(a[i].x * 2);
            // pc.printf("%d\r\n", a[i].x);
            
            
            angleX = (0,0167 * a[i].x) - 300;
            
            angleX = (angleX_old + angleX) /2;
            angleX_old = angleX;


            
            // Ajout dans le tableau
            data[i] = a[i].x;
            pc.printf("%d\r\n", data[i]);
            
            xbee.printf("%c", data[i]);*/
            
        }
        
     




