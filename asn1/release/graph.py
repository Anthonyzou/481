
import numpy as np
import ast
import glob, os

seq = [200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000]

for file in glob.glob("*.psrs"):
    print(file)
    with open(file) as f:
        for i in ast.literal_eval(f.read()):
            a = np.matrix(i)
            b=a.sum(axis=0)
            c=[int(x) for x in a.std(axis=0).tolist()[0]]
            print(c)
            print(b//5)

with open("seq.txt") as f:
    for i in ast.literal_eval(f.read()):
        print(i)
        a = np.matrix(i)
        b=a.sum(axis=0)
        print( a.std(axis=1))
        print(b//5)