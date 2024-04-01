#!/bin/bash

export GENIE=/home/lyazj/software/genie_generator
export ROOTSYS=/usr/local
export LHAPATH=/usr/local/share/LHAPDF
export PATH=${PATH}:${GENIE}/bin
if [ -z "${CPLUS_INCLUDE_PATH}" ]; then
    export CPLUS_INCLUDE_PATH=${GENIE}/src
else
    export CPLUS_INCLUDE_PATH=${LD_LIBRARY_PATH}:${GENIE}/src
fi
if [ -z "${LD_LIBRARY_PATH}" ]; then
    export LD_LIBRARY_PATH=${GENIE}/lib
else
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${GENIE}/lib
fi
export LD_PRELOAD=/usr/local/lib/libG4geometry.so:/usr/local/lib/libG4processes.so
