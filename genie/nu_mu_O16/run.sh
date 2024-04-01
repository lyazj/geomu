#!/bin/bash

source ../env.sh
gevgen -n 100000 -p 14 -t 1000080160 -e 0,10 -f 'x*exp(-x)' --run 1000 --seed 1721827 \
    --cross-sections genie_xsec/v3_04_00/NULL/G2111a00000-k250-e1000/data/gxspl-NUsmall.xml \
    --tune G21_11a_00_000 &> run.log &
