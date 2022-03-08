#!/bin/bash
#
# Copyright (C) 2020 Paranoid Android
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

DEVICE_COMMON=sdm660-common
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

# Default to sanitizing the vendor folder before extraction
CLEAN_VENDOR=true
ONLY_COMMON=false
ONLY_DEVICE=false

while [ "${#}" -gt 0 ]; do
    case "${1}" in
        -o | --only-common )
                ONLY_COMMON=true
                ;;
        -d | --only-device )
                ONLY_DEVICE=true
                ;;
        -n | --no-cleanup )
                CLEAN_VENDOR=false
                ;;
        -k | --kang )
                KANG="--kang"
                ;;
        -s | --section )
                SECTION="${2}"; shift
                CLEAN_VENDOR=false
                ;;
        * )
                SRC="${1}"
                ;;
    esac
    shift
done

if [ -z "$SRC" ]; then
    SRC=adb
fi

function blob_fixup() {
    case "${1}" in

    lib64/libwfdnative.so | lib/libwfdnative.so | lib/libwfdservice.so | lib/libwfdcommonutils.so | lib/libwfdmmsrc.so | lib/libwfdmmsink.so)
        "${PATCHELF}" --add-needed "libshim_wfd.so" "${2}"
        ;;

    vendor/lib64/libril-qc-hal-qmi.so)
        "${PATCHELF}" --replace-needed "libprotobuf-cpp-full.so" "libprotobuf-cpp-full-v29.so" "${2}"
        ;;

    esac

    device_blob_fixup "$@"
}

if ! typeset -f device_blob_fixup > /dev/null; then
    device_blob_fixup() {
        :
    }
fi

# Initialize the common helper
setup_vendor "$DEVICE_COMMON" "$VENDOR" "$ROOT" true $CLEAN_VENDOR

if [[ "$ONLY_DEVICE" = "false" ]] && [[ -s "${COMMON_DIR}"/proprietary-files.txt ]]; then
    extract "$COMMON_DIR"/proprietary-files.txt "$SRC" "${KANG}" --section "${SECTION}"
fi
if [[ "$ONLY_COMMON" = "false" ]] && [[ -s "${DEVICE_DIR}"/proprietary-files.txt ]]; then
    if [[ ! "$IS_COMMON" = "true" ]]; then
        IS_COMMON=false
    fi
    # Reinitialize the helper for device
    setup_vendor "$DEVICE" "$VENDOR" "$ROOT" "$IS_COMMON" "$CLEAN_VENDOR"
    extract "${DEVICE_DIR}"/proprietary-files.txt "$SRC" "${KANG}" --section "${SECTION}"
fi

"$COMMON_DIR"/setup-makefiles.sh
