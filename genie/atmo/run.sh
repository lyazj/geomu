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

LENGTH_UNIT="L"
DENSITY_UNIT="kg_m3"
NEVENT="100000"
ENERGY_RANGE="50,5000"  # GeV
OUTPUT_PREFIX="${ENERGY_RANGE}"
SEED="$(date +%Y%m%d%H%M%S)"
RUN_NUMBER="${SEED}"

source ../env.sh
stdbuf -oL gevgen_atmo -f "${FLUX}" -g "${GEOMETRY}" \
    -L "${LENGTH_UNIT}" -D "${DENSITY_UNIT}" -n "${NEVENT}" -E "${ENERGY_RANGE}" \
    -o "${OUTPUT_PREFIX}" -r "${RUN_NUMBER}" --seed "${SEED}" \
    --cross-sections "${XS_FILE}" --tune "${TUNE}" \
    2>&1 | tee run.log
