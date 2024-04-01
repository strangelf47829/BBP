#!/bin/bash

# Check if argument is supplied
if [ "$#" -eq 1 ]; then
	FILE=$@
else
	echo "Must provide exactly one argument to return error list"
	exit 1
fi

# Set grep regexp rules
REGEXP_WARNINGS="-W[A-Z,a-z,-]*"
REGEXP_ERRORS="error: "

# Get lines for warnings
WARNINGS=$(grep -o -e $REGEXP_WARNINGS $FILE)
ERRORS=$(grep -o -e $REGEXP_ERRORS $FILE)

# Get word count.
WARNING_COUNT=$(echo $WARNINGS | wc -w)
ERROR_COUNT=$(echo $ERRORS | wc -w)

# If both are 0, remove log.
if [ $WARNING_COUNT -eq 0 ] && [ $ERROR_COUNT -eq 0 ]; then
	rm $FILE
	exit 0
fi

RED='\e[0;31m'
PURPLE='\e[0;45m'
GREEN='\e[0;32m'
NORM='\e[0;37;37m'

# Set warning text
if ! [ $WARNING_COUNT -eq 0 ]; then
WARNING_TEXT="${PURPLE}${WARNING_COUNT}${NORM} warnings"
else
WARNING_TEXT="${GREEN}0${NORM} warnings"
fi

# Set error text
if ! [ $ERROR_COUNT -eq 0 ]; then
ERROR_TEXT="${RED}${ERROR_COUNT}${NORM} errors"
else
ERROR_TEXT="${GREEN}0${NORM} errors"
fi

echo -e "| >\tSource compiled with ${WARNING_TEXT} and ${ERROR_TEXT} (See ${FILE})"
echo -e "|"
