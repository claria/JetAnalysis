#!/bin/bash

# source $MY_LANDINGZONE/gc-run.lib || exit 101

echo "---------------------"
echo "Prepare to run JetAna"
echo "---------------------"


echo "-------------------------"
echo "Listing current directory"
echo "-------------------------"

echo $PWD
ls -l
echo

echo "---------------------------------"
echo "Running with the following config"
echo "---------------------------------"
# $FILE_NAMES=echo "${FILE_NAMES//\"}"
dijetana.py -i ${FILE_NAMES//\"} --save-config config.json -c $CONFIG --log-level debug || exit $?

exit 0
