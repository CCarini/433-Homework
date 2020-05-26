#include "imu.h"
#include "i2c_master_noint.h"

void imu_setup() {
    unsigned char who = 0;
    
    // read from IMU_WHOAMI
    i2c_master_start();
    i2c_master_send(IMU_ADDR);
    i2c_master_send(IMU_WHOAMI);
    i2c_master_restart();
    i2c_master_send(IMU_ADDR | 0x01);
    who = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    
    if (who != 0b01101001) {
        while(1) {}
    }
    
    // init IMU_CTRL1_XL
    i2c_master_start();
    i2c_master_send(IMU_ADDR);
    i2c_master_send(IMU_CTRL1_XL);
    i2c_master_send(0b10000010);
    i2c_master_stop();
    
    // init IMU_CTRL2_G
    i2c_master_start();
    i2c_master_send(IMU_ADDR);
    i2c_master_send(IMU_CTRL2_G);
    i2c_master_send(0b10001000);
    i2c_master_stop();
    
    // init IMU_CTRL3_C
    i2c_master_start();
    i2c_master_send(IMU_ADDR);
    i2c_master_send(IMU_CTRL3_C);
    i2c_master_send(0b00000100);
    i2c_master_stop();
    
}

imu_read(unsigned char reg, signed short * data, int len) {
   unsigned char temp_array[len*2];
   i2c_master_read_multiple(IMU_ADDR, reg, temp_array, len*2);
   int i = 0;
   for (i; i < len*2; i++) {
       data[i] = ((temp_array[2*i+1] << 8) | temp_array[2*i]);
   }
}
