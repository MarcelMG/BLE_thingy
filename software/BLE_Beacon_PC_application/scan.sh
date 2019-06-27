#!/bin/bash
# restart Bluettoth dongle to avoid error that occurs sometimes
hciconfig $1 down
hciconfig $1 up
# start BLE scan and dump raw data, parse raw data in C++ program
hcitool  -i $1 lescan --duplicates > /dev/null & hcidump -i $1 --raw | ./hcidump_parse
