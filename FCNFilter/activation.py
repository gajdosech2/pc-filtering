import numpy as np
import keras
import cv2
import os

from model import generate_model, setup_gpu
from inference import generate_feature_image

PROCESS_PATH = 'process/'
RESULT_PATH = 'result/'
POWER_UP = True
WEIGHTS_FILE = 'weights.h5'


def activation_maps():
    model = generate_model()
    model.load_weights(WEIGHTS_FILE)
    model = keras.Model(inputs=model.inputs, outputs=model.layers[1].output)

    files = os.listdir(PROCESS_PATH)
    for i, f in enumerate(files):
        if 'intensitymap' in f: 
            f = '_'.join(f.split('_')[:2])
            print('processing: ' + f)
            feature_image, original_width, original_height = generate_feature_image(f)
            
            prediction = model.predict(feature_image)
            prediction = np.round(prediction[0])
            prediction = prediction[:original_width, :original_height, :]

            for j in range(prediction.shape[2]):
                cv2.imwrite(PROCESS_PATH + f + '_activation' + str(j) + '.png', prediction[:, :, j].astype(np.uint8) * 255)


if __name__ == "__main__":
    setup_gpu()
    activation_maps()


