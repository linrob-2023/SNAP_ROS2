#!/bin/bash

source colcon-build.sh
if [ $? -eq 0 ]
then
    echo " "
else
    exit 1
fi

chmod +x wrapper/run-position-controller.sh
snapcraft clean --destructive-mode
snapcraft --destructive-mode
