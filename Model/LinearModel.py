import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

import os
import numpy as np


def load_data(dataset_name: str) -> np.array:
    data_path = "NeuralNetworkFiles/" + dataset_name + "_data/"
    data_files = os.listdir(data_path)
    data_array = [np.array([])] * len(data_files)
    for i, file in enumerate(data_files):
        if i % 100 == 0:
            print(f"Loaded {i} out of {len(data_files)} from the {dataset_name} dataset")
        data = np.loadtxt(data_path + file, skiprows=1, delimiter=',')
        data_array[i] = data.flatten()
    return np.array(data_array)


def load_labels(dataset_name: str) -> np.array:
    labels_file = "NeuralNetworkFiles/" + dataset_name + "_truth.csv"
    labels = np.loadtxt(labels_file, delimiter=',')
    labels_array = [None] * len(labels)
    for i, label in enumerate(labels):
        labels_array[i] = label[2]
    return np.array(labels_array)


def load_indices(dataset_name: str) -> list:
    labels_file = "NeuralNetworkFiles/" + dataset_name + "_truth.csv"
    labels = np.loadtxt(labels_file, delimiter=',')
    indices = [0] * len(labels)
    for i, label in enumerate(labels):
        indices[i] = label[1]
    return indices


def train_parameters(dataset_name: str) -> np.array:
    data = load_data(dataset_name)
    data = np.append(np.ones((len(data), 1)), data, axis=1)

    labels = load_labels(dataset_name)
    left_side = np.matmul(data.T, data)
    right_side = np.matmul(data.T, labels)
    return np.linalg.solve(left_side, right_side)


def make_prediction(dataset_name: str, export: bool = False, samples: int = float("inf")) -> np.array:
    data = load_data(dataset_name)
    data = np.append(np.ones((len(data), 1)), data, axis=1)
    indices = load_indices(dataset_name)
    labels = load_labels(dataset_name)

    amount = min(samples, len(labels))
    prediction = np.matmul(data[0:amount], Q)

    mistakes = 0
    zeroes = 0
    for i, label in enumerate(prediction):
        print(f"index: {indices[i]} - expected: {labels[i]} ; got: {prediction[i]:.8f} ~ {round(prediction[i])}")
        if round(prediction[i]) == 0:
            zeroes += 1
        if labels[i] != round(prediction[i]):
            mistakes += 1
    print(f"Got {mistakes} mistakes out of {amount} samples!")
    print(f"There were total of {zeroes} zeroes in {amount} samples!")
    if export:
        export_prediction(dataset_name, indices, prediction)

    return prediction


def export_prediction(dataset_name: str, indices: list, prediction: np.array) -> None:
    prediction_file = "NeuralNetworkFiles/" + dataset_name + "_linear_prediction.csv"
    with open(prediction_file, 'w') as file:
        for i, point_index in enumerate(indices):
            p = round(prediction[i])
            if p < 0:
                p = 0
            if p > 1:
                p = 1
            print(f"{int(point_index)},{int(p)}", file=file)


TILE_SIZE = 15
FEATURE_COUNT = 3
INPUT_COUNT = ((TILE_SIZE * TILE_SIZE) - 1) * FEATURE_COUNT

Q = train_parameters("train")
make_prediction("test", export=True)




