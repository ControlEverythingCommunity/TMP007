// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TMP007
// This code is designed to work with the TMP007_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Temperature?sku=TMP007_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, TMP007 I2C address is 0x41(65)
	ioctl(file, I2C_SLAVE, 0x41);

	// Select configuration register(0x02)
	// Continuous conversion, comparator mode(0x15, 0x40)
	char config[3] = {0};
	config[0] = 0x02;
	config[1] = 0x15;
	config[2] = 0x40;
	write(file, config, 3);
	sleep(1);

	// Read 2 bytes of data from register(0x03)
	// temp msb, temp lsb
	char reg[1] = {0x03};
	write(file, reg, 1);
	char data[2] = {0};
	if(read(file, data, 2) != 2)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else
	{
		// Convert the data to 14-bits
		int temp = (data[0] * 256 + (data[1] & 0xFC)) / 4;
		if(temp > 8191)
		{
			temp -= 16384;
		}
		float cTemp = temp * 0.03125;
		float fTemp = cTemp * 1.8 + 32;

		// Output data to screen
		printf("Temperature in Celsius is : %.2f C \n", cTemp);
		printf("Temperature in Fahrenheit is : %.2f F \n", fTemp);
	}
}
