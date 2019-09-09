import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

import os
import numpy as np
import tensorflow as tf
from tensorflow import keras

tile_size = 11
feature_count = 5
input_count = tile_size * tile_size * feature_count


def load_train_data(dataset_name: str) -> np.array:
    """
    Load data files for the given dataset.

    :param dataset_name: Name of the data set.
    :return: np.array of data files in (input_count, ) shape
    """
    data_path = "NeuralNetworkFiles/" + dataset_name + "_data/"
    data_files = os.listdir(data_path)
    data_array = [None] * len(data_files)
    for i, file in enumerate(data_files):
        data = np.loadtxt(data_path + file, skiprows=1, delimiter=',')
        data_array[i] = data.flatten()
    return np.array(data_array)


def load_train_labels(dataset_name: str) -> np.array:
    """
    Load data labels for the given dataset.

    :param dataset_name: Name of the data set.
    :return: np.array of the data labels
    """
    labels_file = "NeuralNetworkFiles/" + dataset_name + "_truth.csv"
    labels = np.loadtxt(labels_file, delimiter=',')
    labels_array = [None] * len(labels)
    for i, label in enumerate(labels):
        labels_array[i] = label[1]
    return np.array(labels_array)


def build_model() -> keras.Sequential:
    """
    Builds the neural network model.

    :return: Neural network model as the keras.Sequential object.
    """
    network_model = keras.Sequential([
        keras.layers.Dense(512, activation=tf.keras.activations.sigmoid, input_shape=(input_count,)),
        keras.layers.Dense(256, activation=tf.keras.activations.relu),
        keras.layers.Dense(1, activation=tf.keras.activations.linear)
    ])
    network_model.compile(optimizer='adam',
                          loss='mean_squared_error',
                          metrics=['accuracy'])
    return network_model


dataset = "small"
train_data = load_train_data(dataset)
train_labels = load_train_labels(dataset)
model = build_model()
model.fit(train_data, train_labels, epochs=20)

prediction = model.predict(train_data)
print(prediction)
