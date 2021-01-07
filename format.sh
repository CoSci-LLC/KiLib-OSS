#!/bin/bash

echo "~~~ KiLib/ Starting ~~~";
for f in $(find KiLib/ -name '*.hpp' -or -name '*.cpp'); do 
    echo "format ${f}";
    sed -i 's/#pragma omp/\/\/#pragma omp/g' ${f}
    clang-format -i -style=file ${f};
    sed -i 's/\/\/ *#pragma omp/#pragma omp/g' ${f}
done

echo "~~~ KiLib/ Done ~~~";