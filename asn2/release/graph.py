
import numpy as np
import ast
import glob, os
import matplotlib.pyplot as plt

size =  [x*1000000 for x in range(8,13)]
psrs = []
processors = [1,2,3,4]
psrserr= []
seq = []
totalsPSRS = []
speedup = []
#===========================================
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
#===========================================
for file in sorted(glob.glob("*.mpipsrs")):
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
speedup = (seq/np.matrix(totalsPSRS)).transpose()
for idx, i in enumerate(speedup):
    plt.plot (processors, i.tolist()[0], label=size[idx])
plt.xlabel('Processes per node')
plt.ylabel('Speedup')
plt.title('Speedup by elements and processes per node')
plt.legend([str(i)+' elements'for i in size]).draggable()
plt.savefig("figure2.png")
#===========================================
plt.figure()
for idx, i in enumerate(totalsPSRS):
    plt.plot (size, i, label=size[idx])
plt.xlabel('Elements')
plt.ylabel('Time (Seconds)')
plt.title('Execution time for N elements and K processes per node')
plt.legend([str(i)+' per node'for i in processors]).draggable()
plt.savefig("figure4.png")
#===========================================
plt.figure()
bar_width = 0.35
phases = ["phase 1","phase 2","phase 3","phase 4",]
tmp = psrs[2][3][:-1]
plt.bar(np.arange(4)-bar_width,tmp, bar_width, align='center', label="3 Threads", color='r', yerr=psrserr[2][3][:-1])
tmp = psrs[3][3][:-1]
plt.bar(np.arange(4),tmp, bar_width, align='center', label="4 Threads", color='b', yerr=psrserr[3][3][:-1])

plt.legend().draggable()
plt.xticks(range(4), phases)
plt.xlabel('phases')
plt.ylabel('time spent')
plt.title('Time spent in seconds on each phase for 12 million elements')
plt.savefig("figure3.png")
plt.show()
