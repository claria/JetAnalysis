#!/bin/sh

source $MY_LANDINGZONE/gc-run.lib || exit 101

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

JetAna $ARTUSCONFIG || exit $?

exit 0
