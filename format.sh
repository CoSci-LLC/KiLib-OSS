#!/bin/bash

function format() {
    echo "~~~ $@ Starting ~~~";
    for f in $(find $@ -name '*.hpp' -or -name '*.cpp'); do 
        echo "format ${f}";
        sed -i 's/#pragma omp/\/\/#pragma omp/g' ${f}
        clang-format -i -style=file ${f};
        sed -i 's/\/\/ *#pragma omp/#pragma omp/g' ${f}
    done

    echo "~~~ $@ Done ~~~";
}

# Check all of the arguments first to make sure they're all directories
for dir in src/ include/; do
    if [ ! -d "${dir}" ]; then
        echo "${dir} is not a directory";
    else
        format ${dir};
    fi
done