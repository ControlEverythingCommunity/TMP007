# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# TMP007
# This code is designed to work with the TMP007_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/content/Temperature?sku=TMP007_I2CS#tabs-0-product_tabset-2

import smbus
import time

# Get I2C bus
bus = smbus.SMBus(1)

# TMP007 address, 0x40(64)
# Select configuration register, 0x02(02)
#		0x1540(5440)	Continuous Conversion mode, Comparator mode
data = [0x1540]
bus.write_i2c_block_data(0x40, 0x02, data)

time.sleep(0.5)

# TMP007 address, 0x40(64)
# Read data back from 0x03(03), 2 bytes
# cTemp MSB, cTemp LSB
data = bus.read_i2c_block_data(0x40, 0x03, 2)

# Convert the data to 14-bits
cTemp = ((data[0] * 256 + (data[1] & 0xFC)) / 4)
if cTemp > 8191 :
	cTemp -= 16384
cTemp = cTemp * 0.03125
fTemp = cTemp * 1.8 + 32

# Output data to screen
print "Object Temperature in Celsius : %.2f C" %cTemp
print "Object Temperature in Fahrenheit : %.2f F" %fTemp
