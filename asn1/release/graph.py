
import numpy as np
import ast
import glob, os
import matplotlib.pyplot as plt

size = [2000000,3000000,4000000,5000000,6000000,7000000,8000000,9000000,10000000]
psrs = []
seq = []

with open("seq.txt") as f:
    for idx, i in enumerate(ast.literal_eval(f.read())):
        a = np.matrix(i)
        b=int(np.average(a))
        print(size[idx], a.std(axis=1).tolist()[0][0],b)
        seq += [a.std(axis=1).tolist()[0][0],b]



for file in glob.glob("*.psrs"):
    print(file)
    with open(file) as f:
        for idx, i in enumerate(ast.literal_eval(f.read())):
            a = np.matrix(i)
            avg=[int(x) for x in np.average(a,axis=0).tolist()[0]]
            std=[int(x) for x in a.std(axis=0).tolist()[0]]
            print(size[idx], avg,std)
            psrs += [avg, std]
# plt.plot(seq, b, 'ro')
# plt.show()

