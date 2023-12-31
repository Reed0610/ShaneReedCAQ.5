#include "mbed.h"
#include "LSM6DSLSensor.h"

#define PI 3.141592654

static DevI2C devI2c(PB_11, PB_10);
static LSM6DSLSensor acc_gyro(&devI2c, 0xD4, D4, D5);

float computeAngle(int x, int y, int z) {
    // Calculate the roll and pitch angles using accelerometer readings
    // Take the acceleration values in X, Y, and Z axes as inputs

    // Convert the acceleration values to floating-point numbers
    float accX = static_cast<float>(x);
    float accY = static_cast<float>(y);
    float accZ = static_cast<float>(z);

    // Calculate the roll angle using Equation 7
    float roll = atan2(accX, sqrt(accY * accY + accZ * accZ));

    // Calculate the pitch angle Equation 6
    float pitch = atan2(-accY, sqrt(accX * accX + accZ * accZ));

    // Convert the angles from radians to degrees
    float rollDegrees = roll * 180.0f / PI;
    float pitchDegrees = pitch * 180.0f / PI;

    // Check if the board is lying flat on the table
    if (rollDegrees > -2.0f && rollDegrees < 2.0f && pitchDegrees > -2.0f && pitchDegrees < 2.0f) {
        // Turn on LED 1
        DigitalOut led(LED1);
        led = 1;
    } else {
        // Turn off LED 1
        DigitalOut led(LED1);
        led = 0;
    }

    return pitchDegrees; // Return the pitch angle
    }


int main() {
    uint8_t id;
    int32_t axes[3];
    float res = 0;

    acc_gyro.init(NULL);

    acc_gyro.enable_x();
    acc_gyro.enable_g();

    printf("This is an accelerometer example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    acc_gyro.read_id(&id);
    printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);

    while (1) {
        // Read the accelerometer axes
        acc_gyro.get_x_axes(axes);

        // Compute the angle using the computeAngle function
        res = computeAngle(axes[0], axes[1], axes[2]);

        // Print the accelerometer values and the computed angle
        printf("LSM6DSL: %6d, %6d, %6d, %3.2f\r\n", axes[0], axes[1], axes[2], res);

        // Wait for 2 seconds
        ThisThread::sleep_for(2000ms);
    }
}
