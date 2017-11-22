FROM debian:latest

# Install packages
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssh-server sudo
RUN apt-get install -y build-essential libpam0g-dev git emacs
#RUN apt-get install -y busybox console-setup console-setup-linux cpio initramfs-tools initramfs-tools-core kbd keyboard-configuration klibc-utils kmod libklibc liblocale-gettext-perl linux-base udev
ADD set_root_pw.sh /set_root_pw.sh
ADD run.sh /run.sh
RUN chmod +x /*.sh
RUN mkdir -p /var/run/sshd && sed -i "s/UsePrivilegeSeparation.*/UsePrivilegeSeparation no/g" /etc/ssh/sshd_config \
  && sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config \
  && touch /root/.Xauthority \
  && true

RUN useradd blowa -s /bin/bash \
        && passwd -d blowa \
        && mkdir /home/blowa \
        && chown blowa:blowa /home/blowa \
        && addgroup blowa staff \
        && addgroup blowa sudo \
        && true

RUN useradd doom -s /bin/bash \
        && passwd -d doom \
        && mkdir /home/doom \
        && chown doom:doom /home/doom \
        && addgroup doom staff \
        && addgroup doom sudo \
        && true

EXPOSE 22
CMD ["/run.sh"]