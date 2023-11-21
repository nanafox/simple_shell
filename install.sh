#!/bin/bash

 ################################################################
#			MSH (Minimal Shell) Installation Script				 #
#		Author: Maxwell Nana Forson <nanaforsonjnr@gmail.com> 	 #
#																 #
#  The script takes care of building the executable file, then   #
#  copies it to the /usr/bin/ directory making the msh shell     #
#  accesible from anywhere on entire system						 #
 ################################################################

if [ $UID -ne 0 ]; then
	echo "You need to be root!"
	exit 1
fi

# Build the executable file
make && cp msh /usr/bin/msh

if [ $? -eq 0 ]; then
	# clean up
	make clean
	echo "Success: msh is now installed! :-)"
	echo "Execute it by running: msh"
else
	# pobably an error occured
	echo "An error occured while installing!"
	echo "Please try again."
fi
