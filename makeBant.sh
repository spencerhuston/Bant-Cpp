if [[ $# -eq 1 && "$1" == "-c" ]]; then
    make clean
fi

make -j

cd ./tests && ./run_tests.sh
cd ..