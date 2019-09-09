import numpy as np


def check(file):
    data = np.loadtxt("NeuralNetworkFiles/" + file, delimiter=',')
    if len(data) == 0 or len(data[0]) != 2:
        return 0, 0
    count_true = 0
    count_false = 0
    for row in data:
        if row[1] == 0:
            count_false += 1
        if row[1] == 1:
            count_true += 1
    return count_true, count_false


print(check("corn_03_truth.csv"))
