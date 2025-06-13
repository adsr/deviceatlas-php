FROM debian:testing-slim

RUN apt-get update && apt-get -y install unzip cmake g++

COPY input /input

RUN /input/build.sh

ENTRYPOINT ["/input/run.sh"]
