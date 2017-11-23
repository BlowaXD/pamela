FROM debian:latest

# Install packages
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssh-server sudo
RUN apt-get install -y build-essential libpam0g-dev git emacs unzip
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y cryptsetup
ADD set_root_pw.sh /set_root_pw.sh
ADD run.sh /run.sh
RUN chmod +x /*.sh
RUN mkdir -p /var/run/sshd && sed -i "s/UsePrivilegeSeparation.*/UsePrivilegeSeparation no/g" /etc/ssh/sshd_config \
  && sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config \
  && touch /root/.Xauthority \
  && true

RUN useradd blowa -s /bin/bash \
        && echo "blowa:blowa" | chpasswd \
        && mkdir /home/blowa \
        && chown blowa:blowa /home/blowa \
        && addgroup blowa staff \
        && addgroup blowa sudo \
        && true

RUN useradd doom -s /bin/bash \
        && echo "doom:doom" | chpasswd \
        && mkdir /home/doom \
        && chown doom:doom /home/doom \
        && addgroup doom staff \
        && addgroup doom sudo \
        && true

EXPOSE 22
CMD ["/run.sh"]