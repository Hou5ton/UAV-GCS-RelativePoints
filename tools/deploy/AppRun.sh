#!/bin/bash
HERE="$(dirname "$(readlink -f "${0}")")"
export LD_LIBRARY_PATH=${HERE}/usr/lib:${HERE}/usr/lib/gcs:$LD_LIBRARY_PATH
exec "${HERE}/usr/bin/gcs" "$@"
