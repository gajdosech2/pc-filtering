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
ALPHA = 1.0
WEIGHTS_FILE = 'weights.h5'


def show(result):
    plt.figure()
    plt.axis('off')
    plt.imshow(result[:, :, 0]).set_cmap('gray')
    plt.show()
    
    
def setup_gpu():
    gpus = tf.config.list_physical_devices('GPU')
    if gpus:
        try:
            for gpu in gpus:
                tf.config.experimental.set_memory_growth(gpu, True)
        except RuntimeError as e:
            print(e)   
            
            
def cogs_files():
    files = os.listdir(d)
    os.makedirs(e, exist_ok=True)
    for f in files:
        if '.cogs' in f and 'processed' not in f and 'truth' not in f:
            if os.name == 'nt':
                os.system('"utils\WCC.exe"' + 
                            ' --process ' + 
                            d + f + ' ' + 
                            d + f[:-5] + '_prediction.png' + ' ' + 
                            e)
            else:
                pass
                
                
def input_files():
    files = os.listdir(d)
    for f in files:
        if '.cogs' in f and 'processed' not in f and 'truth' not in f:
            print("processing: " + f)
            t = ""
            if os.path.isfile(d + 'truth_' + f):
                t = d + 'truth_' + f
            if os.name == 'nt':
                os.system('"utils\WCC.exe"' + 
                            ' --generate ' + 
                            d + f + ' ' + 
                            d + ' ' +
                            t)
            else:
                pass
                
def clean_up():
    files = os.listdir(d)
    for f in files:
        if '.png' in f:
            os.remove(d + f)

            
def generate_feature_image(f):
    intensity_image = imageio.imread(d + f + '_intensitymap.png') / 255
    normals_image = imageio.imread(d + f + '_normalmap.png') / 255
    depth_image = imageio.imread(d + f + '_depthmap.png') / 255
    feature_image = np.dstack((intensity_image, depth_image, normals_image[:, :, 0], normals_image[:, :, 1], normals_image[:, :, 2]))
  
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
    model.load_weights(WEIGHTS_FILE)

    files = os.listdir(d)    
    for i, f in enumerate(files):
        if 'intensitymap' in f: 
            f = '_'.join(f.split('_')[:2])
            print('processing: ' + f)
            feature_image, original_width, original_height = generate_feature_image(f)
            
            prediction = model.predict(feature_image)
           
            prediction = np.round(prediction[0] * ALPHA)
            prediction = prediction[:original_width, :original_height, :]

            imageio.imwrite(d + f + '_prediction.png', prediction.astype(np.uint8) * 255)
            
            
def evaluation():
    size = 0
    metric = 0
    max_metric = 0
    min_metric = 1
    files = os.listdir(d)
    for truth in files:
        if 'truthmask' in truth:
            size += 1
            name = truth[:-14]
            prediction = name + '_prediction.png'
            prediction = np.asarray(imageio.imread(d + prediction) / 255, dtype=np.uint8)
            truth = np.asarray(imageio.imread(d + truth) / 255, dtype=np.uint8)
            
            intersection = np.sum(prediction * truth)
            union = np.sum(truth) + np.sum(prediction) - intersection
            iou = intersection / union
            metric += iou
            max_metric = max(iou, max_metric)
            min_metric = min(iou, min_metric)
            with open (e + name + '_eval.txt', 'w') as result:
                print(iou, file=result)
    
    if size > 0:
        metric /= size
        print('Average IoU: ' + str(metric))
        print('Max IoU: ' + str(max_metric))
        print('Min IoU: ' + str(min_metric))
        with open (e + 'average_iou.txt', 'w') as result:
            print(metric, file=result)


if __name__ == '__main__':
    setup_gpu()
    if (not any('.png' in f for f in os.listdir(d))):
        input_files()
    start = time.time()
    inference()
    cogs_files()
    stop = time.time()
    evaluation()
    #clean_up()
    print(f'Elapsed time: {stop - start} seconds')


