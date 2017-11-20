FROM debian:latest
#
#
#

# Install packages
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssh-server sudo
ADD set_root_pw.sh /set_root_pw.sh
ADD run.sh /run.sh
RUN chmod +x /*.sh
RUN mkdir -p /var/run/sshd && sed -i "s/UsePrivilegeSeparation.*/UsePrivilegeSeparation no/g" /etc/ssh/sshd_config \
  && sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config \
  && touch /root/.Xauthority \
  && true

## Set a user blowa. Available via runtime flag `--user blowa`
## Add user to 'staff' group, granting them write privileges to /usr/local/lib/R/site.library
## User should also have & own a home directory, but also be able to sudo
RUN useradd blowa \
        && passwd -d blowa \
        && mkdir /home/blowa \
        && chown blowa:blowa /home/blowa \
        && addgroup blowa staff \
        && addgroup blowa sudo \
        && true

## Set a user doom. Available via runtime flag `--user doom`
## Add user to 'staff' group, granting them write privileges to /usr/local/lib/R/site.library
## User should also have & own a home directory, but also be able to sudo
RUN useradd doom \
        && passwd -d doom \
        && mkdir /home/doom \
        && chown doom:doom /home/doom \
        && addgroup doom staff \
        && addgroup doom sudo \
        && true

EXPOSE 22
CMD ["/run.sh"]