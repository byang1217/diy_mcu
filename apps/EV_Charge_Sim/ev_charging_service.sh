#!/bin/bash

#don't run as root

while true; do
        if [ -e /dev/ttyACM0 ]; then
                echo "attached"
                sleep 0.1
                echo id > /dev/ttyACM0
                stty -F /dev/ttyACM0 -icanon
                id=`timeout 3 dd if=/dev/ttyACM0 bs=8 count=1 2>/dev/null`
                echo "id: $id"
                while test -e /dev/ttyACM0; do
                        sleep 0.1
                done
                echo "dettached"
        fi
done

