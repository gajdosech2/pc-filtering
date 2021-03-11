import os
#os.environ['KERAS_BACKEND'] = 'plaidml.keras.backend'

from keras.metrics import *
from keras import backend as K
from keras.optimizers import Adam
from keras.models import Model, load_model
from keras.layers import Input, Reshape, UpSampling2D
from keras.layers.core import Lambda
from keras.layers.convolutional import Conv2D, Conv2DTranspose
from keras.layers.pooling import MaxPooling2D
from keras.layers.merge import concatenate
from keras.layers import LeakyReLU

from losses import scaled_binary_crossentropy, weighted_binary_crossentropy, binary_focal_loss

import tensorflow as tf

def skippy(i):
    c1 = Conv2D(16, 3, activation=LeakyReLU(), padding='same') (i)
    c1 = Conv2D(16, 3, activation=LeakyReLU(), padding='same') (c1)
    p1 = MaxPooling2D(2) (c1)

    c2 = Conv2D(24, 3, activation=LeakyReLU(), padding='same') (p1)
    c2 = Conv2D(24, 3, activation=LeakyReLU(), padding='same') (c2)
    p2 = MaxPooling2D(2) (c2)
    
    c3 = Conv2D(48, 5, activation=LeakyReLU(), padding='same') (p2)
    c3 = Conv2D(48, 5, activation=LeakyReLU(), padding='same') (c3)   
    
    u4 = Conv2DTranspose(24, 2, strides=(2, 2), padding='same') (c3)
    u4 = concatenate([u4, c2])
    c4 = Conv2D(24, 3, activation=LeakyReLU(), padding='same') (u4)
    c4 = Conv2D(24, 3, activation=LeakyReLU(), padding='same') (c4)
    
    u5 = Conv2DTranspose(16, 2, strides=(2, 2), padding='same') (c4)
    u5 = concatenate([u5, c1])
    c5 = Conv2D(16, 3, activation=LeakyReLU(), padding='same') (u5)
    c5 = Conv2D(16, 3, activation=LeakyReLU(), padding='same') (c5)
    
    o = Conv2D(1, 1, activation='sigmoid') (c5)
    return o
    

def skip(i):
    c1 = Conv2D(8, 3, activation=LeakyReLU(), padding='same') (i)
    c1 = Conv2D(8, 3, activation=LeakyReLU(), padding='same') (c1)
    p1 = MaxPooling2D(2) (c1)

    c2 = Conv2D(16, 3, activation=LeakyReLU(), padding='same') (p1)
    c2 = Conv2D(16, 3, activation=LeakyReLU(), padding='same') (c2)
    p2 = MaxPooling2D(2) (c2)

    c3 = Conv2D(32, 3, activation=LeakyReLU(), padding='same') (p2)
    c3 = Conv2D(32, 3, activation=LeakyReLU(), padding='same') (c3)
    p3 = MaxPooling2D(2) (c3)

    c4 = Conv2D(48, 3, activation=LeakyReLU(), padding='same') (p3)
    c4 = Conv2D(48, 3, activation=LeakyReLU(), padding='same') (c4)
    p4 = MaxPooling2D(2) (c4)

    c5 = Conv2D(64, 3, activation=LeakyReLU(), padding='same') (p4)
    c5 = Conv2D(64, 3, activation=LeakyReLU(), padding='same') (c5)

    u6 = Conv2DTranspose(48, 2, strides=(2, 2), padding='same') (c5)
    u6 = concatenate([u6, c4])
    c6 = Conv2D(48, 3, activation=LeakyReLU(), padding='same') (u6)
    c6 = Conv2D(48, 3, activation=LeakyReLU(), padding='same') (c6)

    u7 = Conv2DTranspose(32, 2, strides=(2, 2), padding='same') (c6)
    u7 = concatenate([u7, c3])
    c7 = Conv2D(32, 3, activation=LeakyReLU(), padding='same') (u7)
    c7 = Conv2D(32, 3, activation=LeakyReLU(), padding='same') (c7)

    u8 = Conv2DTranspose(16, 2, strides=(2, 2), padding='same') (c7)
    u8 = concatenate([u8, c2])
    c8 = Conv2D(16, 3, activation=LeakyReLU(), padding='same') (u8)
    c8 = Conv2D(16, 3, activation=LeakyReLU(), padding='same') (c8)

    u9 = Conv2DTranspose(8, 2, strides=(2, 2), padding='same') (c8)
    u9 = concatenate([u9, c1])
    c9 = Conv2D(8, 3, activation=LeakyReLU(), padding='same') (u9)
    c9 = Conv2D(8, 3, activation=LeakyReLU(), padding='same') (c9)

    o = Conv2D(1, 1, activation='sigmoid') (c9)
    return o

def autoencoder(i):
    x = Conv2D(filters=16, kernel_size=3, padding='same', activation=LeakyReLU())(i)
    x = MaxPooling2D(pool_size=2, strides=(2, 2))(x)
    
    x = Conv2D(filters=32, kernel_size=3, padding='same', activation=LeakyReLU())(x)
    x = MaxPooling2D(pool_size=2, strides=(2, 2))(x)
    
    x = Conv2D(filters=64, kernel_size=5, padding='same', activation=LeakyReLU())(x)
    
    x = UpSampling2D(size=2)(x)
    x = Conv2D(filters=32, kernel_size=3, padding='same', activation=LeakyReLU())(x)
    
    x = UpSampling2D(size=2)(x)
    x = Conv2D(filters=16, kernel_size=3, padding='same', activation=LeakyReLU())(x)
    
    o = Conv2D(filters=1, kernel_size=1, activation='sigmoid')(x) 
    return o

def simple(i):
    x = Conv2D(filters=8, kernel_size=3, padding='same', activation=LeakyReLU())(i)
    x = Conv2D(filters=16, kernel_size=5, padding='same', activation=LeakyReLU())(x)
    x = Conv2D(filters=32, kernel_size=7, padding='same', activation=LeakyReLU())(x)
    x = Conv2D(filters=64, kernel_size=3, padding='same', activation=LeakyReLU())(x)
    o = Conv2D(filters=1, kernel_size=1, activation='sigmoid')(x)
    return o
      

def generate_model(channels=5):
    i = Input(shape=(None, None, channels))
    o = skip(i)
   
    model = Model(inputs=i, outputs=o)
    
    print(model.summary())
    print('Total number of layers: {}'.format(len(model.layers)))

    model.compile(optimizer=Adam(lr=1e-6), # optimizer='rmsprop' optimizer=Adam(lr=1e-6)
                  loss=binary_focal_loss(alpha=0.06, gamma=5), #loss=binary_focal_loss(alpha=.1, gamma=5)
                  metrics=[#'accuracy',
                  Precision(),
                  Recall(),
                  SpecificityAtSensitivity(0.5, num_thresholds=1, name='specificity'), 
                  SensitivityAtSpecificity(0.5, num_thresholds=1, name='sensitivity')
                  #TrueNegatives(), TruePositives(), FalseNegatives(), FalsePositives()
                  ])

    return model


if __name__ == '__main__':
    generate_model()

