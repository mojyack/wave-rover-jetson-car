#!/bin/bash

hostapd /root/hostapd.conf &
cd /root/controller
python3 daemon.py
