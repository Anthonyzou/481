#!/usr/bin/env bash


#cmake .
#make asn1
#make asn1test

for elements in $(seq 1000 1000 10000)
do
    numProcessors=$(($(nproc)+2))
    for processor in $(seq 1 1 ${numProcessors})
    do
        echo ${processor}
    done

    ARRAY1=()
    for i in $(seq 5)
    do
        ARRAY1+=($i)
    done

    ARRAY2=()
    for i in $(seq 5)
    do
        ARRAY2+=($i)
    done
done


