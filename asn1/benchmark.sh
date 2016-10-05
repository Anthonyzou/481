#!/usr/bin/env bash

seq=$(seq 8000000 1000000 12000000)

numProcessors=$(($(nproc)+2))
echo ${seq}


for processor in $(seq 1 1 ${numProcessors}); do
    file="${processor}.psrs"
    rm ${file}
    echo "["  >> ${file}
    for elements in ${seq}; do
        printf "["  >> ${file}
        for i in $(seq 5); do
            echo "[$(./asn1 -threads ${processor} -size ${elements})]," >> ${file}
        done
        printf "],"  >> ${file}
    done
    echo "]"  >> ${file}
done

file="seq.txt"
rm seq.txt
printf "[" >> ${file}
for elements in ${seq}; do
    printf "[" >> ${file}
    for i in $(seq 5); do
      echo "$(./asn1test -size ${elements})," >> ${file}
    done
    echo "]," >> ${file}
done
echo "]" >> ${file}
