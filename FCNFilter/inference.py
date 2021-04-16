import numpy as np
import cv2
import time
import os
import math

from model import generate_model, setup_gpu

#os.environ["CUDA_VISIBLE_DEVICES"]="-1"    

PROCESS_PATH = 'process/'
RESULT_PATH = 'result/'
WEIGHTS_FILE = 'weights.h5'

SCALE_FACTOR = 2.0
ALPHA = 0.95
CLEAN_UP = False

            
def cogs_files():
    files = os.listdir(PROCESS_PATH)
    os.makedirs(RESULT_PATH, exist_ok=True)
    for f in files:
        if '.cogs' in f and 'processed' not in f and 'truth' not in f:
            if os.name == 'nt':
                os.system('"utils\WCC.exe"' + 
                          ' --process ' +
                          PROCESS_PATH + f + ' ' +
                          PROCESS_PATH + f[:-5] + '_prediction.png' + ' ' +
                          RESULT_PATH)
            else:
                print('OS other than Windows is currently not supported.')
                
                
def input_files():
    files = os.listdir(PROCESS_PATH)
    for f in files:
        if '.cogs' in f and 'processed' not in f and 'truth' not in f:
            print("processing: " + f)
            t = ""
            if os.path.isfile(PROCESS_PATH + 'truth_' + f):
                t = PROCESS_PATH + 'truth_' + f
            if os.name == 'nt':
                os.system('"utils\WCC.exe"' + 
                          ' --generate ' +
                          PROCESS_PATH + f + ' ' +
                          PROCESS_PATH + ' ' +
                          t)
            else:
                print('OS other than Windows is currently not supported.')


def clean_up():
    files = os.listdir(PROCESS_PATH)
    for f in files:
        if '.png' in f:
            os.remove(PROCESS_PATH + f)

            
def generate_feature_image(f):
    intensity_image = cv2.imread(PROCESS_PATH + f + '_intensitymap.png', cv2.IMREAD_GRAYSCALE) / 255
    depth_image = cv2.imread(PROCESS_PATH + f + '_depthmap.png', cv2.IMREAD_GRAYSCALE) / 255
    normals_image = cv2.imread(PROCESS_PATH + f + '_normalmap.png', cv2.IMREAD_UNCHANGED)
    normals_image = cv2.cvtColor(normals_image, cv2.COLOR_BGR2RGB) / 255
    feature_image = np.dstack((intensity_image,
                               depth_image,
                               normals_image[:, :, 0],
                               normals_image[:, :, 1],
                               normals_image[:, :, 2]))
  
    shape = feature_image.shape
    i1 = 2**math.ceil(math.log2(shape[0])) - shape[0]
    i2 = 2**math.ceil(math.log2(shape[1])) - shape[1]
    shape = (shape[0]+i1, shape[1]+i2, shape[2])
    resized = np.zeros(shape, dtype=np.float32)
    resized[:feature_image.shape[0], :feature_image.shape[1], :] = feature_image
    
    feature_image = cv2.resize(resized, (0,0), fx=SCALE_FACTOR, fy=SCALE_FACTOR)
    feature_image = np.expand_dims(feature_image, axis=0)
    return feature_image, intensity_image.shape[0], intensity_image.shape[1]
           
            
def inference():
    start = time.time()
    model = generate_model()
    model.load_weights(WEIGHTS_FILE)
    print(f'Load time: {time.time() - start} seconds')

    files = os.listdir(PROCESS_PATH)   
    for f in files:
        if 'intensitymap' in f: 
            f = '_'.join(f.split('_')[:2])
            print('processing: ' + f)
            feature_image, original_width, original_height = generate_feature_image(f)
            
            start = time.time()
            prediction = model.predict(feature_image)
            print(f'Inference time: {time.time() - start} seconds')
            prediction = np.round(prediction[0] * ALPHA)
            prediction = cv2.resize(prediction, (0,0), fx=1/SCALE_FACTOR, fy=1/SCALE_FACTOR)
            prediction = prediction[:original_width, :original_height]
            #kernel = np.ones((1, 1), np.uint8)
            #prediction = cv2.erode(prediction, kernel, iterations = 1)

            cv2.imwrite(PROCESS_PATH + f + '_prediction.png', prediction.astype(np.uint8) * 255)
            
            
def evaluation():
    size = 0
    metric = 0
    max_metric = 0
    min_metric = 1
    files = os.listdir(PROCESS_PATH)
    for truth in files:
        if 'truthmask' in truth:
            size += 1
            name = truth[:-14]
            prediction = name + '_prediction.png'
            prediction = cv2.imread(PROCESS_PATH + prediction, cv2.IMREAD_GRAYSCALE) / 255
            truth = cv2.imread(PROCESS_PATH + truth, cv2.IMREAD_GRAYSCALE) / 255
            
            intersection = np.sum(prediction * truth)
            union = np.sum(truth) + np.sum(prediction) - intersection
            iou = intersection / union
            metric += iou
            max_metric = max(iou, max_metric)
            min_metric = min(iou, min_metric)
            with open (RESULT_PATH + name + '_eval.txt', 'w') as result:
                print(iou, file=result)
    
    if size > 0:
        metric /= size
        print('Average IoU: ' + str(metric))
        print('Max IoU: ' + str(max_metric))
        print('Min IoU: ' + str(min_metric))
        with open (RESULT_PATH + 'average_iou.txt', 'w') as result:
            print(metric, file=result)


if __name__ == '__main__':
    setup_gpu()
    start = time.time()
    if not any('.png' in f for f in os.listdir(PROCESS_PATH)):
        input_files()
    inference()
    cogs_files()
    stop = time.time()
    evaluation()
    if CLEAN_UP:
        clean_up()
    print(f'Total time: {stop - start} seconds')


