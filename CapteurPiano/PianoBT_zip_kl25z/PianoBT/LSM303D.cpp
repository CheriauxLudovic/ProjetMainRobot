#include "LSM303D.h"
#include "mbed.h"


Serial pc(USBTX, USBRX);
//Serial xbee(PTE0,PTE1);  //Initalise XBEE serial
 
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
 

 
LSM303D::LSM303D(SPI &spi, PinName CS) : cs(CS), mspi(spi) {
    cs = 1;
}
 
int LSM303D::whoami() {
    int me;
    cs=0;   //talk to compass
    
    //Send who am I (first bit must be 1 for read)
    mspi.write(0x8F);
    
    //Get who am I response
    me = mspi.write(0x00);
    cs=1;   //done talking
    
    return me;
}

int LSM303D::initialize() {
    int iam;
    pc.baud(9600); 
    //Check device
    iam=whoami();
    if (iam==73) {
        
       
        //set up accelerometer
        //CTRL1
        cs=0;   //talk to compass
        mspi.write(0x20);
        mspi.write(0x67);  //100Hz, continuous update, all axes enabled
        cs=1;   //done talking
        
        
        //CTRL2
        cs=0;   //talk to compass
        mspi.write(0x21);
        mspi.write(0x00);  // 773Hz anti-alias filter, +/- 2g scale, self-test disabled, 4-wire SPI
        cs=1;   //done talking
        
        //CTRL3
        
        //set up magnetometer
        //CTRL5
        cs=0;   //talk to compass
        mspi.write(0x24);
        mspi.write(0x74);  //temperature disabled, high resolution, 100 Hz, int2 disable, int1 disabled
        cs=1;   //done talking
        
        
        //CTRL6 (Magnetic Scale)
        cs=0;   //talk to compass
        mspi.write(0x25);
        mspi.write(0x20);  //+/-4g
        cs=1;   //done talking
        
        //CTRL7 (filtering settings, and other)
        cs=0;   //talk to compass
        mspi.write(0x26);
        mspi.write(0x00); //normal mode, keep other on
        cs=1;   //done talking
        
        return 1;
    }
    else {
        //Not talking to right device
       //pc.printf("capteur NOK!!!!!");
        return 0;
    }
}

void LSM303D::setOffset(float x, float y, float z)
{
    _offset_x = x;
    _offset_y = y;
    _offset_z = z;   
}
 
void LSM303D::setScale(float x, float y, float z)
{
    _scale_x = x;
    _scale_y = y;
    _scale_z = z;
}


int LSM303D::magnitometer(int axis) {
    if (axis==0) {
        cs=0;   //lower cs to talk
        mspi.write(0x88);
        compass.b[0] = mspi.write(0x00);
        
        cs=1;   //done talking
        cs=0;   //lower cs to talk
    
        mspi.write(0x89);
        compass.b[1] = mspi.write(0x00);
    
        cs=1;   //done talking
    }
    else if (axis==1) {
        //Y-Axis
        cs=0;
        mspi.write(0x8A);
        compass.b[0] = mspi.write(0x00);
        
        cs=1;   //done talking
        cs=0;   //lower cs to talk
        
        mspi.write(0x8B);
        compass.b[1] = mspi.write(0x00);
        
        cs=1;   //done talking
    }
    else if (axis==2) {
        //Z-Axis
        cs=0;
        mspi.write(0x8C);
        compass.b[0] = mspi.write(0x00);
        
        cs=1;   //done talking
        cs=0;   //lower cs to talk
        
        mspi.write(0x8D);
        compass.b[1] = mspi.write(0x00);
        
        cs=1;   //done talking
    }
    return compass.raw;
}

int LSM303D::accelerometer(int axis) {
    if (axis==0) {
        cs=0;   //lower cs to talk
        mspi.write(0xA8);
        compass.b[0] = mspi.write(0x00);
        
        cs=1;   //done talking
        cs=0;   //lower cs to talk
    
        mspi.write(0xA9);
        compass.b[1] = mspi.write(0x00);
    
        cs=1;   //done talking
    }
    else if (axis==1) {
        //Y-Axis
        cs=0;
        mspi.write(0xAA);
        compass.b[0] = mspi.write(0x00);
        
        cs=1;   //done talking
        cs=0;   //lower cs to talk
        
        mspi.write(0xAB);
        compass.b[1] = mspi.write(0x00);
        
        cs=1;   //done talking
    }
    else if (axis==2) {
        //Z-Axis
        cs=0;
        mspi.write(0xAC);
        compass.b[0] = mspi.write(0x00);
        
        cs=1;   //done talking
        cs=0;   //lower cs to talk
        
        mspi.write(0xAD);
        compass.b[1] = mspi.write(0x00);
        
        cs=1;   //done talking
    }
    return compass.raw;
}


int LSM303D::interrupt()
{
    mspi.write(0x22);
    compass.b[0] = mspi.write(0x00);
    compass.b[1] = 0;
    return compass.raw;   
    
}


float LSM303D::reada(vector &a)
{
    short a_x, a_y, a_z;
    float acc;
 
    a_x=accelerometer(XAXIS);   
    a_y=accelerometer(YAXIS); 
    a_z=accelerometer(ZAXIS);
   
    
  
   a.x = a_x *0.000061;
   a.y = a_y *0.000061;
   a.z = a_z *0.000061;
   
   acc = sqrt (a.x * a.x + a.y *a.y + a.z*a.z);
   
   return a.x;
}
    
    
int LSM303D::readStatus()
{
    mspi.write(0xA7);
    compass.b[0] = mspi.write(0x00);
    compass.b[1] = 0;

    return compass.raw;   
}
    
    
void LSM303D::readm(vector &m)
{

    short m_x, m_y, m_z;
    
    m_x=magnitometer(XAXIS);
    m_y=magnitometer(YAXIS);
    m_z=magnitometer(ZAXIS);
    
    m.x = m_x * 0.00008;
    m.y = m_y * 0.00008;
    m.z = m_z * 0.00008;

}


/// Returns the number of degrees from the -Y axis that it
// is pointing.
float LSM303D::heading()
{
    return heading((vector){0,-1,0});
}
 
 
float LSM303D::heading(vector from)
{
    vector a, m;
    float heading;
 
    this->reada(a);
    this->readm(m);
 
 
    heading = (atan2(m.y, m.x)) * (180/M_PI);// Calculation for heading. North is zero degrees.
    
    if(heading < 0)
  
        heading += 2 * M_PI;
    
    return heading;
 

    
}