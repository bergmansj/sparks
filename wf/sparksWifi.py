#!/usr/bin/env python
#
# Author:		Patrick Hanckmann
# Company:		TNO Technical Sciences
# Description:	Scans for Wifi devices. It writes all findings (mac address,
#				device class) into a SQLite database.
# Depends on:	Python 2.6
#				aircrack-ng suite (includes multiple programs)
#
# Version:		3.1
# Date:			27-07-2011

from __future__ import with_statement

import os
import sys
import time
import getopt
import ConfigParser
import logging
import subprocess
import commands
from sqlite3 import dbapi2 as sqlite   # Linux

# TO DO:
#  - more intelligent killing mechanism in __init__

# Standard procedure for aircrack-ng
# 1 # sudo airmon-ng check
# 2 # >>> now check if nothing (except wpa_supplicant???) is using the interface
# 3 # sudo kill <pid from process>
# 4 # sudo airmon-ng start wlan0
# 5 # sudo airbase-ng mon0


class WifiScanner():
	"""Store the discovered devices in the SQLite database.

	Asks for a Wifi scan. If a Wifi devices has been discovered then it writes
	all findings (mac address, major_class) into an SQLite database.
	"""

	sqlfile = ""
	device = ""
	scan_finished = False
	scan_delay = 1
	timeframe = 5	  # Time between detections to link the detection

	def clearWifiCard(self):
		"""Makes sure that no processes try to acces any wifi cards. """
		i = -1
		#while i != 0:
		#	self.log.info("WIFI_: Clearing the %s card; killed a process" % (self.device,))
		#	output = commands.getoutput("airmon-ng check kill")
		#	output = output.splitlines(False)
		#	#print output
		#	i = len(output)


	def startAirmonService(self):
		"""Starts the airmon-ng injection service. """
		airmon = "airmon-ng start " + self.device
		output = commands.getoutput(airmon)
		self.log.info("WIFI_: airmon-ng injection service started")
		self.log.info("WIFI_: %s" % (output,))

	def __init__(self, device_name, sqlfile, logfile):
		"""Prepares and starts the needed services, and creates the database.

		First all processes connected with the wireless-card should be
		terminated. Then the airmon-ng server has to be started. Finally SQLite
		database is prepared for data.
		"""
		# Logging
		self.log = logging.getLogger("WifiScanner")
		self.hdlr = logging.FileHandler(logfile)
		formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
		self.hdlr.setFormatter(formatter)
		self.log.addHandler(self.hdlr)
		self.log.setLevel(logging.DEBUG)
		self.log.info("WIFI_: WifiScanner has started")
		self.log.info("WIFI_: Time: %s", (round(time.time())))
		
		# Database local
		try:
			dfile = open(sqlfile)
		except IOError:
			self.log.info("WIFI_: No SQLite file found, new SQLite file created")
			pass
		self.t1_connection = sqlite.connect(sqlfile)
		self.t1_cursor = self.t1_connection.cursor()
		# Database local AIS table
		try:
			self.t1_cursor.execute('CREATE TABLE Wifi (id INTEGER PRIMARY KEY, \
								timestamp DATETIME, \
								address VARCHAR(17), \
								signal FLOAT, \
								name VARCHAR(80), \
								active INTEGER)')
			self.t1_connection.commit()
		except sqlite.OperationalError:
			self.log.info("WIFI_: Table 'Wifi' already exists")

		# Prepare wifi card
		self.device = device_name
		self.clearWifiCard()
		self.startAirmonService()

	def __del__(self):
		"""Destructor"""
		self.t1_cursor.close()
		self.t1_connection.close()
		self.log.info("WIFI_: Time: %s", (round(time.time())))
		self.log.info("WIFI_: WifiScanner is terminating")

	def scanDelay(self, seconds) :
		"""Sets the number of seconds between each scan."""
		self.scan_delay = seconds

	def scan(self):
		"""Scans for Wifi devices and stores the outcomes."""
		virtual_device = "mon0"
		process = subprocess.Popen(["airbase-ng", "-Av", virtual_device], \
			stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		while True:
			outp = process.stdout.readline()
			while outp != "":
				if outp.find("request") != -1:
					# Collect GPS info and initialise
					id		  = -1
					starttime   = round(time.time())
					active	  = 1
					word_list = outp.rsplit()
						#0 18:04:35			 -> time
						#1 Got				  -> text
						#2 broadcast			-> probe_type
						#3 probe				-> text
						#4 request			  -> text
						#5 from				 -> text
						#6 00:1B:77:37:FF:BF	-> address
						#7 -					-> text
						#8 "xxx"				-> name
					address = word_list[6]
					if (len(word_list) == 0):
						name = word_list[8]
					else:
						name = ""
					
					# Add measurement to db
					self.t1_cursor.execute('INSERT INTO Wifi \
										VALUES (null, ?, ?, ?, ?, ?)', \
										(timestamp, address, signal, name, active))
					self.log.info("WIFI_: address: %s",(address))
							
					self.t1_connection.commit()
					
				outp = process.stdout.readline()
			
			time.sleep(self.scan_delay)

def usage():
	print "usage: wifiscanner.py [-h] [-c condif_file] [-i interface_name]"
	print "                      [-d database_name] [-l log_file] [-s scan_delay]"
	print ""
	print "Arguments:"
	print "	 {-h --help}"
	print "	 {-c --configfile}    <path and filename of the configfile>"
	print "	 {-i --interface}     <interface name of the wireless card>"
	print "	 {-d --database}      <path and filename of the database>"
	print "	 {-l --log}           <path and filename of the logfile>"
	print "	 {-s --scan_delay}    <delay time (seconds) between scans>"
	print "All arguments are optional, except the wireless card interface name!"
	print "NOTE: Script needs root permissions to be able to run!"

def main(argv):
	# Script needs root permissions to be able to run
	if not os.geteuid()==0:
		usage()
		sys.exit("\nerror: script needs root permissions to be able to run.")
	try:
		opts, args = getopt.getopt(argv, "hc:i:d:l:m:s:", ["help", \
														 "configfile=", \
														 "interface=", \
														 "database=", \
														 "log=", \
														 "scan_delay="])
	except getopt.GetoptError:
		usage()
		sys.exit(2)

	interface = ""
	config_file = "wifi.conf"
	database = ""
	log = ""
	scan_delay = -1  # in seconds

	for opt, arg in opts:
		if opt in ("-h", "--help"):
			usage()
			sys.exit()
		elif opt in ("-i", "--interface"):
			interface = arg
		elif opt in ("-d", "--database"):
			database = arg
		elif opt in ("-l", "--log"):
			log = arg
		elif opt in ("-s", "--scan_delay"):
			scan_delay = arg
		elif opt in ("-c", "--configfile"):
			config_file = arg

	if interface == "":
		usage()
		sys.exit("\nerror: you need to specify the device interface.")

	setConf = True
	config = ConfigParser.ConfigParser()
	if len(config.read(config_file)) != 0:
		if config.has_section('Wifi') == True:
			if database == "":
				database = config.get('Wifi', 'Database')
			if log == "":
				log = config.get('Wifi', 'Log')
			if scan_delay == -1:
				scan_delay = config.getint('Wifi', 'Scan_delay')
			setConf = False
		
	if setConf == True:
		if database == "":
			database = "wifi.sqlite"
		if log == "":
			log = "wifi.log"
		if scan_delay == -1:
			scan_delay = 3
		config.add_section('Wifi')
		config.set('Wifi', 'Database', database)
		config.set('Wifi', 'Log', log)
		config.set('Wifi', 'Scan_delay', scan_delay)
		with open(config_file, 'wb') as configfile:
			config.write(configfile)

	x = WifiScanner(interface, database, log)
	x.scanDelay(float(scan_delay))
	x.scan()

if __name__=='__main__' :
	main(sys.argv[1:])

