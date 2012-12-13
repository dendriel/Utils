#!/usr/bin/python
# -*- coding: utf-8 -*-
#################################################################
# Author: Vitor Rozsa                                           #
# Contact: vitor.rozsa@hotmail.com                              #
# Version: 1.0                                                  #
#                                                               #
# Description:                                                  #
# TO BE UPDATED! This scripts was made without time and coffee..#
# auto connect into remote host with ssh.                       #
# will use with an alias, like "$s 223", then, the# script      #
# will look for the in the passwords file for the pass for      #
# the given IP. If could find a pass, will call the expect      #
# script giving the IP and PASS to connect.                     #
#################################################################

import sys
import os

PASSWD_FILE = "/home/name/dir/password_path"
IP_MASK = "192.168.0."
ROOT_TK = "root: "
TOKEN = ":"
EXPECT_SCRIPT = "/home/name/dir_to_autoss_expect_script/auto_ssh.exp"

if len(sys.argv) < 2:
	print "missing the target"
	sys.exit()

ip_addr = sys.argv[1]
header = IP_MASK+ip_addr+TOKEN
password = ''

f = open(PASSWD_FILE, "r")

line = f.readline()

while line != '':

	if line.find(header) > -1:
		while line != '':
			line = f.readline()
			if line.find(ROOT_TK) > -1:
				password = line[line.find(ROOT_TK)+len(ROOT_TK):len(line)-1] # -1 for the new line char
				break

		if password != '':
			break

	line = f.readline()

if password != '':
	command = "expect " + EXPECT_SCRIPT + " " + IP_MASK + ip_addr + " " +  password
	os.system(command)
else:
	print "> IP Address not found!"

