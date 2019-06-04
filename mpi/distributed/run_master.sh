#!/bin/bash
NAME=master

docker rm -f $NAME
docker run \
	-d \
	-p 127.0.0.1:32768:22/tcp \
	--name $NAME \
	mpi:v2
