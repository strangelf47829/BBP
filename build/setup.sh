#!/bin/bash

# Get line with format "NAME=*"
DISTR_LINE=$(cat /etc/*-release | grep -E "^NAME=")

# Remove the preceding 'NAME=' proportion
DISTR_SPLIT=(${DISTR_LINE//=/ })
DISTR_SPLIT=${DISTR_SPLIT[1]}

# Sanitize any quotes or any other illegal characters
DISTR=(${DISTR_SPLIT//\"/})

# Show detected platform
echo "Detected platform: $DISTR"
echo
echo

# Now setup enviromnent
make -f makefile.guess guess GUESSING="$DISTR" SHOULDRUN="yes"

