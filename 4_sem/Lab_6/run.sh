#!/bin/bash

RED="\e[1;31m"
GREEN="\e[1;32m"
YELLOW="\e[1;33m"
BLUE="\e[1;34m"
NC="\e[0m"

echo
echo -e " ${BLUE}######################################################################################################${NC} "
echo -e " ${BLUE}#                                                                                                    #${NC} "
echo -e " ${BLUE}#${NC}                              ${GREEN}Построение и Анализ Алгоритмов${NC}                                        ${BLUE}#${NC}"
echo -e " ${BLUE}#${NC}                                                                                                    ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC}                                Лабораторная работа № 6                                             ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC}                                 Алгоритм Ахо-Корасик                                               ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC}                                                                                                    ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC}                                                                                                    ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC} ${YELLOW}   ____          _          _   _               _              _                                   ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC} ${YELLOW}  / ___|___   __| | ___  __| |  | |__  _   _   | | _______   _(_)_ __   _____   ___ __ _____   __  ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC} ${YELLOW} | |   / _ \ / _' |/ _ \/ _' |  | '_ \| | | |  | |/ / _ \ \ / / | '_ \ / _ \ \ / / '_ ' _ \ \ / /  ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC} ${YELLOW} | |__| (_) | (_| |  __/ (_| |  | |_) | |_| |  |   < (_) \ V /| | | | |  __/\ V /| | | | | \ V /   ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC} ${YELLOW}  \____\___/ \__,_|\___|\__,_|  |_.__/ \__, |  |_|\_\___/ \_/ |_|_| |_|\___| \_/ |_| |_| |_|\_/    ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC} ${YELLOW}                                      |___/                                                        ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC}                                                                                                    ${BLUE}#${NC} "
echo -e " ${BLUE}######################################################################################################${NC} "
echo




w1="[${RED}-${NC}]"
w2="[${GREEN}+${NC}]"
w3="[${YELLOW}?${NC}]"

if [ -z "$1" ];
then
	set -- "build"
else
	if [ "$1" = ".." ] || [ "$1" = "." ];
	then
		echo -e "$w1 Invalid name folder was detected. You must pass a correct folder name as a parameter"
		echo -e "    Example: $0 build"
		exit 1
	else
		echo -e "$w2 Entered folder name: $1"
	fi
fi


echo -e "$w2 Checking directory ./$1"

if ! [ -d "./$1" ];
then
	echo -e "$w2 Current folder \"$1\" does not exist"
else
	echo -e "$w1 Directory exists: ./$1"
	echo -n -e "$w3 Do you want to delete project in folder \"$1\"? [Y/N] "
	read var
	if  [ "$var" = "Y" ] || [ "$var" = "y" ] || [ "$var" = "" ]
	then
		echo -e "$w2 Deleting folder $1"
		rm -rf "./$1"
		echo -e "$w2 Deleted successfully!"
	else
		name=`head /dev/urandom | tr -dc A-Za-z0-9 | head -c 8 ; echo ''`
		echo -e "$w2 Saving $1 as $1_old_$name"
		mv "./$1" "./$1_old_$name"
		echo -e "$w2 Saved successfully!"
	fi
fi

echo -e "$w2 Creating new directory ./$1"
mkdir "./$1"
echo -e "$w2 Created"


cd "./$1"
echo -e "$w2 Moved to the directory $1"


path=`pwd`
echo -e "$w2 Path: ${path}"

echo -e "$w2 Running CMake"
echo
cmake ..
if [[ $? != 0 ]];
then
	echo -e "$w1 ${RED}Error running CMake. Check version of CMake and GTest (see README.md)"
	exit 1
else
	echo
fi
echo -e "$w2 CMake ends successfully!"

echo -e "$w2 Building project"
echo
cmake --build .
if [[ $? != 0 ]];
then
	echo -e "$w1 Error building project."
	exit 1
else
	echo
fi
echo -e "$w2 Built project successfully!"

echo -e "$w2 Running GTests"
./test_google > log.txt

if [[ $? != 0 ]];
then
	echo -e "$w1 Error GTests"
	echo -n -e "$w3 Do you want to delete project \"$1\"? [Y/N] "
	read var
	if  [ "$var" = "Y" ] || [ "$var" = "y" ] || [ "$var" = "" ]
	then
		echo -e "$w2 Deleting folder $1"
		rm -rf "./$1"
		echo -e "$w2 Deleted successfully!"
		exit 1
	else
		echo -e "$w1 ${RED}Warning! GTests completed incorrectly. It is recommended to fix errors.${NC}"
	fi
else
	echo -e "$w2 GTests complited successfully!"
fi

rm -f log.txt
cd ..
echo -e "$w2 Running Menu"
./menu.sh "$1"