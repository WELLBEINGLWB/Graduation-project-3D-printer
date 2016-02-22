from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

device = UUGearDevice('UUGear-Arduino-1239-9170')

if device.isValid():
	device.stepper_move(100, 800)
	
	device.detach()
	device.stopDaemon()
else:
	print 'UUGear device is not correctly initialized.'