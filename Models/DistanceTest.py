import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

import os
import numpy as np
import math


def sigmoid(x):
    return 1 / (1 + math.exp(-x))


def load_data(dataset_name: str) -> np.array:
    data_path = "NeuralNetworkFiles/" + dataset_name + "_data/"
    data_files = os.listdir(data_path)
    data_array = [np.array([])] * len(data_files)
    for i, file in enumerate(data_files):
        data = np.loadtxt(data_path + file, skiprows=1, delimiter=',')
        data_array[i] = data.flatten()
    return np.array(data_array)


def make_sum(data_array: np.array) -> list:
    res = [0] * len(data_array)
    count = 0
    for i, data in enumerate(data_array):
        res[i] = np.sum(data)
        if res[i] > 2:
            count += 1
    print(count)
    return res


def matrix_sum(data_array: np.array) -> list:
    Q = np.array([1] * 224)
    s = list(np.matmul(data_array, Q))
    return list(s / np.max(s))


train_data = load_data("synthetic_01")
Q = np.array([1] * 225)
print(make_sum(train_data))
print(matrix_sum(train_data))
