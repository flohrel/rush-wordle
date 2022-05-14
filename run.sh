#!/bin/bash

# FG COLORS
DEFAULT='\033[0m'
BLACK='\033[1;30m'
RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
BLUE='\033[1;34m'
MAGENTA='\033[1;35m'
CYAN='\033[1;36m'
WHITE='\033[1;107m'

# TERMCAPS
UP='\033[1A'
DELETE='\033[2K'
DELPREV="${UP}${DELETE}\r"

# EMOJI
CHECK='\xE2\x9C\x94'
CROSS='\xE2\x9D\x8C'

# SCRIPT VARS
LOGDIR='log'
RETURN_VALUE=0

header()
{
	set -o noglob			# disable globbing to prevent PAD_CHAR from expanding

	HEAD_SIZE=64
	NAME_SIZE=${#1}
	PAD_WIDTH=$(((${HEAD_SIZE} - ${NAME_SIZE}) / 2))
	PAD_PREC=$((${PAD_WIDTH} / 2))
	PAD_CHAR='*'
	PAD_STR=`eval printf "${PAD_CHAR}%.0s" {1..${PAD_WIDTH}}`
	LEFT_PAD=`printf '%-*.*s' ${PAD_WIDTH} ${PAD_PREC} ${PAD_STR}`
	RIGHT_PAD=`printf '%*.*s' $((${PAD_WIDTH} + ${NAME_SIZE} % 2)) ${PAD_PREC} ${PAD_STR}`
	BODY_WIDTH=$((${HEAD_SIZE} - 1))
	
	printf "${LEFT_PAD}${BLUE}${1}${DEFAULT}${RIGHT_PAD}\n"

	set +o noglob			# restore globbing
}

compile()
{
	printf "%-*s" ${BODY_WIDTH} "Compiling..."
	make -sC tester INCLUDE="../${INCLUDE}" &> ${LOGFILE}
	if [[ "$?" -ne 0 ]]; then
		printf "${RED}${CROSS}${DEFAULT}\n"
		printf "Error: compilation failed. See ${LOGFILE} for more details.\n"
		exit "$?"
	else
		printf "${GREEN}${CHECK}${DEFAULT}\n"
	fi
}

execute()
{
	TOTAL=0
	for TEST_RUNNER in `ls tester/run_*`; do
		if [[ ! -e ${TEST_RUNNER} ]]; then
			printf "Error: ${TEST_RUNNER}: binary not found.\n"
			exit "$?"
		elif [[ ! -x ${TEST_RUNNER} ]]; then
			chmod +x ${TEST_RUNNER}
		fi

		printf "%-*s" ${BODY_WIDTH} "Executing ${TEST_RUNNER##*/}..."

		./${TEST_RUNNER} &>> ${LOGFILE}

		RETURN_VALUE=$((${RETURN_VALUE}+"$?"))

		local TIME="[$(tac ${LOGFILE} | grep 'Master' | grep 'testing' | grep -Eo "[+-]?([0-9]*[.])?[0-9]+ms$" | head -1)]"
		printf "\033[%dD%s " $((${#TIME} + 1)) ${TIME}

		if [[ "$?" -ne 0 ]]; then
			printf "${RED}${CROSS}${DEFAULT} => errors reported in ${LOGFILE}."
		else
			printf "${GREEN}${CHECK}${DEFAULT}"
		fi
		printf "\n"

		TOTAL=`echo "$(echo ${TIME} | grep -Eo "[+-]?([0-9]*[.])?[0-9]+") + ${TOTAL}" | bc `
	done
}

tester()
{
	header "$1"
	LOGFILE="${LOGDIR}/${1,,}.out"
	compile "$1"
	execute "$1"
	make -sC tester fclean &>> ${LOGFILE}
	printf "${MAGENTA}%-*s" ${BODY_WIDTH} "Total time elapsed:"
	printf "\033[%dD%sms${DEFAULT}\n" $((${#TOTAL} + 1)) ${TOTAL}
}

print_usage()
{
	printf "Usage: ./run.sh INCLUDE_DIR PROJECT_NAME\n"
}

if [[ "$#" -ne 2 ]]; then
	echo $1
	print_usage
	exit 1
fi

mkdir -p log
if [[ ! -d "./log" ]]; then
	printf "Error: cannot create log folder.\n"
	exit 1
fi

if [[ ! -d "$1" ]]; then
	printf "Error: \"$1\" is not a directory.\n"
	exit 1
else
	INCLUDE="$1"
fi

tester "$2"

exit ${RETURN_VALUE}
