
import numpy as np
import ast

with open('./2.psrs') as f:
    my_lines = ast.literal_eval(f.read())

    for i in (my_lines):
        a = np.matrix(i)
        # print(a)
        a=a.sum(axis=0)
        print(a//5)
