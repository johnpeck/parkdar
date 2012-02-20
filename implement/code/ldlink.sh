#!/bin/bash
# 
# ldlink.sh -- Create the missing link to avr5.x
# Requires: mlocate
# You need to run this as root to create the symlink.


# Find the linker script path and save it to /tmp/ldfile
locate -b "\avr5.x" | sed s/avr5.x// > /tmp/ldfile
ldpath=$(cat /tmp/ldfile)
echo "Found inker script path in "$ldpath
ln -s $ldpath "/usr/avr/lib/ldscripts" # Create symlink
echo "Created symlink in /usr/avr/lib"
rm /tmp/ldfile # Clean up the temporary file

