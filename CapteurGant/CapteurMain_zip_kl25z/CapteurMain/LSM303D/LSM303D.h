/** LSM303D Interface Library
 *
 * Written by Eric Coyle
 *
 * Based on Michael Shimniok's LSM303DLH library which is based on
 * test program by @tosihisa and 
 * Pololu sample library for LSM303DLH breakout by ryantm:
 *
 * Copyright (c) 2011 Pololu Corporation. For more information, see
 *
 * http://www.pololu.com/
 * http://forum.pololu.com/
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef MBED_LSM303D_H
#define MBED_LSM303D_H
 
#include "mbed.h"
#include "vector.h"
 
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define FILTER_SHIFT 6      // used in filtering acceleromter readings

#define XAXIS 0x00
#define YAXIS 0x01
#define ZAXIS 0x02

 
class LSM303D {
public:
    LSM303D(SPI &spi, PinName CS);
    int whoami();
    
    //Initializes all accelerometer and magnetometer
    //Sets values to defaults
    int initialize();
    
    //Returns raw 16-bit magnetometer values for a given axis
    int magnitometer(int axis);
    
    //Returns raw 16-bit magnetometer values for a given axis
    int accelerometer(int axis);
    
    /** sets the x, y, and z offset corrections for hard iron calibration
     * 
     * Calibration details here:
     *  http://mbed.org/users/shimniok/notebook/quick-and-dirty-3d-compass-calibration/
     *
     * If you gather raw magnetometer data and find, for example, x is offset
     * by hard iron by -20 then pass +20 to this member function to correct
     * for hard iron.
     *
     * @param x is the offset correction for the x axis
     * @param y is the offset correction for the y axis
     * @param z is the offset correction for the z axis
    */
    void setOffset(float x, float y, float z);
        
    /** sets the scale factor for the x, y, and z axes
     *
     * Calibratio details here:
     *  http://mbed.org/users/shimniok/notebook/quick-and-dirty-3d-compass-calibration/
     *
     * Sensitivity of the three axes is never perfectly identical and this
     * function can help to correct differences in sensitivity.  You're
     * supplying a multipler such that x, y and z will be normalized to the
     * same max/min values
    */
    void setScale(float x, float y, float z);
    
    /** read the raw accelerometer and compass values
     *
     *@param a is the accelerometer 3d vector, written by the function
     *@param m is the magnetometer 3d vector, written by the function
    **/
    float reada(vector &a);
    void readm(vector &m);
    
    
    //returns the magnetic heading with respect to the y axis
    float heading(void);
        
    // returns the heading with respect to the specified vector
    float heading(vector from);
    
    int readStatus();
    
    int interrupt();
    
    union _data {
      short int raw;
      char b[2];
    } compass;
    
private:  
    DigitalOut cs;
    SPI &mspi;
    float _offset_x;
    float _offset_y;
    float _offset_z;
    float _scale_x;
    float _scale_y;
    float _scale_z;
    long _filt_ax;
    long _filt_ay;
    long _filt_az;
};
 
#endif