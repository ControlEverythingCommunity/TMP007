// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TMP007
// This code is designed to work with the TMP007_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Temperature?sku=TMP007_I2CS#tabs-0-product_tabset-2

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class TMP007
{
	public static void main(String args[]) throws Exception
	{
		// Create I2CBus
		I2CBus bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, TMP007 I2C address is 0x41(64)
		I2CDevice device = bus.getDevice(0x41);

		// Select configuration register
		// Continuous conversion, comparator mode
		byte[] config = {0x15, 0x40};
		device.write(0x02, config, 0, 2);

		// Read 2 bytes of data from address 0x03(3)
		// temp msb, temp lsb
		byte[] data = new byte[2];
		device.read(0x03, data, 0, 2);

		// Convert the data to 14-bits
		int temp = (((data[0] & 0xFF) * 256 + (data[1] & 0xFC)) / 4);
		if(temp > 8191)
		{
			temp -= 16384;
		}
		double cTemp = temp * 0.03125;
		double fTemp = cTemp * 1.8 + 32;

		// Output data to screen
		System.out.printf("Temperature in Celsius : %.2f C %n", cTemp);
		System.out.printf("Temperature in Fahrenheit : %.2f C %n", fTemp);
    }
}
