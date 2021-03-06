/*
 * This is part of a graduation project solution aims to build a 3d printer,
 * made by students at the AASTMT sheraton, Mechatronics department, 2016
 * https://github.com/afakharany93/Graduation-project-3D-printer
 *
 * Used UUgear for communication
 *
 * Authors:
 * Ahmed El Fakharany - afakharany93@gmail.com
 */
#ifndef UUGEAR_DAEMON_H_
#define UUGEAR_DAEMON_H_

#define DAEMON_NAME "uugear_daemon"

/* The daemon may need root privileges to access Arduino and message queue */
#define RUN_AS_USER "root"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define REQUEST_QUEUE_NAME  	"/uugear_request_queue"
#define RESPONSE_QUEUE_PREFIX	"/uugear_response_queue_"
#define MAX_QUEUE_NAME_LENGTH	32
#define MAX_MSG_PARTS		8
#define MAX_MSG_SIZE    	2048
#define MSG_PART_SEPARATOR	"]=["

#define COMMAND_START_CHAR		'U'
#define COMMAND_END_CHAR1		'\r'
#define COMMAND_END_CHAR2		'\n'
#define UUGEAR_DEVICE_PREFIX1		"ttyUSB"
#define UUGEAR_DEVICE_PREFIX2		"ttyACM"
#define UUGEAR_DEVICE_BAUD_RATE		115200
#define UUGEAR_ID_PREFIX			"UUGear-"
#define UUGEAR_ID_MAX_LENGTH		1024
#define UUGEAR_RESPONSE_START_CHAR	'\t'
#define UUGEAR_RESPONSE_END_STRING	":)"

//for sending 2 bytes of data , used for status byte
#define STATUS_BYTE_INITIAL_VAL                0x10
#define MOST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS   0x45
#define LEAST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS  0x38
#define DATA_BYTE_EQ_ZERO_SUBSTITUTE           0xFF
/* Messages between client and daemon */
#define MSG_EXIT    		0
#define MSG_GET_DEVICE  	1
#define MSG_CLOSE_DEVICE  	2

#define MSG_RESET_DEVICE		217
//stepper messages
#define MSG_STEPPER_MOVE        50
#define MSG_STEPPER_D_TIME      51      //for delay time between steps
#define MSG_STEPPER_GO_HOME     52
#define MSG_STEPPER_STOP        53
#define MSG_STEPPER_RESUME      54
#define MSG_STEPPER_STATUS      93
//temperature messages
#define MSG_TEMPERATURE_STATUS  56    //to get the temperature 
//heatbed messages
#define MSG_SET_HEATBED         60      //to set heatbed temperature
#define MSG_HEATBED_STATUS      61
 //extruder heat MESSAGES
#define MSG_SET_EXT_HEAT        65   //to set heatbed temperature
#define MSG_EXT_HEAT_STATUS     66
//EXTRUDER stepper messages
#define MSG_EXT_STEPPER_MOVE        40
#define MSG_EXT_STEPPER_D_TIME      41      //for delay time between steps
#define MSG_EXT_STEPPER_STOP        42
#define MSG_EXT_STEPPER_RESUME      43
#define MSG_EXT_STEPPER_STATUS      44


/* Commands between daemon and UUGear device */
#define CMD_GET_DEVICE_ID  		0x30

#define CMD_RESET_DEVICE		0xFF
//stepper commands
#define CMD_STEPPER_MOVE        0x60
#define CMD_STEPPER_D_TIME      0x61    //for delay time between steps
#define CMD_STEPPER_GO_HOME     0x62
#define CMD_STEPPER_STOP        0x63
#define CMD_STEPPER_RESUME      0x64
#define CMD_STEPPER_STATUS      0x65
//temperature commands
#define CMD_TEMPERATURE_STATUS  0x66    //to get the temperature 
//heatbed commands
#define CMD_SET_HEATBED         0x70    //to set heatbed temperature
#define CMD_HEATBED_STATUS      0x71
//extruder heat commands
#define CMD_SET_EXT_HEAT         0x75    //to set heatbed temperature
#define CMD_EXT_HEAT_STATUS      0x76
//stepper commands
#define CMD_EXT_STEPPER_MOVE        0x40
#define CMD_EXT_STEPPER_D_TIME      0x41    //for delay time between steps
#define CMD_EXT_STEPPER_STOP        0x42
#define CMD_EXT_STEPPER_RESUME      0x43
#define CMD_EXT_STEPPER_STATUS      0x44

#define ASSERT_TRUE(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
            perror(#x); \
            exit(EXIT_FAILURE); \
        } \
    } while (0) \

struct DeviceOpen
{
	int fd;			/* file describer for serial device */
	char *devName;	/* serial device name */
	char *id;		/* UUGear device id */
};

typedef struct DeviceOpen DeviceOpen;

#endif /* #ifndef UUGEAR_DAEMON_H_ */