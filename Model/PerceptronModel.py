import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

import os
import numpy as np
import tensorflow as tf
from tensorflow import keras


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


def build_model() -> keras.Sequential:
    network_model = keras.Sequential([
        keras.layers.Dense(1, activation=tf.keras.activations.linear, input_shape=(INPUT_COUNT,))
    ])
    network_model.compile(optimizer='adam',
                          loss='mean_squared_error',
                          metrics=['accuracy'])
    return network_model


def train_model(dataset_name: str) -> None:
    train_data = load_data(dataset_name)
    train_labels = load_labels(dataset_name)
    model.fit(train_data, train_labels, epochs=64)


def make_prediction(dataset_name: str, export: bool = False, samples: int = float("inf")) -> np.array:
    indices = load_indices(dataset_name)
    data = load_data(dataset_name)
    labels = load_labels(dataset_name)

    amount = min(samples, len(data))
    prediction = model.predict(data[0:amount])
    mistakes = 0
    zeroes = 0
    for i, label in enumerate(prediction):
        print(f"index: {indices[i]} - expected: {labels[i]} ; got: {prediction[i][0]:.8f} ~ {round(prediction[i][0])}")
        if round(prediction[i][0]) == 0:
            zeroes += 1
        if labels[i] != round(prediction[i][0]):
            mistakes += 1
    print(f"Got {mistakes} mistakes out of {amount} samples!")
    print(f"There were total of {zeroes} zeroes in {amount} samples!")
    if export:
        export_prediction(dataset_name, indices, prediction)
    return prediction


def export_prediction(dataset_name: str, indices: list, prediction: np.array) -> None:
    prediction_file = "NeuralNetworkFiles/" + dataset_name + "_neuron_prediction.csv"
    with open(prediction_file, 'w') as file:
        for i, point_index in enumerate(indices):
            p = round(prediction[i][0])
            if p < 0:
                p = 0
            if p > 1:
                p = 1
            print(f"{int(point_index)},{int(p)}", file=file)


TILE_SIZE = 15
FEATURE_COUNT = 3
INPUT_COUNT = ((TILE_SIZE * TILE_SIZE) - 1) * FEATURE_COUNT

model = build_model()
train_model("synthetic_01")
make_prediction("synthetic_03", export=False)
