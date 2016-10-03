#!/usr/bin/env bash


#cmake .
#make asn1
#make asn1test

for k in $(seq 1000 1000 10000)
do
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


