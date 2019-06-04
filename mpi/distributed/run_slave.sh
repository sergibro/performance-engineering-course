#!/bin/bash
NAME=slave$1

docker rm -f $NAME
docker run \
	-d \
	--name $NAME \
	--network mpinet \
	mpi:v2
