#!/bin/bash
IFS=$'\t\n'

function runmyfile {
	oldf1=$(exiv2 -PInv $gfile | grep 'FixtureId' | sed 's/^FixtureId[       ]*//')
	fixold=$oldf1
	echo -e "The word or phrase currently in this file reads:\n\t \033[4;37m${oldf1}\033[0m."
	echo -ne "\033[0;36mDo you want to change this Fixture ID to something else? \033[0m (y/n)\n"
	read yesorno
	if [ $yesorno = "n"  ]
	then
		echo -ne "No changes made to file \033[1;34m${gfile}.\033[0m\nBye.\n"
	else
		echo -ne "\033[1;31mAre you sure?\033[0m (y/n) \n"
		read confirmm
		if [ $confirmm = "n" ]
		then
		echo -ne "No changes made to file \033[1;34m${gfile}.\033[0m\nBye.\n"
		else
			exiv2 -M"del Iptc.Application2.FixtureId" modify $gfile
			echo -ne "FIXTURE IDENTIFICATION HAS BEEN REMOVED.\n"
			echo -ne "Please enter the word or phrase to replace '$fixold .'\n"
			read newstringhx 
			capnew="`echo ${newstringhx} | awk '{print toupper($0)}'`"
			exiv2 -M"add Iptc.Application2.FixtureId String $newstringhx" modify $gfile
			sleep 0.5
			filecapper="`echo ${gfile} | awk '{print toupper($0)}'`"
			echo -ne "FIXTURE IDENTIFICATION \"$capnew\" WRITTEN TO FILE ${filecapper}.\n"
			echo -ne "(Don't worry -- it was written the way you wrote it.)\n"
		fi
	fi
}


echo -ne "Fixture Identification Fixer for Exiv2 - FIFE\n"
echo -ne "Please enter the name of the file you want to modify.\n"
read 'gfile'
if [ -f $gfile ]
then
	runmyfile
else
	echo -e "Sorry, that file is not in this folder.\n"
	echo -e "Let me take another look.\n"
	maybe=(`find . -name "*$gfile*" | cut -d / -f2`)
	sleep 1.5
	echo -ne "Do you mean: $maybe ? (y/n) \n"
	#Using a variable one can be sure has no built-in value in BASH:
	read cheeseburger
	if [ ["$cheeseburger" == "n"] ]
	then
		echo -ne "\033[0;34mWell, I tried.\033[0m\n"
	else
		gfile=$maybe
		runmyfile
	fi
fi