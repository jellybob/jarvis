FROM ubuntu:trusty

ENV DEBIAN_FRONTEND noninteractive

# TODO: Find a way to depend on apt installable versions of PG and Bunny to
# avoid including a full build-essential toolchain here.
RUN apt-get -qq update && apt-get install -y ruby2.0 ruby2.0-dev libpq-dev build-essential
RUN ln -sf /usr/bin/ruby2.0 /usr/bin/ruby && ln -sf /usr/bin/gem2.0 /usr/bin/gem
RUN gem install --no-ri --no-rdoc bundler
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN mkdir -p /srv/jarvis
ADD . /srv/jarvis
WORKDIR /srv/jarvis
RUN bundle check || bundle install

CMD ./docker/jarvis-runner jarvis-heartbeat-listener
