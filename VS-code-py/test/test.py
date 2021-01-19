import os
import random
import json
import sys
import numpy as np

def random_int_list(start, stop, length,C):
    start, stop = (int(start), int(stop)) if start <= stop else (int(stop), int(start))
    length = int(abs(length)) if length else 0
    random_list = []
    for i in range(length):
        random_num = random.uniform(start,stop)
        random_num = round(random_num,C)  #随机数的精度round(数值，精度)
        random_list.append(random_num)
    return random_list

data_path = "D://xxw_files//VSCODE//VS-code-py//data.txt"

with open(data_path, 'w') as f: 
    f.seek(0)
    f.truncate() 

data_list_1 = random_int_list(0,1,50,10)
data_list_2 = random_int_list(0,1,50,10)
data_list_3 = random_int_list(0,1,50,10)
data_list_4 = random_int_list(0,1,50,10)

e_list_1 = random_int_list(-2,2,50,1)
e_list_2 = random_int_list(-2,2,50,1)
e_list_3 = random_int_list(-2,2,50,1)
e_list_4 = random_int_list(-2,2,50,1)

with open(data_path, 'a') as f:
    for i in range(50):
        data_str_1 = '%.9f' %data_list_1[i] + 'D' + '%d '%e_list_1[i] 
        data_str_2 = '%.9f' %data_list_2[i] + 'D' + '%d '%e_list_2[i]
        data_str_3 = '%.9f' %data_list_3[i] + 'D' + '%d '%e_list_3[i]
        data_str_4 = '%.9f' %data_list_4[i] + 'D' + '%d '%e_list_4[i]
        data_str = data_str_1 + data_str_2 + data_str_3 + data_str_4 +'\n'
        f.write(data_str) 