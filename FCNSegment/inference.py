import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
import imageio
import time
import os
import math

from model import generate_model

d = 'process/'
e = 'result/'
POWER_UP = True
                          
def input_files():
    files = os.listdir(d)
    for f in files:
        if '.cogs' in f:
            if os.name == 'nt':
                os.system('"utils\WCC.exe"' + 
                            ' --segment ' + 
                            d + f + ' ' + 
                            d)
            else:
                pass
                
def clean_up():
    files = os.listdir(d)
    for f in files:
        if '.png' in f:
            os.remove(d + f)

            
def generate_feature_image(f):
    intensity_image = imageio.imread(d + f + "_depthmap.png") / 255
    normals_image = imageio.imread(d + f + "_normalmap.png") / 255
    feature_image = np.dstack((intensity_image, normals_image[:, :, 0], normals_image[:, :, 1], normals_image[:, :, 2]))
  
    if POWER_UP:
        shape = feature_image.shape
        i1 = 2**math.ceil(math.log2(shape[0])) - shape[0]
        i2 = 2**math.ceil(math.log2(shape[1])) - shape[1]
        shape = (shape[0]+i1, shape[1]+i2, shape[2])
        resized = np.zeros(shape, dtype='float32')    
        resized[:feature_image.shape[0], :feature_image.shape[1], :] = feature_image
        feature_image = resized
    
    feature_image = np.expand_dims(feature_image, axis=0)
    return feature_image, intensity_image.shape[0], intensity_image.shape[1]
           
            
def inference():
    model = generate_model()
    model.load_weights("weights.keras")
    
    if not os.path.exists(e):
        os.makedirs(e, exist_ok=True)

    files = os.listdir(d)    
    for i, f in enumerate(files):
        if "depthmap" in f: 
            f = '_'.join(f.split('_')[:2])
            print("processing: " + f)
            feature_image, original_width, original_height = generate_feature_image(f)
            
            prediction = model.predict(feature_image)
           
            prediction = np.round(prediction[0])
            prediction = prediction[:original_width, :original_height, :]

            imageio.imwrite(e + f + "_prediction.png", prediction)


if __name__ == "__main__":
    start = time.time()
    input_files()
    inference()
    #clean_up()
    print(f"Elapsed time: {time.time() - start} seconds")


