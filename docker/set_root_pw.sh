#!/bin/bash

if [ -z "${SSH_KEY}" ]; then
	echo "=> Please pass your public key in the SSH_KEY environment variable"
	exit 1
fi

for MYHOME in /root /home/doom /home/blowa; do
	echo "=> Adding SSH key to ${MYHOME}"
	mkdir -p ${MYHOME}/.ssh
	chmod go-rwx ${MYHOME}/.ssh
	echo "${SSH_KEY}" > ${MYHOME}/.ssh/authorized_keys
	chmod go-rw ${MYHOME}/.ssh/authorized_keys
	echo "=> Done!"
done
chown -R doom:doom /home/doom/.ssh
chown -R blowa:blowa /home/blowa/.ssh

echo "========================================================================"
echo "You can now connect to the container through SSH using:"
echo ""
echo "    ssh -p <port> <user>@<host>"
echo ""
echo "Users: root (full access), doom/blowa (limited user account)."
echo "========================================================================"
