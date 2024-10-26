#! /bin/bash

# Get line with format "NAME=*"
DISTR_LINE=$(cat /etc/*-release | grep -E "^NAME=")

# Remove the preceding 'NAME=' proportion
DISTR_SPLIT=(${DISTR_LINE//=/ })
DISTR_SPLIT=${DISTR_SPLIT[1]}

# Sanitize any quotes or any other illegal characters
DISTR=(${DISTR_SPLIT//\"/})

# Check if 'makefile.config.$DISTR' exists
if [ -f "makefile.config.$DISTR" ]; then
	PLAT=$DISTR
else
	PLAT=generic
fi

# Echo stuff back
echo "Detected platform: $PLAT"

# Now setup enviromnent
cp makefile.config.$PLAT ../../Source/makefile.config
