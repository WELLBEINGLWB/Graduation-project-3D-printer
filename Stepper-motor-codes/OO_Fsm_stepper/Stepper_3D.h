#ifndef _STEPPER_3D_
#define _STEPPER_3D_

#include "Arduino.h"


//defining the direction of the change in states
#define NEXT 			1
#define PREVIOUS 		0


/*struct stepper_state_struct is a struct used to hold the info concerning the states, each state resemnles one step,
it holds the output of the state and a pointer to the next state to use to step forward and
a pointer to the previous step in case of backwards */

typedef struct stepper_state_struct
{
	unsigned char out;						//used to hold the output values to all pins, this value needs to be inturpeted later
 	struct stepper_state_struct	*nxt;		//used to hold a pointer to the next state which resmbles the next step on the stepper motor, used for forward motion
	struct stepper_state_struct	*prev;		//pointer used to hold the address to the previos state, used for backwards motion
} stepper_state_struct;
/* struct timer1_value is used in the lookup table used to determine the value of the prescale and the determination of the value of OCR1A register
 to be able to operate with the timer1 asagile as possible */
typedef struct timer1_value
{
	unsigned int prescale;				//to hold the value of the prescale
	float time_per_tick_us;				//to hold the value of time needed to make one tick in the timer in micro seconds
	unsigned long int max_period_us;	//to haold the max value the timer can hold in micro seconds
}timer1_value;

class stepper_3d
{
	public:		//what the user has access to
		stepper_3d ();	//constructor

		//mapping the pins with wire colors
		unsigned char black;
		unsigned char blue =7 ;
		unsigned char red;
		unsigned char green =6;
		unsigned char brown =5;
		unsigned char orange;
		unsigned char yellow;
		unsigned char white =4;
		//mapping wire colors with firing order
		unsigned char first  = blue;
		unsigned char second = brown;
		unsigned char third  = green;
		unsigned char forth  = white;
		//mapping directions motion with state transition direction
		unsigned char clockwise     = PREVIOUS;
		unsigned char anticlockwise = NEXT;
		unsigned char forward       = clockwise;
		unsigned char backward      = anticlockwise;

		unsigned char permission = 1;		//used to prevent stepper_move function fromoverwriting itself, to execute stepper_move set it to 1, to stop the overwrting set it to 0

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
		  	Method of operation : it is used to move the stepper motor in a very big nummber of steps that it seems it moves for infinityyyyy, but it doesn't, 
		  						  best used for moving the stepper motor till a physical event happens to stop it
		*/
		void stepper_flow (unsigned char direction_flow);
		/*
			Function name : inside_ISR
		  	return : void
		  	parameters :void
		  	Functionality : this function is to be called inside the timer ISR function, it the function resposible for doing the motion everytime the ISR runs
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

	private:	//stuff under the hood, the user shouldn't bither himself with
		/*stepper_states is an array that holds the constant values of all the states of the stepper motor */
		struct stepper_state_struct stepper_states[8] = 
		{
		//states {out  , next state         , previos state      }
		/*0*/	 {0x01 , &stepper_states[1] , &stepper_states[7] } ,
		/*1*/	 {0x03 , &stepper_states[2] , &stepper_states[0] } ,
		/*2*/	 {0x02 , &stepper_states[3] , &stepper_states[1] } ,
		/*3*/	 {0x06 , &stepper_states[4] , &stepper_states[2] } ,
		/*4*/	 {0x04 , &stepper_states[5] , &stepper_states[3] } ,
		/*5*/	 {0x0c , &stepper_states[6] , &stepper_states[4] } ,
		/*6*/	 {0x08 , &stepper_states[7] , &stepper_states[5] } ,
		/*7*/	 {0x09 , &stepper_states[0] , &stepper_states[6] } ,
		};

		/* timer1_value_lookup_table is an array that holds the values of the prescales and the time per tick and the max time value for each prescale */
		struct timer1_value timer1_value_lookup_table[5] = 
		{
		 //{prescale , time_per_tick_us , max_period_us}
			{1 	  	, 0.0625  			, 4096   	   },
			{8 	  	, 0.5 				, 32767  	   },
			{64   	, 4 	 			, 262140 	   },
			{256  	, 16	 			, 1048560	   },
			{1024 	, 64 				, 4194240	   }
		};

		struct stepper_state_struct current_state;		//the variable that will hold the current state information, initialized with state zero info
		unsigned long int 	stepper_steps;			//this variable holds the number of steps remained to be moved, needed by the isr
		unsigned char 		direction;				//this variable holds the direction of movement, needed by the isr

		/*Function name : stepper_output
		  return : void
		  parameters : struct stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
		  Functionality : to  output the ouy member in the current_state struct to the pins, use after next_step or previos_step functions, runs after next_step or previos_step
		 */
		void stepper_output (struct stepper_state_struct *current_state);
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
		  	return : struct timer1_value * :- pointer to the element of the timer1_value_lookup_table with the right prescale
		  	parameters : struct timer1_value *timer1_value_lookup_table_ptr_for_prescale :- pointer to the timer1_value_lookup_table array
		  				 unsigned long int time_bet_steps_for_prescale :- used to hold the time between each step in microseconds
		  	Method of operation : it searches the timer1_value_lookup_table array for the suitable prescale and returns a pointer to the member with the suitable prescale
		*/
		struct timer1_value * prescale_determination (struct timer1_value *timer1_value_lookup_table_ptr_for_prescale , unsigned long int time_bet_steps_for_prescale);

		/*
			Function name : ctc_value_determination
		  	return : unsigned int:- used to return the value needed for the OCR1A register in ctc mode
		  	parameters : struct timer1_value *timer1_value_lookup_table_ptr_for_prescale :- pointer to the suitable element in the timer1_value_lookup_table array
		  				 unsigned long int time_bet_steps_for_ctc :- used to hold the time between each step in microseconds
		  	Method of operation : it calculates the value neede to be in the OCR1A register for the isr to work in the right perioo of time
		*/
		unsigned int ctc_value_determination (struct timer1_value *timer1_value_lookup_table_ptr_for_ctc , unsigned long int time_bet_steps_for_ctc);

		/*
			Function name : timer1_setup
		  	return : void
		  	parameters : struct timer1_value *timer1_value_lookup_table_ptr_for_prescale :- pointer to the suitable element in the timer1_value_lookup_table array
		  				 unsigned long int time_bet_steps :- used to hold the time between each step in microseconds
		  	Method of operation : it sets the registers for timer 1 to the right prescale value and ctc value and enables the timer one ctc interrupt
		*/
		void timer1_setup (struct timer1_value *timer1_value_lookup_table_ptr , unsigned long int time_bet_steps);
};



#endif 