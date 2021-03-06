/* some hints for usage:
The Arduino has 3Timers and 6 PWM output pins. The relation between timers and PWM outputs is:

Pins 5 and 6: controlled by timer0
Pins 9 and 10: controlled by timer5
Pins 11 and 3: controlled by timer2

On the Arduino Mega we have 6 timers and 15 PWM outputs:

Pins 4 and 13: controlled by timer0
Pins 11 and 12: controlled by timer5
Pins 9 and 10: controlled by timer2
Pin 2, 3 and 5: controlled by timer 3
Pin 6, 7 and 8: controlled by timer 4
Pin 46, 45 and 44:: controlled by timer 5
*/

#ifndef _EXT_STEPPER_3D_
#define _EXT_STEPPER_3D_

#define EXTRUDER  1

#if EXTRUDER
#if defined(__AVR_ATmega2560__)|| defined(__AVR_ATmega1280__)	//if arduino mega is used
#include "Arduino.h"
#include "Stepper_3D.h"
#include <avr/interrupt.h>


//defining the direction of the change in states
#define NEXT 			1
#define PREVIOUS 		0
//statuses
#define MOVE 			1
#define END_MOVE		2
#define	SW_FORCE_STOP	3
#define	RESUME_AF_STOP	4
#define	FLOW			5
//enstop states
#define NOTHING_PRESSED	0
#define HOME_PRESSED	1
#define AWAY_PRESSED	2

/*struct stepper_state_struct is a struct used to hold the info concerning the states, each state resembles one step,
it holds the output of the state and a pointer to the next state to use to step forward and
a pointer to the previous step in case of backwards */

/*typedef struct stepper_state_struct
{
	unsigned char out;						//used to hold the output values to all pins, this value needs to be interpreted later
 	struct stepper_state_struct	*nxt;		//used to hold a pointer to the next state which resembles the next step on the stepper motor, used for forward motion
	struct stepper_state_struct	*prev;		//pointer used to hold the address to the previous state, used for backwards motion
} stepper_state_struct;*/
/* struct timer5_value is used in the lookup table used to determine the value of the prescale and the determination of the value of OCR1A register
 to be able to operate with the timer5 as agile as possible */
typedef struct timer5_value
{
	unsigned int prescale;				//to hold the value of the prescale
	float time_per_tick_us;				//to hold the value of time needed to make one tick in the timer in micro seconds
	unsigned long int max_period_us;	//to hold the max value the timer can hold in micro seconds
}timer5_value;

class ext_stepper_3d
{
	public:		//what the user has access to
		ext_stepper_3d ();	//constructor

		//mapping the pins with wire colors
		unsigned char black = 3;
		unsigned char blue  = 5;
		unsigned char red   = 4;
		unsigned char green = 2;
		unsigned char brown;
		unsigned char orange;
		unsigned char yellow;
		unsigned char white;
		//mapping wire colors with firing order
		unsigned char first  = green;
		unsigned char second = red;
		unsigned char third  = black;
		unsigned char forth  = blue;
		//mapping directions motion with state transition direction
		unsigned char clockwise     = PREVIOUS;
		unsigned char anticlockwise = NEXT;
		unsigned char forward       = clockwise;
		unsigned char backward      = anticlockwise;
		unsigned char min_pwm=0;
		unsigned char max_pwm=255;
		unsigned char brake=0;
		

		//time variable
		unsigned long int time_bet_steps_us = 1000 ;

		//permission handler
		unsigned char permission = 1;		//used to prevent stepper_move function from overwriting itself, to execute stepper_move set it to 1, to stop the overwriting set it to 0

		//status holding variable
		unsigned char status_var = END_MOVE;

		/*
			Function name : stepper_move
		  	return : void
		  	parameters : long int steps :- variable to hold the value of the number of steps needed to be move, if +ve the motion is in a direction, if -ve the motion is in reverse
		  				 unsigned long int time_bet_steps_stepper :- time between each step, this is the value that determines the speed of motion
		  	Method of operation : its a non blocking function, used to move the stepper motor when it has permission i.e.(stepper_permission == 1), 
		  						  it sets the values of the global variables direction and stepper_steps, and sends the parameters needed to the timer setup function
		*/
		void stepper_move (long int steps, unsigned long int time_bet_steps_stepper );

		/*
			Function name : stepper_stop
		  	return : void
		  	parameters : none
		  	Method of operation : it stops - or pauses- the motion of the stepper motor, it actually stops the timer interrupt since the timer ISR is the function that does the motion 
		*/
		void stepper_stop ();

		/*
			Function name : stepper_resume
		  	return : void
		  	parameters : none
		  	Method of operation : it is used to resume the motion after stopping
		*/
		void stepper_resume ();

		/*
			Function name : stepper_flow
		  	return : void
		  	parameters : unsigned char direction_flow :- to determine the direction of motion
		  	Method of operation : it is used to move the stepper motor in a very big number of steps that it seems it moves for infinityyyyy, but it doesn't, 
		  						  best used for moving the stepper motor till a physical event happens to stop it
		*/
		void stepper_flow (unsigned char direction_flow);
		/*
			Function name : inside_ISR
		  	return : void
		  	parameters :void
		  	Functionality : this function is to be called inside the timer ISR function, it the function responsible for doing the motion everytime the ISR runs
		*/
		void inside_ISR () ;
	
		/*
			Function name : change_rotation_direction
		  	return : void
		  	parameters :void
		  	Functionality : if the motor rotates in the other direction than the one specified - given to a function - in all times and all calls, just use 
		  					this function to correct the rotation direction
		*/
		void change_rotation_direction_mapping();
		/*
			Function name : change_rotation_direction
		  	return : void
		  	parameters :void
		  	Functionality : if the motor when connected to a linear actuator moves in the other direction than the one specified - given to a function - in all times and all calls, 
		  					just use this function to correct the motion direction
		*/
		void change_linear_direction_mapping();

		char * stepper_status();

	private:	//stuff under the hood, the user shouldn't bither himself with
		/*stepper_states is an array that holds the constant values of all the states of the stepper motor */
		struct stepper_state_struct stepper_states[8] = 
		{
		//states {out  , next state         , previos state      }
		/*0*/	 {0x01 , &stepper_states[1] , &stepper_states[3] } ,
		/*2*/	 {0x02 , &stepper_states[2] , &stepper_states[0] } ,
		/*4*/	 {0x04 , &stepper_states[3] , &stepper_states[1] } ,
		/*6*/	 {0x08 , &stepper_states[0] , &stepper_states[2] } ,
		};

		/* timer5_value_lookup_table is an array that holds the values of the prescales and the time per tick and the max time value for each prescale */
		struct timer5_value timer5_value_lookup_table[5] = 
		{
		 //{prescale , time_per_tick_us , max_period_us}
			{1 	  	, 0.0625  			, 4096   	   },
			{8 	  	, 0.5 				, 32767  	   },
			{64   	, 4 	 			, 262140 	   },
			{256  	, 16	 			, 1048560	   },
			{1024 	, 64 				, 4194240	   }
		};

		struct stepper_state_struct current_state;		//the variable that will hold the current state information, initialized with state zero info
		unsigned long int 	stepper_steps = 0;			//this variable holds the number of steps remained to be moved, needed by the isr
		unsigned char 		direction;				//this variable holds the direction of movement, needed by the isr

		/*Function name : stepper_output
		  return : void
		  -	parameters : struct stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
	
		  Functionality : to  output the ouy member in the current_state struct to the pins, use after next_step or previos_step functions, runs after next_step or previos_step
		 */
		void stepper_output (struct stepper_state_struct *current_state,unsigned char pwm);
		/*
			Function name : next_step
		  	return : void
		  	parameters : struct stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
		  	Functionality : To make resdy the stepper to take the next step in a direction
		*/
		void next_step (struct stepper_state_struct *current_state);
		/*
			Function name : previos_step
		  	return : void
		  	parameters : struct stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
		  	Functionality : to ready the stepper to take the previous step in a direction opposite to that of next_step function
		*/
		void previos_step (struct stepper_state_struct *current_state);

		/*
			Function name : prescale_determination
		  	return : struct timer5_value * :- pointer to the element of the timer5_value_lookup_table with the right prescale
		  	parameters : struct timer5_value *timer5_value_lookup_table_ptr_for_prescale :- pointer to the timer5_value_lookup_table array
		  				 unsigned long int time_bet_steps_for_prescale :- used to hold the time between each step in microseconds
		  	Method of operation : it searches the timer5_value_lookup_table array for the suitable prescale and returns a pointer to the member with the suitable prescale
		*/
		struct timer5_value * prescale_determination (struct timer5_value *timer5_value_lookup_table_ptr_for_prescale , unsigned long int time_bet_steps_for_prescale);

		/*
			Function name : ctc_value_determination
		  	return : unsigned int:- used to return the value needed for the OCR1A register in ctc mode
		  	parameters : struct timer5_value *timer5_value_lookup_table_ptr_for_prescale :- pointer to the suitable element in the timer5_value_lookup_table array
		  				 unsigned long int time_bet_steps_for_ctc :- used to hold the time between each step in microseconds
		  	Method of operation : it calculates the value neede to be in the OCR1A register for the isr to work in the right perioo of time
		*/
		unsigned int ctc_value_determination (struct timer5_value *timer5_value_lookup_table_ptr_for_ctc , unsigned long int time_bet_steps_for_ctc);

		/*
			Function name : timer5_setup
		  	return : void
		  	parameters : struct timer5_value *timer5_value_lookup_table_ptr_for_prescale :- pointer to the suitable element in the timer5_value_lookup_table array
		  				 unsigned long int time_bet_steps :- used to hold the time between each step in microseconds
		  	Method of operation : it sets the registers for timer 1 to the right prescale value and ctc value and enables the timer one ctc interrupt
		*/
		void timer5_setup (struct timer5_value *timer5_value_lookup_table_ptr , unsigned long int time_bet_steps);
};

#endif
#endif
#endif 