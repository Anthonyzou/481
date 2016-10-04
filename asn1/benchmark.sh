#!/usr/bin/env bash

seq=$(seq 200000 100000 1000000)

for elements in ${seq}
do
    numProcessors=$(($(nproc)+2))
    for processor in $(seq 1 1 ${numProcessors})
    do
        echo ${elements} ${processor}

        for i in $(seq 5)
        do
            echo $(./asn1)
        done

    done
done

for elements in ${seq}
do

    echo ${elements}

    for i in $(seq 5)
    do
        echo $(./asn1test)
    done

done



