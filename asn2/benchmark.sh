#!/usr/bin/env bash
seq=$(seq $1 $2 $3)

numProcessors=$(seq 4)
echo ${seq}

for threads in $numProcessors; do
    file="release/${threads}.mpipsrs"
    rm ${file}
    echo "["  >> ${file}
    for elements in ${seq}; do
        printf "["  >> ${file}
        for i in $(seq 5); do
            echo "[$(mpirun -hostfile hostfile -npernode ${threads} release/asn2 -size ${elements})]," >> ${file}
        done
        printf "],"  >> ${file}
    done
    echo "]"  >> ${file}
done

file="release/seq.txt"
rm seq.txt
printf "[" >> ${file}
for elements in ${seq}; do
    printf "[" >> ${file}
    for i in $(seq 5); do
      echo "$(./release/sequential -size ${elements})," >> ${file}
    done
    echo "]," >> ${file}
done
echo "]" >> ${file}
