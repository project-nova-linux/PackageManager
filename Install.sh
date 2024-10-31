#!/bin/bash

set -e

echo "Install npkg on root(/) or local(~/.local)?"

read -p "> " input

if [[ $input == "local" || $input == "2nd" || $input == "2" || $input == "~/.local" ]]; then
	echo "Installing on ~/.local"
	mkdir -p ~/.local/bin
	cp Bin/npkg-local ~/.local/bin/npkg
	mkdir -p ~/.local/npkg
	cp -r Source/* ~/.local/npkg
elif [[ $input == "root" || $input == "1st" || $input == "1" || $input == "/" ]]; then
	echo "Installing on / not yet supported"
	exit
else
	echo "Unknown option"
	. $(basename $0) && exit
fi
