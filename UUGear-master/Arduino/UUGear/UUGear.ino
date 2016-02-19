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
 * 
 * Quick test: after uploading the sketch, send "U01" with both NL and CR in
 * the serial monitor, you should be able to see the 1 + {device id} + :)
 */
#include <EEPROM.h>
#include <Servo.h>

#define BAUD_RATE  115200

#define EEPROM_SIZE  1024

#define ID_PREFIX  "UUGear-Arduino-"

#define COMMAND_START_CHAR  'U'
#define COMMAND_END_STRING  "\r\n"

#define RESPONSE_START_CHAR  '\t'
#define RESPONSE_END_STRING  ":)"

#define SERVO_MAX_NUMBER  12



Servo allServos[SERVO_MAX_NUMBER];
int servoPins[SERVO_MAX_NUMBER];

String cmdBuf = "";
int cmdEndStrLen = strlen(COMMAND_END_STRING);



// declare reset function
void(* resetDevice) (void) = 0;

void setup() {
  // if has no id yet, generate one
  if (getID() == "") {
    generateID();
  }

  // initialize serial port
  Serial.begin(BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect
  }
  
  // initialize servo pins
  for (int i = 0; i < SERVO_MAX_NUMBER; i ++) {
    servoPins[i] = -1;
  }

  //Serial.println(getID());
}



void loop() {

  // listen to incoming commands
  int len = Serial.available();
  for (int i = 0; i < len; i ++) {
    cmdBuf += (char)Serial.read();
  }
  if (cmdBuf != "") {
    // drop useless data
    if (cmdBuf.charAt(0) != COMMAND_START_CHAR) {
      int pos = cmdBuf.indexOf(COMMAND_START_CHAR);
      if (pos != -1) {
        cmdBuf = cmdBuf.substring(pos);
      } 
      else {
        cmdBuf = "";
      }
    }
    // extract complete command
    if (cmdBuf != "") {
      int pos = cmdBuf.indexOf(COMMAND_END_STRING);
      if (pos != -1) {
        String cmd = cmdBuf.substring(0, pos + cmdEndStrLen);
        cmdBuf = cmdBuf.substring(pos + cmdEndStrLen);
        processCommand(cmd);
      }
    }
  }
}

// read ID from EEPROM
String getID() {
  String id = "";
  for (int i = 0; i < EEPROM_SIZE; i ++) {
    int value = EEPROM.read(i);
    if (value == 0xFF) {
      return id;
    }
    id += char(value);
  }
  return id;
}

// generate ID and save it into EEPROM
void generateID() {
  randomSeed(analogRead(0));
  int part1 = random(1000, 10000);
  int part2 = random(1000, 10000);
  String id = ID_PREFIX + String(part1) + "-" + String(part2);
  int len = id.length();
  for (int i = 0; i < EEPROM_SIZE; i ++) {
    EEPROM.write(i, i < len ? id.charAt(i) : 0xFF);
  }
}

// log string to serial, with hex format
void logAsHex(String str) {
  String hex = "";
  int len = str.length();
  for (int i = 0; i < len; i ++) {
    int val = str.charAt(i);
    if (val < 16) {
      hex += "0";
    }
    hex += String(val, HEX);
    hex += " ";
  }
  hex.toUpperCase();
  Serial.println(hex);
}



// process the command
void processCommand(String cmd) {
  //logAsHex(cmd);
  if (cmd.length() > 3) {
    byte cmdId = cmd.charAt(1);
    switch(cmdId) {
    case 0x30:
      cmdGetID(cmd);
      break;
    case 0x31:
      cmdPinModeOutput(cmd);
      break;
    case 0x32:
      cmdPinModeInput(cmd);
      break;
    case 0x33:
      cmdSetPinUp(cmd);
      break;
    case 0x34:
      cmdSetPinDown(cmd);
      break;
    case 0x35:
      cmdGetPinStatus(cmd);
      break;
    case 0x36:
      cmdAnalogWrite(cmd);
      break;
    case 0x37:
      cmdAnalogRead(cmd);
      break;
    case 0x38:
      cmdAnalogReference(cmd);
      break;
    case 0x40:
      cmdAttachServo(cmd);
      break;
    case 0x41:
      cmdWriteServo(cmd);
      break;
    case 0x42:
      cmdReadServo(cmd);
      break;
    case 0x43:
      cmdDetachServo(cmd);
      break;
    case 0x50:
      cmdReadDHT(cmd);
      break;
    case 0x51:
      cmdReadSR04(cmd);
      break;
    case 0xF0:
      break;
    case 0xFF:
      resetDevice();
      break;
    }
  }
}

// command to retrive the ID
// example: 55 30 01 0D 0A
void cmdGetID(String cmd) {
  if (cmd.length() > 4) {
    byte clientId = cmd.charAt(2);
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(getID());
    Serial.print(RESPONSE_END_STRING);
  }
}

// command to set pin as output
// example: 55 31 09 0D 0A
void cmdPinModeOutput(String cmd) {
  if (cmd.length() > 4) {
    byte pin = cmd.charAt(2);
    pinMode(pin, OUTPUT);
  }
}

// command to set pin as input
// example: 55 32 09 0D 0A
void cmdPinModeInput(String cmd) {
  if (cmd.length() > 4) {
    byte pin = cmd.charAt(2);
    pinMode(pin, INPUT);
  }
}

// command to set pin up
// example: 55 33 09 0D 0A
void cmdSetPinUp(String cmd) {
  if (cmd.length() > 4) {
    byte pin = cmd.charAt(2);
    digitalWrite(pin, HIGH);
  }
}

// command to set pin down
// example: 55 34 09 0D 0A
void cmdSetPinDown(String cmd) {
  if (cmd.length() > 4) {
    byte pin = cmd.charAt(2);
    digitalWrite(pin, LOW);
  }
}

// command to read pin status
// example: 55 35 09 01 0D 0A
void cmdGetPinStatus(String cmd) {
  if (cmd.length() > 5) {
    byte pin = cmd.charAt(2);
    byte clientId = cmd.charAt(3);    
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(String(digitalRead(pin)));
    Serial.print(RESPONSE_END_STRING);
  }
}

// command to write analog value
// example: 55 36 09 70 0D 0A
void cmdAnalogWrite(String cmd) {
  if (cmd.length() > 5) {
    byte pin = cmd.charAt(2);
    byte value = cmd.charAt(3);
    analogWrite(pin, value);
  }
}

// command to read analog value
// example: 55 37 04 01 0D 0A
void cmdAnalogRead(String cmd) {
  if (cmd.length() > 5) {
    byte pin = cmd.charAt(2);
    byte clientId = cmd.charAt(3);
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(String(analogRead(pin)));
    Serial.print(RESPONSE_END_STRING);
  }
}

// command to set analog reference
// example: 55 38 01 0D 0A
void cmdAnalogReference(String cmd) {
  if (cmd.length() > 4) {
    byte type = cmd.charAt(2);
    switch (type) {
      case 0x00:
        analogReference(DEFAULT);
        break;
      case 0x01:
        analogReference(EXTERNAL);
        break;
    }
  }
}

// command to attach given pin to servo
// example: 55 40 04 01 0D 0A
void cmdAttachServo(String cmd) {
  if (cmd.length() > 5) {
    byte pin = cmd.charAt(2);
    byte clientId = cmd.charAt(3);
    int servoIndex = -1;
    for (int i = 0; i < SERVO_MAX_NUMBER; i ++) {
      if (servoPins[i] == pin) {
        return;
      } else if (servoIndex == -1 && servoPins[i] == -1) {
        servoIndex = i;
      }
    }
    if (servoIndex != -1) {
      allServos[servoIndex].attach(pin);
      servoPins[servoIndex] = pin;
    }
  }
}

// command to write angle to servo on given pin
// example: 55 41 04 64 01 0D 0A
void cmdWriteServo(String cmd) {
  if (cmd.length() > 6) {
    byte pin = cmd.charAt(2);
    byte angle = cmd.charAt(3);
    byte clientId = cmd.charAt(4);
    for (int i = 0; i < SERVO_MAX_NUMBER; i ++) {
      if (servoPins[i] == pin) {
        allServos[i].write(angle);
        return;
      }
    }
  }
}

// command to read angle from servo on given pin
// example: 55 42 04 01 0D 0A
void cmdReadServo(String cmd) {
  if (cmd.length() > 5) {
    byte pin = cmd.charAt(2);
    byte clientId = cmd.charAt(3);
    int servoIndex = -1;
    for (int i = 0; i < SERVO_MAX_NUMBER; i ++) {
      if (servoPins[i] == pin) {
        Serial.write(RESPONSE_START_CHAR);
        Serial.write(clientId);
        Serial.print(String(allServos[i].read()));
        Serial.print(RESPONSE_END_STRING);
        return;
      }
    }
  }
}

// command to detach given pin from servo
// example: 55 43 04 01 0D 0A
void cmdDetachServo(String cmd) {
  if (cmd.length() > 5) {
    byte pin = cmd.charAt(2);
    byte clientId = cmd.charAt(3);
    for (int i = 0; i < SERVO_MAX_NUMBER; i ++) {
      if (servoPins[i] == pin) {
        allServos[i].detach();
        servoPins[i] = -1;
        return;
      }
    }
  }
}

// command to read DHT11/22 sensor
// example: 55 50 04 01 0D 0A
void cmdReadDHT(String cmd) {
  if (cmd.length() > 5) {
    byte pin = cmd.charAt(2);
    byte clientId = cmd.charAt(3);

    // pull down 18ms as start signal
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(18);
    
    // make sure no interrupts during the reading
    noInterrupts();

    // pull up to receive data
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);

    // now let's read 3 + 40 * 2 edges
    // first 3 edges are falling, raising and falling, as start
    // the following 80 edges are raising and falling, repeat 40 times, as 40 bits
    // none of the edges should let you wait longer than 80us
    unsigned long startTime;
    word buf = 0;
    uint8_t data[5];
    data[0] = data[1] = data[2] = data[3] = data[4] = 0;
    
    for (char i = -3, j = 0; i < 80; i ++) {
      byte age = 0;
      startTime = micros();
      // wait until expected edge shows up, response -1 for timeout
      while (digitalRead(pin) == (i & 1) ? HIGH : LOW) {
        age = micros() - startTime;
        if (age > 80) {
          // allow interrupts now
          interrupts();
          Serial.write(RESPONSE_START_CHAR);
          Serial.write(clientId);
          Serial.print("-1");
          Serial.print(RESPONSE_END_STRING);
          return;
        }
      }
      // save the bit (zero bit never be longer than 30us)
      if (i >= 0 && (i & 1)) {
        buf <<= 1;
        if (age > 30) {
          buf |= 1;
        }
      }
      // save the raw data, byte by byte
      if (i > 0 && (i + 1) % 16 == 0) {
        data[j] = buf & 0xFF;
        j ++;
        buf = 0;
      }
    }
    
    // allow interrupts now
    interrupts();

    // verify the checksum, response -2 for failure
    if ((0xFF & (data[0] + data[1] + data[2] + data[3])) != data[4]) {
      Serial.write(RESPONSE_START_CHAR);
      Serial.write(clientId);
      Serial.print("-2");
      Serial.print(RESPONSE_END_STRING);
      return;
    }

    // response with the result
    long result = ((long)data[0] << 24) + ((long)data[1] << 16) + ((long)data[2] << 8) + data[3];    
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(result);
    Serial.print(RESPONSE_END_STRING);
  }
}

// command to read HC-SR04 sensor
// example: 55 51 04 05 01 0D 0A
void cmdReadSR04(String cmd) {
  if (cmd.length() > 6) {
    byte trigPin = cmd.charAt(2);
    byte echoPin = cmd.charAt(3);
    byte clientId = cmd.charAt(4);
    // make sure no interrupts during the measuring
    noInterrupts();
    // trigger the sensor
    pinMode(trigPin, OUTPUT);
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);
    // read result
    pinMode(echoPin, INPUT);
    while (digitalRead(echoPin) == LOW) {}
    unsigned long start = micros();
    while (digitalRead(echoPin) == HIGH) {}
    unsigned long duration = micros() - start;
    // allow interrupts now
    interrupts();
    // calculate the distance in cm
    float result = (float)duration / 58.2;
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(result);
    Serial.print(RESPONSE_END_STRING);
  }
}


