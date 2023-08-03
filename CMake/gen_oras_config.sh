#!/bin/sh

if [[ -f /etc/os-release ]]; then
	OS=$(awk  -F '=' '/^NAME/{print $2}' /etc/os-release)
	RELEASE=$(awk  -F '=' '/^VERSION_ID/{print $2}' /etc/os-release)
else
	OS="UNKNOWN"
	RELEASE="UNKNOWN"
fi

ARCH=$(uname -m)
LIBRARY="KiLib-OSS"
GIT_HASH=$(git log -1 --format='%h')

cat << EOF 
{
    "os": ${OS},
    "release": ${RELEASE},
    "mediaType": "appication/octet-stream",
    "architecture": "${ARCH}",
    "library": "${LIBRARY}",
    "maintainer": "dev@cosci-llc.com",
    "git-hash": "${GIT_HASH:-UNKNOWN}"
}
EOF

