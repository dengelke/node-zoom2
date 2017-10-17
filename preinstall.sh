#!/bin/sh
#
# init IndexData ubuntu deb repository

sources_list_d=/etc/apt/sources.list.d
indexdata_list=indexdata.list
apt_key=http://ftp.indexdata.dk/debian/indexdata.asc
deb_url=http://ftp.indexdata.dk

set -e

init_apt() {
    if [[ "$OSTYPE" == "linux-gnu" ]]; then
        file="$sources_list_d/$indexdata_list"
        os=ubuntu

        if [ ! -e $file ]; then 
        codename=$(lsb_release -c -s)
            wget -O- $apt_key | sudo apt-key add -
            sudo sh -c "echo deb $deb_url/${os} ${codename} main > $file.tmp"
            sudo mv -f $file.tmp $file
            sudo apt-get update -qq
            sudo apt-get install -y libyaz5-dev
        fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        brew install yaz
    fi
}

init_apt