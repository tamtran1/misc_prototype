#!/bin/bash

dd if=/dev/zero of=/tmp/empty.dd bs=1048576; rm /tmp/empty.dd
dd if=/dev/zero of=/home/empty.dd bs=1048576; rm /home/empty.dd

