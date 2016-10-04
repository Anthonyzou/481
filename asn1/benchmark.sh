#!/usr/bin/env bash

seq=$(seq 200000 100000 1000000)
rm seq.txt *.psrs
numProcessors=$(($(nproc)+2))
echo ${seq}
for processor in $(seq 2 1 ${numProcessors})
do
    file="${processor}.psrs"
    echo "["  >> ${file}
    for elements in ${seq}
    do
        printf "["  >> ${file}
        for i in $(seq 5)
        do
            echo "[$(./asn1 -threads ${processor} -size ${elements})]," >> ${file}
        done
        printf "],"  >> ${file}

    done
    echo "]"  >> ${file}
done


file="seq.txt"
for elements in ${seq}
do
    printf ${elements} >> ${file}
    printf ":[" >> seq.txt
    for i in $(seq 5)
    do
        printf $(./asn1test)"," >> ${file}
    done
    echo "]" >> ${file}
    echo "" >> ${file}
done



