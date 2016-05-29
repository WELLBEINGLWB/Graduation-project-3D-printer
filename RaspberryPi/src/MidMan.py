from time import sleep
from UUGear import *

UUGearDevice.setShowLogs(0)

class MidMan :
	X = UUGearDevice('UUGear-Arduino-1239-9170')	#X axis stepper and extruder heater
	Y = UUGearDevice('UUGear-Arduino-3167-3008')	#Y axis stepper and heatbed
	Z = UUGearDevice('UUGear-Arduino-5658-7598')	#Z axis stepper and extruder stepper

	def __init__(self):
		self.valid = 1

		self.Xlist = [0,0]
		self.Ylist = [0,0]
		self.Zlist = [0,0]
		self.Elist = [0,0]
		self.heatbed_t = 0
		self.old_heatbed_t = 0
		self.ext_heat = 0
		self.old_ext_heat = 0
		self.exec_time = 0
		#check validity
		if MidMan.X.isValid() :
			self.valid = self.valid * self.valid
		else :
			print "X axis isn't correctly initialized. Hint : check UUgear ID"
			self.valid = 0

		if MidMan.Y.isValid() :
			self.valid = self.valid * self.valid
		else :
			print "Y axis isn't correctly initialized. Hint : check UUgear ID"
			self.valid = 0

		if MidMan.Z.isValid() :
			self.valid = self.valid * self.valid
		else :
			print "Z axis isn't correctly initialized. Hint : check UUgear ID"
			self.valid = 0

	def is_valid (self) :
		return self.valid

	def fill_Xdata(self, lst) :
		self.Xlist = lst

	def fill_Ydata(self, lst) :
		self.Ylist = lst

	def fill_Zdata(self, lst) :
		self.Zlist = lst

	def fill_Edata(self, lst) :
		self.Elist = lst

	def fill_heatbed_data(self, var) :
		self.heatbed_t = var

	def fill_ext_heat_data(self, var) :
		self.ext_heat = var

	def send_Xdata(self) :
		if self.Xlist[0] != 0 :
			#send time between steps
			res = MidMan.X.stepper_time_bet_steps(self.Xlist[1])
			#if no valid response try again
			if res == -1 or res != 47 :
				res = MidMan.X.stepper_time_bet_steps(self.Xlist[1])
				#if again no valid response, flag an error
				if res == -1 or res != 47 :
					print "Error sending stepper_time_bet_steps. Hint: check if stepper module is properly defined in X axis"
					return False
			#if time between steps is sent properly then proceed to send the number of steps
			res1 = MidMan.X.stepper_move(self.Xlist[0])
			#if no valid response try again
			if res1 == -1 or res1 != 47 :
				res1 = MidMan.X.stepper_move(self.Xlist[0])
				#if again no valid response, flag an error
				if res1 == -1 or res1 != 47 :
					print "Error sending stepper_move. Hint: check if stepper module is properly defined in X axis"
					return False
		return True

	def send_Ydata(self) :
		if self.Ylist[0] != 0 :
			#send time between steps
			res = MidMan.Y.stepper_time_bet_steps(self.Ylist[1])
			#if no valid response try again
			if res == -1 or res != 47 :
				res = MidMan.Y.stepper_time_bet_steps(self.Ylist[1])
				#if again no valid response, flag an error
				if res == -1 or res != 47 :
					print "Error sending stepper_time_bet_steps. Hint: check if stepper module is properly defined in Y axis"
					return False
			#if time between steps is sent properly then proceed to send the number of steps
			res1 = MidMan.Y.stepper_move(self.Ylist[0])
			#if no valid response try again
			if res1 == -1 or res1 != 47 :
				res1 = MidMan.Y.stepper_move(self.Ylist[0])
				#if again no valid response, flag an error
				if res1 == -1 or res1 != 47 :
					print "Error sending stepper_move. Hint: check if stepper module is properly defined in Y axis"
					return False
		return True

	def send_Zdata(self) :
		if self.Zlist[0] != 0 :
			#send time between steps
			res = MidMan.Z.stepper_time_bet_steps(self.Zlist[1])
			#if no valid response try again
			if res == -1 or res != 47 :
				res = MidMan.Z.stepper_time_bet_steps(self.Zlist[1])
				#if again no valid response, flag an error
				if res == -1 or res != 47 :
					print "Error sending stepper_time_bet_steps. Hint: check if stepper module is properly defined in Z axis"
					return False
			#if time between steps is sent properly then proceed to send the number of steps
			res1 = MidMan.Z.stepper_move(self.Zlist[0])
			#if no valid response try again
			if res1 == -1 or res1 != 47 :
				res1 = MidMan.Z.stepper_move(self.Zlist[0])
				#if again no valid response, flag an error
				if res1 == -1 or res1 != 47 :
					print "Error sending stepper_move. Hint: check if stepper module is properly defined in Z axis"
					return False
		return True

	def send_Edata(self) :
		if self.Elist[0] != 0 :
			#send time between steps
			res = MidMan.Z.ext_stepper_time_bet_steps(self.Elist[1])
			#if no valid response try again
			if res == -1 or res != 47 :
				res = MidMan.Z.ext_stepper_time_bet_steps(self.Elist[1])
				#if again no valid response, flag an error
				if res == -1 or res != 47 :
					print "Error sending ext_stepper_time_bet_steps. Hint: check if stepper module is properly defined in E axis"
					return False
			#if time between steps is sent properly then proceed to send the number of steps
			res1 = MidMan.Z.ext_stepper_move(self.Elist[0])
			#if no valid response try again
			if res1 == -1 or res1 != 47 :
				res1 = MidMan.Z.ext_stepper_move(self.Elist[0])
				#if again no valid response, flag an error
				if res1 == -1 or res1 != 47 :
					print "Error sending ext_stepper_move. Hint: check if stepper module is properly defined in Z axis"
					return False
		return True

	def send_heatbed_t(self) :
		if (self.heatbed_t - self.old_heatbed_t) != 0 :
			#send heatbed temperature
			res = MidMan.Y.heatbed_set_temp(self.heatbed_t)
			#if no valid response try again
			if res == -1 or res != 47 :
				res = MidMan.Y.heatbed_set_temp(self.heatbed_t)
				#if again no valid response, flag an error
				if res == -1 or res != 47 :
					print "Error sending heatbed_set_temp. Hint: check if heatbed module is properly defined in Y axis"
					return False
		self.old_heatbed_t = self.heatbed_t
		return True

	def send_ext_t(self) :
		if (self.ext_heat - self.old_ext_heat) != 0 :
			#send heatbed temperature
			res = MidMan.X.ext_heat_set_temp(self.ext_heat)
			#if no valid response try again
			if res == -1 or res != 47 :
				res = MidMan.X.ext_heat_set_temp(self.ext_heat)
				#if again no valid response, flag an error
				if res == -1 or res != 47 :
					print "Error sending ext_heat_set_temp. Hint: check if extruder heat module is properly defined in X axis"
					return False
		self.old_ext_heat = self.ext_heat
		return True

	def machine_control(self) :
		if self.send_Xdata() and self.send_Ydata() and self.send_Zdata() and self.send_Edata() and self.send_heatbed_t() and self.send_ext_t() :
			return True
		else :
			return False

	def Detach_machine(self) :
		MidMan.X.detach()
		MidMan.Y.detach()
		MidMan.Z.detach()
		MidMan.X.stopDaemon()
		MidMan.Y.stopDaemon()
		MidMan.Z.stopDaemon()
		
