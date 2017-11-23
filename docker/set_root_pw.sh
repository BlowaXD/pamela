#!/bin/bash

if [ -z "${SSH_KEY}" ]; then
    echo "=> Please pass your public key in the SSH_KEY environment variable"
    exit 1
fi

echo "=> Adding SSH key to /root"
mkdir -p /root/.ssh
chmod go-rwx /root/.ssh
echo "${SSH_KEY}" > /root/.ssh/authorized_keys
chmod go-rw /root/.ssh/authorized_keys
echo "=> Done!"

echo "========================================================================"
echo "            You can now connect to the container through SSH"
echo ""
echo "      Users: root (full access), doom/blowa (regular user account)"
echo "========================================================================"
