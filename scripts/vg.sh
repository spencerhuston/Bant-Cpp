#!/bin/bash

valgrind --verbose \
	--error-limit=no \
	--leak-check=full \
	--track-origins=yes \
	$1 $2