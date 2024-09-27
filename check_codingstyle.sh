#!/bin/bash

function usage() {
	local bold="\033[1m"
	local red="\033[31m"
	local green="\033[32m"
	local yellow="\033[33m"
	local reset="\033[0m"

	echo -e "${bold}Usage: ${green}$0 ${red}<fpath>${reset}

Before this script gets executed, you need to download the checkpatch by yourself since it is GPL licensed:
${bold}
${green}wget -O ${yellow}./Tools/scripts/checkpatch.pl ${red}https://raw.githubusercontent.com/torvalds/linux/master/scripts/checkpatch.pl
${green}wget -O ${yellow}./Tools/scripts/const_structs.checkpatch ${red}https://raw.githubusercontent.com/torvalds/linux/master/scripts/const_structs.checkpatch
${green}wget -O ${yellow}./Tools/scripts/spelling.txt ${red}https://raw.githubusercontent.com/torvalds/linux/master/scripts/spelling.txt
${reset}"
}

function main() {
	if [ $# -ne 1 ]; then
		usage
		exit 1
	fi

	perl Tools/scripts/checkpatch.pl --no-tree --file $1
}

main $@
