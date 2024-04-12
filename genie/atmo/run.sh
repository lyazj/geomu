#!/bin/bash

FLUX=""
FLUX=${FLUX}$([ -z "${FLUX}" ] || echo ,)BGLRS-Kamioka/f210_3_z.kam_nue[12]   # nu_e
FLUX=${FLUX}$([ -z "${FLUX}" ] || echo ,)BGLRS-Kamioka/f210_3_z.kam_nbe[-12]  # antinu_e
FLUX=${FLUX}$([ -z "${FLUX}" ] || echo ,)BGLRS-Kamioka/f210_3_z.kam_num[14]   # nu_mu
FLUX=${FLUX}$([ -z "${FLUX}" ] || echo ,)BGLRS-Kamioka/f210_3_z.kam_nbm[-14]  # antinu_mu
FLUX=BGLRS:${FLUX}

GEOMETRY=""
GEOMETRY=${GEOMETRY}$([ -z "${GEOMETRY}" ] || echo ,)1000080160[1.000000]  # O16

XS_FILE="genie_xsec/v3_04_00/NULL/G2111a00000-k250-e1000/data/gxspl-NUsmall.xml"
TUNE="G21_11a_00_000"

LENGTH_UNIT="m"
DENSITY_UNIT="kg_m3"
NEVENT="100000"
ENERGY_RANGE="1,999"  # GeV
COS_THETA_RANGE="0,1"
OUTPUT_PREFIX="${ENERGY_RANGE}"
SEED="$(date +%Y%m%d%H%M%S)"
RUN_NUMBER="${SEED}"
MESSAGE_CONFFILE="Messenger_laconic.xml"
INFILE="${OUTPUT_PREFIX}.${RUN_NUMBER}.ghep.root"
OUTFILE="$(sed 's@\.ghep\.root$@.ntuple.root@' <<< "${INFILE}")"

while [ -e "${INFILE}" ]; do
    sleep 1.$[$RANDOM % 1000]
done

source ../env.sh

#echo gevgen_atmo -w -f "${FLUX}" -g "${GEOMETRY}" \
#    -L "${LENGTH_UNIT}" -D "${DENSITY_UNIT}" -n "${NEVENT}" -E "${ENERGY_RANGE}" -C "${COS_THETA_RANGE}" \
#    -o "${OUTPUT_PREFIX}" -r "${RUN_NUMBER}" --seed "${SEED}" \
#    --cross-sections "${XS_FILE}" --tune "${TUNE}" --message-thresholds "${MESSAGE_CONFFILE}"
#exit  # DEBUG
stdbuf -oL gevgen_atmo -w -f "${FLUX}" -g "${GEOMETRY}" \
    -L "${LENGTH_UNIT}" -D "${DENSITY_UNIT}" -n "${NEVENT}" -E "${ENERGY_RANGE}" -C "${COS_THETA_RANGE}" \
    -o "${OUTPUT_PREFIX}" -r "${RUN_NUMBER}" --seed "${SEED}" \
    --cross-sections "${XS_FILE}" --tune "${TUNE}" --message-thresholds "${MESSAGE_CONFFILE}" \
    2>&1 | tee run-"${SEED}".log
rm -f "${OUTFILE}"
stdbuf -oL ../../bin/ntuple-ntpmc "${OUTFILE}" "${INFILE}" 2>&1 | tee -a run-"${SEED}".log
