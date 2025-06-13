DOCKER?=docker

all: run

build:
	$(DOCKER) build -t deviceatlas-php-bug .

run: build
	$(DOCKER) run -t deviceatlas-php-bug

shell: build
	$(DOCKER) run -it --entrypoint=/bin/bash deviceatlas-php-bug

clean:
	$(DOCKER) image rm -f deviceatlas-php-bug
