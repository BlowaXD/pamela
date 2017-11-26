FROM debian:latest

# Install packages
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssh-server sudo bash-completion
RUN apt-get install -y build-essential libpam0g-dev git emacs unzip
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y cryptsetup
ADD set_root_pw.sh /set_root_pw.sh
ADD run.sh /run.sh
RUN chmod +x /*.sh
RUN mkdir -p /var/run/sshd && sed -i "s/UsePrivilegeSeparation.*/UsePrivilegeSeparation no/g" /etc/ssh/sshd_config \
  && sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config \
  && touch /root/.Xauthority \
  && true

RUN useradd porn -s /bin/bash \
        && echo "porn:porn" | chpasswd \
        && mkdir /home/porn \
        && chown porn:porn /home/porn \
        && addgroup porn staff \
        && addgroup porn sudo \
        && true

RUN useradd anarchy -s /bin/bash \
        && echo "anarchy:anarchy" | chpasswd \
        && mkdir /home/anarchy \
        && chown anarchy:anarchy /home/anarchy \
        && addgroup anarchy staff \
        && addgroup anarchy sudo \
        && true

RUN echo >> /etc/bash.bashrc && echo 'PATH=$PATH:/sbin/' >> /etc/bash.bashrc

RUN echo >> /etc/sudoers && echo '%staff ALL=(root) NOPASSWD:/sbin/*,/bin/mount,/bin/umount' >> /etc/sudoers

EXPOSE 22
CMD ["/run.sh"]