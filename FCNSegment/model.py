import os
#os.environ["KERAS_BACKEND"] = "plaidml.keras.backend"

import keras
from keras.models import Model, load_model
from keras.layers import Input, Reshape, UpSampling2D
from keras.layers.core import Lambda
from keras.layers.convolutional import Conv2D, Conv2DTranspose
from keras.layers.pooling import MaxPooling2D
from keras.layers.merge import concatenate

import tensorflow as tf

def skip(i):
    c1 = Conv2D(8, (3, 3), activation='relu', padding='same') (i)
    c1 = Conv2D(8, (3, 3), activation='relu', padding='same') (c1)
    p1 = MaxPooling2D((2, 2)) (c1)

    c2 = Conv2D(16, (3, 3), activation='relu', padding='same') (p1)
    c2 = Conv2D(16, (3, 3), activation='relu', padding='same') (c2)
    p2 = MaxPooling2D((2, 2)) (c2)

    c3 = Conv2D(32, (3, 3), activation='relu', padding='same') (p2)
    c3 = Conv2D(32, (3, 3), activation='relu', padding='same') (c3)
    p3 = MaxPooling2D((2, 2)) (c3)

    c4 = Conv2D(64, (3, 3), activation='relu', padding='same') (p3)
    c4 = Conv2D(64, (3, 3), activation='relu', padding='same') (c4)
    p4 = MaxPooling2D(pool_size=(2, 2)) (c4)

    c5 = Conv2D(128, (3, 3), activation='relu', padding='same') (p4)
    c5 = Conv2D(128, (3, 3), activation='relu', padding='same') (c5)

    u6 = Conv2DTranspose(64, (2, 2), strides=(2, 2), padding='same') (c5)
    u6 = concatenate([u6, c4])
    c6 = Conv2D(64, (3, 3), activation='relu', padding='same') (u6)
    c6 = Conv2D(64, (3, 3), activation='relu', padding='same') (c6)

    u7 = Conv2DTranspose(32, (2, 2), strides=(2, 2), padding='same') (c6)
    u7 = concatenate([u7, c3])
    c7 = Conv2D(32, (3, 3), activation='relu', padding='same') (u7)
    c7 = Conv2D(32, (3, 3), activation='relu', padding='same') (c7)

    u8 = Conv2DTranspose(16, (2, 2), strides=(2, 2), padding='same') (c7)
    u8 = concatenate([u8, c2])
    c8 = Conv2D(16, (3, 3), activation='relu', padding='same') (u8)
    c8 = Conv2D(16, (3, 3), activation='relu', padding='same') (c8)

    u9 = Conv2DTranspose(8, (2, 2), strides=(2, 2), padding='same') (c8)
    u9 = concatenate([u9, c1], axis=3)
    c9 = Conv2D(8, (3, 3), activation='relu', padding='same') (u9)
    c9 = Conv2D(8, (3, 3), activation='relu', padding='same') (c9)

    o = Conv2D(1, (1, 1), activation='sigmoid') (c9)
    #o = Reshape((-1,))(o)
    return o

def autoencoder(i):
    x = Conv2D(filters=8, kernel_size=3, padding = "same", activation='relu')(i)
    x = MaxPooling2D(pool_size=2, strides=(2,2))(x)
    
    x = Conv2D(filters=16, kernel_size=3, padding = "same", activation='relu')(x)
    x = MaxPooling2D(pool_size=2, strides=(2,2))(x)
    
    x = Conv2D(filters=32, kernel_size=3, padding = "same", activation='relu')(x)
    x = MaxPooling2D(pool_size=2, strides=(2,2))(x)

    x = UpSampling2D(size=2)(x)
    x = Conv2D(filters=32, kernel_size=3, padding = "same", activation='relu')(x)
    
    x = UpSampling2D(size=2)(x)
    x = Conv2D(filters=16, kernel_size=3, padding = "same", activation='relu')(x)
    
    x = UpSampling2D(size=2)(x)
    x = Conv2D(filters=8, kernel_size=3, padding = "same", activation='relu')(x)
    
    o = Conv2D(filters=1, kernel_size=1, activation='sigmoid')(x) 
    
    return o

def simple(i):
    x = Conv2D(filters=8, kernel_size=3, activation='relu')(i)
    x = Conv2D(filters=16, kernel_size=3, activation='relu')(x)
    x = Conv2D(filters=32, kernel_size=3, activation='relu')(x)
    
    x = Conv2DTranspose(filters=32, kernel_size=3)(x)
    x = Conv2DTranspose(filters=16, kernel_size=3)(x)
    x = Conv2DTranspose(filters=8, kernel_size=3)(x)
    o = Conv2D(filters=1, kernel_size=1, activation='sigmoid')(x)
    
    return o

def generate_model():
    i = Input(shape=(None, None, 4))
    o = skip(i)
   
    model = Model(inputs=i, outputs=o)
    
    print(model.summary())
    print(f'Total number of layers: {len(model.layers)}')

    model.compile(optimizer=keras.optimizers.Adam(lr=0.00005),
                  loss='binary_crossentropy',
                  metrics=['accuracy'])

    return model


if __name__ == "__main__":
    generate_model()

