import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

import os
import numpy as np
import tensorflow as tf
from tensorflow import keras

tile_size = 11
feature_count = 5
input_count = tile_size * tile_size * feature_count


def load_train_files(folder_name: str) -> np.array:
    """
    Load data files from the given folder.

    :param folder_name: Name of the folder followed by '/' containing the data files
    :return: np.array of data files in (input_count, ) shape
    """
    files_folder = "NeuralNetworkFiles/"
    data_files = os.listdir(files_folder + folder_name)
    data_array = []
    for file in data_files:
        path = files_folder + folder_name + file
        data = np.loadtxt(path, skiprows=1, delimiter=',')
        data_array.append(data.flatten())
    return np.array(data_array)


def build_model() -> keras.Sequential:
    """
    Builds the neural network model.

    :return: Neural network model as keras.Sequential object.
    """
    network_model = keras.Sequential([
        keras.layers.Dense(128, activation=tf.nn.relu, input_shape=(input_count,)),
        keras.layers.Dense(1, activation=tf.keras.activations.linear)
    ])
    network_model.compile(optimizer='adam',
                          loss='mean_squared_error',
                          metrics=['accuracy'])
    return network_model


model = build_model()
train_data = load_train_files("apple_01_data/")
prediction = model.predict(train_data)
print(prediction)
