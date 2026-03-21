FROM ubuntu:22.04
RUN apt-get update && apt-get install build-essential make -y
WORKDIR /home/labOS
COPY *.c *.h Makefile.mk ./
RUN make -f Makefile.mk all
CMD ./analyzer argomenti di default