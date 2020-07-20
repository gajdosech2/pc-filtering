import os

os.environ["KERAS_BACKEND"] = "plaidml.keras.backend"

import keras


def generate_model():
    i = keras.layers.Input(shape=(None, None, 4))

    x = keras.layers.Conv2D(filters=32, kernel_size=3, activation='relu')(i)
    x = keras.layers.Conv2D(filters=64, kernel_size=3, activation='relu')(x)
    x = keras.layers.Conv2D(filters=128, kernel_size=3, activation='relu')(x)
    x = keras.layers.Conv2DTranspose(filters=128, kernel_size=3)(x)
    x = keras.layers.Conv2DTranspose(filters=64, kernel_size=3)(x)
    x = keras.layers.Conv2DTranspose(filters=32, kernel_size=3)(x)

    o = keras.layers.Conv2D(filters=1, kernel_size=1, activation='sigmoid')(x)

    model = keras.Model(inputs=i, outputs=o)
    
    print(model.summary())
    print(f'Total number of layers: {len(model.layers)}')

    model.compile(optimizer=keras.optimizers.Adam(lr=0.0001),
                  loss='binary_crossentropy',
                  metrics=['accuracy'])

    return model


if __name__ == "__main__":
    generate_model()

