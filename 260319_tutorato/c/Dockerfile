FROM ubuntu:22.04
RUN apt-get update && apt-get install build-essential make -y
COPY . /home/labos
RUN cd /home/labos && make -f Makefile.mk all
CMD cd /home/labos && ./analyzer argomenti di default