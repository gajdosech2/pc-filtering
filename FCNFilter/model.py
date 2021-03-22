from keras.models import Model
from keras.layers import Input, UpSampling2D, LeakyReLU
from keras.layers.convolutional import Conv2D, Conv2DTranspose
from keras.layers.pooling import MaxPooling2D
from keras.layers.merge import concatenate
from keras import layers
from keras.utils import plot_model
from packaging import version
import tensorflow as tf


def setup_gpu():
    if (version.parse(tf.__version__) >= version.parse('2.0')):   
        gpus = tf.config.list_physical_devices('GPU')
        if gpus:
            try:
                for gpu in gpus:
                    tf.config.experimental.set_memory_growth(gpu, True)
            except RuntimeError as e:
                print(e)


def xception(i):
    # [First half of the network: downsampling inputs]

    x = layers.Conv2D(8, 3, strides=2, padding="same")(i)
    x = layers.BatchNormalization()(x)
    x = layers.Activation("relu")(x)

    previous_block_activation = x  # Set aside residual

    for filters in [16, 32, 64]:
        x = layers.Activation("relu")(x)
        x = layers.SeparableConv2D(filters, 3, padding="same")(x)
        x = layers.BatchNormalization()(x)

        x = layers.Activation("relu")(x)
        x = layers.SeparableConv2D(filters, 3, padding="same")(x)
        x = layers.BatchNormalization()(x)

        x = layers.MaxPooling2D(3, strides=2, padding="same")(x)

        # Project residual
        residual = layers.Conv2D(filters, 1, strides=2, padding="same")(
            previous_block_activation
        )
        x = layers.add([x, residual])  # Add back residual
        previous_block_activation = x  # Set aside next residual

    # [Second half of the network: upsampling inputs]
    
    for filters in [96, 64, 32, 16]:
        x = layers.Activation("relu")(x)
        x = layers.Conv2DTranspose(filters, 3, padding="same")(x)
        x = layers.BatchNormalization()(x)

        x = layers.Activation("relu")(x)
        x = layers.Conv2DTranspose(filters, 3, padding="same")(x)
        x = layers.BatchNormalization()(x)

        x = layers.UpSampling2D(2)(x)

        # Project residual
        residual = layers.UpSampling2D(2)(previous_block_activation)
        residual = layers.Conv2D(filters, 1, padding="same")(residual)
        x = layers.add([x, residual])  # Add back residual
        previous_block_activation = x  # Set aside next residual

    o = Conv2D(1, 1, activation='sigmoid') (x)
    return o
    

def skippy(i):
    c1 = Conv2D(8, 3, activation=LeakyReLU(), padding='same') (i)
    c1 = Conv2D(8, 3, activation=LeakyReLU(), padding='same') (c1)
    p1 = MaxPooling2D(2) (c1)

    c2 = Conv2D(16, 3, activation=LeakyReLU(), padding='same') (p1)
    c2 = Conv2D(16, 3, activation=LeakyReLU(), padding='same') (c2)
    p2 = MaxPooling2D(2) (c2)
    
    c3 = Conv2D(64, 5, activation=LeakyReLU(), padding='same') (p2)
    c3 = Conv2D(64, 5, activation=LeakyReLU(), padding='same') (c3)   
    
    u4 = Conv2DTranspose(16, 2, strides=(2, 2), padding='same') (c3)
    u4 = concatenate([u4, c2])
    c4 = Conv2D(16, 3, activation=LeakyReLU(), padding='same') (u4)
    c4 = Conv2D(16, 3, activation=LeakyReLU(), padding='same') (c4)
    
    u5 = Conv2DTranspose(8, 2, strides=(2, 2), padding='same') (c4)
    u5 = concatenate([u5, c1])
    c5 = Conv2D(8, 3, activation=LeakyReLU(), padding='same') (u5)
    c5 = Conv2D(8, 3, activation=LeakyReLU(), padding='same') (c5)
    
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

    return model


if __name__ == '__main__':
    model = generate_model()
    try:
        plot_model(model, show_shapes=True)
    except:
        print("Install pydot and graphviz to get the diagram of the network!")

