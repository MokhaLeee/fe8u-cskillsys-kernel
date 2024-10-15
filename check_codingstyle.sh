#!/bin/bash

function usage() {
	local bold="\033[1m"
	local red="\033[31m"
	local green="\033[32m"
	local yellow="\033[33m"
	local reset="\033[0m"

	echo -e "${bold}Usage: ${green}$0 ${red}<fpath>${reset}"
}

function main() {
	if [ $# -ne 1 ]; then
		usage
		exit 1
	fi

	perl Tools/check_patch/checkpatch.pl --no-tree --file $1
}

main $@
