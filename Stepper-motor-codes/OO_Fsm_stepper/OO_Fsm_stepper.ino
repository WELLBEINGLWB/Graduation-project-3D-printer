/*
this file is just to test the library
if it works it was done by Ahmed El Fakharany to test the finite state machine concept with stepper motors for the 3d printer graduation project, 
if it doesn't workm I don't know who the hell did it but I'm sure it is not Ahmed El Fakharany :D */

#include "Stepper_3D.h"
#include "Global_variables.h"

#define FORWARD	extruder.forward 
#define BACKWARD extruder.backward


stepper_3d extruder;
/*********** main and isr functions ***********/

void setup() 
{
	
}

/* the ISR function is the one that does the moving of the stepper motor, it outputs the step at the time required */
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
	extruder.inside_ISR();
}

void loop() 
{
	extruder.stepper_flow (BACKWARD);
	if (digitalRead(7) == HIGH)
	{
		extruder.stepper_stop();
	}
	if (digitalRead(6) == HIGH)
	{
		extruder.stepper_resume ();
	}
	delay(5000);
	extruder.permission = 1;
	extruder.change_linear_direction();
	//extruder.stepper_flow (BACKWARD);
}



