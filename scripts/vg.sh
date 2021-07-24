#!/bin/bash

valgrind --verbose \
	--error-limit=no \
	--leak-check=full \
	--track-origins=yes \
	--error-exitcode=1 \
	./build/bant $1