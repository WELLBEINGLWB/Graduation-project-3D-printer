/*
 * UUGear Solution: extend your Raspberry Pi with Arduino
 *
 * Author: Shawn (shawn@uugear.com)
 *
 * Copyright (c) 2014 UUGear s.r.o.
 *
 ***********************************************************************
 *  This file is part of UUGear Solution: 
 *  http://www.uugear.com/uugear-rpi-arduino-solution/
 *  
 *  UUGear Solution is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  UUGear Solution is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with UUGear Solution.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************
 */
#include "UUGearDaemon.h"

struct UUGearDevice
{
	char id[UUGEAR_ID_MAX_LENGTH];
	int clientId;
	int fd;
	mqd_t in;
	mqd_t out;
};

typedef struct UUGearDevice UUGearDevice;

extern void setShowLogs (int show);

extern void setupUUGear ();

extern struct UUGearDevice attachUUGearDevice (char *id);


extern void detachUUGearDevice (UUGearDevice *dev);

extern void resetUUGearDevice(UUGearDevice *dev);


extern void cleanupUUGear();


extern void sendMessage(mqd_t in, int msgType, int clientId, int fd, int pin);

extern void sendMessageWithParameter(mqd_t in, int msgType, int clientId, int fd, int pin, int parameter);

extern void send_message_with_3_data_bytes(mqd_t in, int msgType, int clientId, int fd,, int data_byte_1, int data_byte_2 , int data_byte_3);

extern int waitForInteger(UUGearDevice *dev, int * errorCode);

extern char * waitForString(UUGearDevice *dev, int * errorCode);

extern float waitForFloat(UUGearDevice *dev, int * errorCode);

extern void stepper_move(UUGearDevice *dev, short steps);