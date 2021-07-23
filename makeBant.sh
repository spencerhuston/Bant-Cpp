#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NONE='\033[0m'

# Build
echo -e "${YELLOW}BUILDING...${NONE}\n"
if [[ $# -eq 1 && "$1" == "-c" ]]; then
    make clean
fi
make -j

if [[ $? -eq 0 ]]; then
    echo -e "\n${GREEN}BUILD SUCCESSFUL${NONE}\n"
else
    echo -e "\n${YELLOW}BUILD FAILED${NONE}"
    exit 1
fi

# Static Analysis
echo -e "\n${YELLOW}RUNNING STATIC ANALYSIS...${NONE}\n"
cppcheck --enable=all --error-exitcode=1 .

if [[ $? -eq 1 ]]; then
    echo -e "${RED}STATIC ANALYSIS FAILED${NONE}"
    exit 1
else
    echo -e "${GREEN}STATIC ANALYSIS SUCCESSFUL${NONE}\n"
fi

# Testing
cd ./tests && ./run_tests.sh
testsResult=$(echo $?)
cd ..

if [[ testsResult -eq 0 ]]; then
    echo -e "${YELLOW}=====================\n${GREEN}FULL BUILD SUCCESSFUL\n${YELLOW}=====================${NONE}"
else
    echo -e "${YELLOW}=================\n${RED}FULL BUILD FAILED\n${YELLOW}=================${NONE}"
fi