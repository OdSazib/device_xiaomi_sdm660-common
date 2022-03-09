#!/bin/bash
#
# Copyright (C) 2020 Paranoid Android
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

DEVICE_COMMON=sdm660-common
GUARDED_DEVICES_COMMON="clover jasmine_sprout jason lavender platina tulip wayne whyred"
VENDOR=xiaomi

# Load extract_utils and do some sanity checks
COMMON_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$COMMON_DIR" ]]; then COMMON_DIR="$PWD"; fi

if [[ -z "$DEVICE_DIR" ]]; then
    DEVICE_DIR="${COMMON_DIR}/../${DEVICE}"
fi

ROOT="$COMMON_DIR"/../../..

HELPER="$ROOT"/tools/extract-utils/extract_utils.sh
if [ ! -f "$HELPER" ]; then
    echo "Unable to find helper script at $HELPER"
    exit 1
fi
. "$HELPER"

# Initialize the common helper
setup_vendor "$DEVICE_COMMON" "$VENDOR" "$ROOT" true

if ([[ "$ONLY_DEVICE" = "false" ]] || [[ -z "$ONLY_DEVICE" ]]) && [[ -s "${COMMON_DIR}"/proprietary-files.txt ]]; then
    # Copyright headers and guards
    write_headers "$GUARDED_DEVICES_COMMON"
    # The common blobs
    write_makefiles "$COMMON_DIR"/proprietary-files.txt true
    # Finish
    write_footers
fi
if ([[ "$ONLY_COMMON" = "false" ]] || [[ -z "$ONLY_COMMON" ]]) && [[ -s "${DEVICE_DIR}"/proprietary-files.txt ]]; then
    # Reinitialize the helper for device and write copyright headers and guards
    DEVICE_COMMON="$DEVICE"
    if [[ ! "$IS_COMMON" = "true" ]]; then
        IS_COMMON=false
        GUARDED_DEVICES=
    fi
    # Reinitialize the helper for device
    setup_vendor "$DEVICE" "$VENDOR" "$ROOT" "$IS_COMMON" "$CLEAN_VENDOR"
    # Copyright headers and guards
    write_headers "$GUARDED_DEVICES"
    # The standard device blobs
    write_makefiles "${DEVICE_DIR}"/proprietary-files.txt true
    # We are done!
    write_footers
fi
