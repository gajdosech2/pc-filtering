import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

import numpy as np
import tensorflow as tf
from tensorflow import keras

tile_size = 11
feature_count = 5
input_count = tile_size * tile_size * feature_count

def load_train_files(folder_name):
    train_data = np.array()
    input_file = "464.csv"
    input_data = np.loadtxt(input_file, skiprows=1, delimiter=',')
    input_data = input_data.flatten()
    return 1


model = keras.Sequential([
    keras.layers.Dense(128, activation=tf.nn.relu, input_shape=(input_count,)),
    keras.layers.Dense(1, activation=tf.keras.activations.linear)
])

model.compile(optimizer='adam',
              loss='mean_squared_error',
              metrics=['accuracy'])


#train_data = load_train_files("apple_01_data")
#prediction = model.predict(train_data)

#print(prediction)
