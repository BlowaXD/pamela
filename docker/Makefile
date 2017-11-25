### AUTHOR :
### DOOM & BLOWA

PROJECT=pamela
CONTAINER_NAME=$(PROJECT):latest
PORT=2222
DEFAULT_USER=root

killall:	.FORCE
		docker kill $$(docker ps | sed -r -n '/^[^ ]+ +$(CONTAINER_NAME) / {s/ .*$$//;p}')

build:		.FORCE
		docker build -t $(CONTAINER_NAME) .

rebuild:	.FORCE
		docker build --no-cache -t $(CONTAINER_NAME) .

all:		build .FORCE
		docker run -d -p $(PORT):22 -e SSH_KEY="$$(cat ~/.ssh/id_rsa.pub)" $(CONTAINER_NAME)
		while ! ssh $(DEFAULT_USER)@localhost -p $(PORT) -o "StrictHostKeyChecking=no" env; do sleep 0.1; done
		docker kill $$(docker ps -ql)

debug-ssh:	build .FORCE
		docker run -p $(PORT):22 -e SSH_KEY="$$(cat ~/.ssh/id_rsa.pub)" $(CONTAINER_NAME)

debug-connect:	.FORCE
		ssh $(DEFAULT_USER)@localhost -p $(PORT) -o "StrictHostKeyChecking=no" env

OPTS=--privileged --cap-add=all #-v /dev:/dev -v /lib/modules:/lib/modules

root-shell:	build .FORCE
		docker run -p $(PORT):22 -ti $(OPTS) $(CONTAINER_NAME) bash

.FORCE:
