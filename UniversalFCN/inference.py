#import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np
import imageio
import sys
import time

from model import generate_model

#def enable_gpu():
#    gpus = tf.config.experimental.list_physical_devices('GPU')
#    if gpus:
#        for gpu in gpus:
#            tf.config.experimental.set_memory_growth(gpu, True)


def show(result):
    plt.figure()
    plt.axis('off')
    plt.imshow(result[:, :, 0]).set_cmap('gray')
    plt.show()


if __name__ == "__main__":
    #enable_gpu()
    model = generate_model()
    model.load_weights("weights.keras")

    d = 'data/inference/'

    f = sys.argv[1]

    intensity_image = imageio.imread(d + f + "_intensitymap.png") / 255
    normals_image = imageio.imread(d + f + "_normalmap.png") / 255
    feature_image = np.dstack((intensity_image[:, :, 0],
                               normals_image[:, :, 0], normals_image[:, :, 1], normals_image[:, :, 2]))

    feature_image = np.expand_dims(feature_image, axis=0)
    print(feature_image.shape)

    start = time.time()
    predictions = model.predict(feature_image)
    print(f"Elapsed time: {time.time() - start} seconds")

    print(predictions.shape)
    show(np.round(predictions[0]))



