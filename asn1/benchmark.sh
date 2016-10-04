#!/usr/bin/env bash

seq=$(seq 2000000 1000000 5000000)

numProcessors=$(($(nproc)+2))
echo ${seq}

psrs(){
    rm *.psrs

    for processor in $(seq 1 1 ${numProcessors})
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
}

seq(){
    file="seq.txt"
    rm seq.txt
    printf "[" >> seq.txt
    for elements in ${seq}
    do
        printf "[" >> seq.txt
        for i in $(seq 5)
        do
            printf $(./asn1test -size ${elements})"," >> ${file}
        done
        echo "]," >> ${file}
    done
    echo "]" >> ${file}
}

$@
