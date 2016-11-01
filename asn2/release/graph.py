
import numpy as np
import ast
import glob, os
import matplotlib.pyplot as plt

size = [8000000,9000000,10000000,11000000,12000000]
processors = [1,2,3,4,5,6]
psrs = []
psrserr= []
seq = []
totalsPSRS = []
speedup = []
with open("seq.txt") as f:
    for idx, i in enumerate(ast.literal_eval(f.read())):
        a = np.matrix(i)
        b=int(np.average(a))/1000000
        seq.append(b)
print(seq)

plt.figure()
plt.plot (size, seq )

plt.xlabel('Elements')
plt.ylabel('Runtime (seconds)')
plt.title('Runtime of sequential merge sort')
plt.savefig("figure1.png")

for file in sorted(glob.glob("*.psrs")):
    with open(file) as f:
        totals = []
        phases = []
        phaseserr= []
        for idx, i in enumerate(ast.literal_eval(f.read())):
            a = np.matrix(i)
            avg=[int(x)/1000000 for x in np.average(a,axis=0).tolist()[0]]
            std=[int(x)/1000000 for x in a.std(axis=0).tolist()[0]]
            totals.append(avg[-1])
            phases.append(avg)
            phaseserr.append(std)
        psrs.append(phases)
        psrserr.append(phaseserr)
        totalsPSRS.append(totals)

plt.figure()
err = (np.matrix(phaseserr).transpose())
print(err)
speedup = (seq/np.matrix(totalsPSRS)).transpose()
# err = np.matrix(psrserr).transpose()
for idx, i in enumerate(speedup):
    plt.errorbar (processors, i.tolist()[0], yerr=np.ones(6), label=size[idx] )

plt.xlabel('Processors')
plt.ylabel('Speedup')
plt.title('Speedup by elements and # threads')
plt.legend([str(size[i])+' elements'for i in range(0,len(size))]).draggable()
plt.savefig("figure2.png")

plt.figure()
bar_width = 0.35
phases = ["phase 1","phase 2","phase 3","phase 4",]
tmp = psrs[2][3][:-1]
plt.bar(np.arange(4)-bar_width,tmp, bar_width, align='center', label="3 Threads", color='r', yerr=psrserr[2][3][:-1])
tmp = psrs[3][3][:-1]
plt.bar(np.arange(4),tmp, bar_width, align='center', label="4 Threads", color='b', yerr=psrserr[3][3][:-1])
tmp = psrs[4][3][:-1]
plt.bar(np.arange(4)+bar_width,tmp, bar_width, align='center', label="5 Threads", color='g', yerr=psrserr[4][3][:-1])
plt.legend().draggable()
plt.xticks(range(4), phases)
plt.xlabel('phases')
plt.ylabel('time spent')
plt.title('Time spent in seconds on each phase for 11 million elements')
plt.savefig("figure3.png")
