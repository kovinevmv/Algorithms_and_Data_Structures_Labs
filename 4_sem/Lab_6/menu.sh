#!/bin/bash

RED="\e[1;31m"
GREEN="\e[1;32m"
YELLOW="\e[1;33m"
BLUE="\e[1;34m"
NC="\e[0m"

w1="[${RED}-${NC}]"
w2="[${GREEN}+${NC}]"
w3="[${YELLOW}?${NC}]"

echo
echo -e " ${BLUE}######################################################################################################${NC} "
echo -e " ${BLUE}#                                                                                                    #${NC} "
echo -e " ${BLUE}#${NC}                                               ${GREEN}Меню${NC}                                                 ${BLUE}#${NC}"
echo -e " ${BLUE}#${NC}                                                                                                    ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC}                     1. Запустить ./test_google     (GTests)                                        ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC}                     2. Запустить ./task_1          (Алгоритм Ахо-Корасик)                          ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC}                     3. Запустить ./task_2          (Поиск с джокером)                              ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC}                     4. Выход                                                                       ${BLUE}#${NC} "
echo -e " ${BLUE}#${NC}                                                                                                    ${BLUE}#${NC} "
echo -e " ${BLUE}######################################################################################################${NC} "
echo

if [ -z "$1" ];
then
	set -- "build"
fi

echo -n "Select one of the options [1-4]: "
read var
if [ "$var" = "1" ]
then
	echo -e "$w2 Running GTests:"
	"./$1/test_google"
elif [ "$var" = "2" ]
then
	echo -e "$w2 Running task_1:"
	"./$1/task_1"
elif [ "$var" = "3" ]
then
	echo -e "$w2 Running task_2:"
	"./$1/task_2"
elif [ "$var" = "4" ]
then
	exit 1
else
	echo -e "$w1 Wrong input!"
fi
