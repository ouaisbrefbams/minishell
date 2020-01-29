#!/bin/sh

red() {
	echo "`tput setaf 1`$1`tput sgr 0`\c"
	test_status=1
}

green() {
	echo "`tput setaf 2`$1`tput sgr 0`\c"
}

echo "minishell test"
