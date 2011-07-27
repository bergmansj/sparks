#!/usr/bin/env python
#
# Author:		Patrick Hanckmann
# Company:		TNO Technical Sciences
# Description:	Scans for bluetooth devices. When found it tries to identify the
#				device class. It writes all findings (mac address, device class)
#				into a SQLite database.
# Depends on:	Python 2.6
#				python-dateutil
#				pysqlite
#				PyBluez-0.16 (Linux version required!) / python-pybluez
#
# Version:		3.1
# Date:			27-07-2011

from __future__ import with_statement

import sys
import getopt
import ConfigParser
import time
import bluetooth
import select
import logging
from sqlite3 import dbapi2 as sqlite   # Linux

# TO DO:
#  - signal strength

class MyDiscoverer(bluetooth.DeviceDiscoverer):
	"""Scans for Bluetoth devices and classifies them.

	Scans for bluetooth devices. When found it tries to identify the device
	class.
	"""

	def pre_inquiry(self):
		"""Set the start variables."""
		self.done = False
		self.found = False

	def device_discovered(self, address, device_class, name):
		"""Classify the detected devices.

		Maps the device_class variable to a human-readable device class.
		"""
		self.found = True
		self.address = address
		self.name = name

		major_classes = ( "Miscellaneous",
						  "Computer",
						  "Phone",
						  "LAN/Network Access point",
						  "Audio/Video",
						  "Peripheral",
						  "Imaging" )
		major_class = (device_class >> 8) & 0xf
		if major_class < 7:
			self.major_class = major_classes[major_class]
		else:
			self.major_class = "Uncategorized"

	def inquiry_complete(self):
		"""Set the finish variables."""
		self.done = True

class BTScanner(bluetooth.DeviceDiscoverer):
	"""Store the discovered devices in the SQLite database.

	Asks for a bluetooth scan. If a Bluetooth devices has been discovered then
	it writes all findings (mac address, name, major_class) into an SQLite
	database.
	"""

	sqlfile = ""
	scan_finished = False
	scan_delay = 1	# in seconds
	timeframe = 5	# Time between detections to link the detection

	def __init__(self, sqlfile, logfile):
		"""Sets the standard filenames and creates the database."""
		# Logging
		self.log = logging.getLogger("Bluetooth scanner")
		self.hdlr = logging.FileHandler(logfile)
		formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
		self.hdlr.setFormatter(formatter)
		self.log.addHandler(self.hdlr)
		self.log.setLevel(logging.DEBUG)
		self.log.info("BT___: Bluetooth scanner has started")
		self.log.info("BT___: Time: %s", (round(time.time())))

		# Database local
		try:
			dfile = open(sqlfile)
		except IOError:
			self.log.info("BT___: The local database does not exist and will be created.")
			pass
		self.t1_connection = sqlite.connect(sqlfile)
		self.t1_cursor = self.t1_connection.cursor()
		# Database local AIS table
		try:
			self.t1_cursor.execute('CREATE TABLE Bluetooth (id INTEGER PRIMARY KEY, \
								timestamp DATETIME, \
								address VARCHAR(17), \
								signal FLOAT, \
								name VARCHAR(80), \
								major_class VARCHAR(30), \
                                active INTEGER)')
			self.t1_connection.commit()
		except sqlite.OperationalError:
			self.log.info("BT___: Table 'Bluetooth' already exists")

	def __del__(self):
		"""Destructor"""
		self.t1_cursor.close()
		self.t1_connection.close()
		self.log.info("BT___: Bluetooth scanner is terminating")
		self.log.info("BT___: Time: %s", (round(time.time())))

	def scanDelay(self, seconds) :
		"""Sets the number of seconds between each scan."""
		if (seconds >= 0):
			self.scan_delay = seconds


	def scan(self) :
		"""Scans for Bluetooth devices and stores the outcomes.

		Asks the MyDiscover class to scan for Bluetooth devices and to map the
		devices classes to a human-readable format. Finally it stores the
		results in the SQLite database as defined in __init__(self, sqlfile).
		"""
		while True:
			d = MyDiscoverer()
			d.find_devices(lookup_names = True)
			readfiles = [ d, ]
			while True:
				# Collect GPS info and initialise
				id = -1
				timestamp = (round(time.time()))
				active = 0 # = false

				rfds = select.select( readfiles, [], [] )[0]
				if d in rfds:
					d.process_event()
				if d.found:
					# Measurement performed
					address = d.address
					signal = 0
					name = d.name
					major_class = d.major_class
					
					# Add measurement to db
					self.t1_cursor.execute('INSERT INTO Bluetooth \
											VALUES (null, ?, ?, ?, ?, ?, ?)', \
											(timestamp, address, signal, \
											name, major_class, active))
					self.log.info("BT___: address: %s",(address))
					self.t1_connection.commit()
				if d.done: break
			
			time.sleep(self.scan_delay)

def usage():
	print "usage: btscanner.py [-h] [-c condif_file] [-d database_name] [-l log_file]"
	print "                    [-m moving_sensor] [-s scan_delay]"
	print ""
	print "Arguments:"
	print "	 {-h --help}"
	print "	 {-c --configfile} <path and filename of the configfile>"
	print "	 {-d --database}   <path and filename of the database>"
	print "	 {-l --log}        <path and filename of the logfile>"
	print "	 {-s --scan_delay} <delay time (seconds) between scans>"
	print "All arguments are optional."

def main(argv):
	try:
		opts, args = getopt.getopt(argv, "hc:d:l:s:", ["help", \
													   "configfile=", \
													   "database=", \
													   "log=", \
													   "scan_delay="])
	except getopt.GetoptError:
		usage()
		sys.exit(2)

	config_file = "./bt.conf"
	database = ""
	log = ""
	scan_delay = -1

	for opt, arg in opts:
		if opt in ("-h", "--help"):
			usage()
			sys.exit()
		elif opt in ("-d", "--database"):
			database = arg
		elif opt in ("-l", "--log"):
			log = arg
		elif opt in ("-s", "--scan_delay"):
			scan_delay = arg
		elif opt in ("-c", "--configfile"):
			config_file = arg

	setConf = True
	config = ConfigParser.ConfigParser()
	if len(config.read(config_file)) != 0:
		if config.has_section('Bluetooth') == True:
			if database == "":
				database = config.get('Bluetooth', 'Database')
			if log == "":
				log = config.get('Bluetooth', 'Log')
			if scan_delay == -1:
				scan_delay = config.getint('Bluetooth', 'Scan_delay')
			setConf = False
			
		
	if setConf == True:
		if database == "":
			database = "./bt.sqlite"
		if log == "":
			log = "./bt.log"
		if scan_delay == -1:
			scan_delay = 3
		config.add_section('Bluetooth')
		config.set('Bluetooth', 'Database', database)
		config.set('Bluetooth', 'Log', log)
		config.set('Bluetooth', 'Scan_delay', scan_delay)
		with open(config_file, 'wb') as configfile:
			config.write(configfile)

	x = BTScanner(database, log)
	x.scanDelay(float(scan_delay))
	x.scan()

if __name__=='__main__' :
	main(sys.argv[1:])

