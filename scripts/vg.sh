#!/bin/bash

valgrind --verbose \
	--error-limit=no \
	--leak-check=full \
	--track-origins=yes \
	./build/bant $1