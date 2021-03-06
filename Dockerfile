FROM debian:latest

RUN apt-get update && \
  apt-get install -y \
    build-essential wget python git \
    libglfw3-dev libglew-dev libfreetype6-dev libfontconfig1-dev uuid-dev libxcursor-dev libxinerama-dev libxi-dev libasound2-dev libexpat1-dev \
    libnss3-dev libxcomposite-dev libxtst-dev libxss-dev libdbus-1-dev libpango-1.0-0 libpangocairo-1.0-0 libatk1.0-0 libatk-bridge2.0-0

ADD . /app
WORKDIR /app

RUN \
  wget "https://nodejs.org/dist/v10.14.2/node-v10.14.2-linux-x64.tar.gz" -O node.tar.gz && \
  tar -zxf node.tar.gz && \
  rm node.tar.gz && \
  mv node-v10.14.2-linux-x64 node
RUN \
  export PATH="$PATH:$(pwd)/node/bin" && \
  npm install --unsafe-perm . && \
  npm run test:ci
RUN \
  mkdir -p /tmp/exokit-bin/bin /tmp/exokit-bin/lib/exokit && \
  cp -R . /tmp/exokit-bin/lib/exokit && \
  cp scripts/exokit-bin.sh /tmp/exokit-bin/bin/exokit && \
  cd /tmp/exokit-bin && \
  tar -czf /app/exokit-linux-bin.tar.gz --exclude=".*" --exclude="*.tar.gz" * && \
  cd /app && \
  tar -czf exokit-linux-full.tar.gz --exclude=".*" --exclude="*.tar.gz" * && \
  rm -R node && \
  tar -czf exokit-linux.tar.gz --exclude=".*" --exclude="*.tar.gz" *

