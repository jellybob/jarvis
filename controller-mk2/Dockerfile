FROM phusion/baseimage:0.9.15

# Set up Base Image basics.
ENV HOME /root # Home directory
ENV DEBIAN_FRONTEND noninteractive

# Disable SSH, prefer docker exec
RUN rm -rf /etc/service/sshd /etc/my_init.d/00_regen_ssh_host_keys.sh

CMD [ "/sbin/my_init" ] # Init

# Install RabbitMQ, requires HOME to be set for rabbmitmq-plugins to
# find the Erlang cookie.
VOLUME [ "/var/lib/rabbitmq" ]
ENV RABBITMQ_BASE /var/lib/rabbitmq
ENV RABBITMQ_URL amqp://localhost

ENV HOME /var/lib/rabbitmq

RUN apt-get -qq update && apt-get install -y rabbitmq-server
RUN /usr/sbin/rabbitmq-plugins enable rabbitmq_management
RUN mkdir /etc/service/rabbitmq
ADD docker/service/rabbitmq/run /etc/service/rabbitmq/run

# Flip back to root now activating plugins is done.
ENV HOME /root

# Install Ruby and dependencies
RUN apt-get install -y ruby2.0
# See https://bugs.launchpad.net/ubuntu/+source/ruby2.0/+bug/1310292
RUN ln -sf /usr/bin/ruby2.0 /usr/bin/ruby && ln -sf /usr/bin/gem2.0 /usr/bin/gem
RUN gem install --no-ri --no-rdoc bundler

# Give Jarvis it's own user
RUN adduser --system --home /srv/jarvis --group --disabled-password jarvis

# Grab the latest version of Jarvis
RUN apt-get install -y git-core
ADD . /srv/jarvis
RUN chown -R jarvis:jarvis /srv/jarvis
WORKDIR /srv/jarvis
RUN /sbin/setuser jarvis bundle check || /sbin/setuser jarvis bundle install --path vendor/bundle
ADD docker/service/jarvis-heartbeat-listener/run /etc/service/jarvis-heartbeat-listener/run

EXPOSE 5672
EXPOSE 15672

# Tidy up apt detritus
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

ENTRYPOINT /sbin/my_init
