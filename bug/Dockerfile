FROM debian:testing-slim

COPY input /input

RUN apt-get update && apt-get -y install unzip cmake g++

RUN /input/build.sh

ENTRYPOINT ["/input/run.sh"]
