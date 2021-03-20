import cv2
import numpy as np
import imageio
import os
import math
from sklearn.metrics import recall_score, precision_score


d = 'process/'
e = 'result/'

def cogs_files():
    files = os.listdir(d)
    os.makedirs(e, exist_ok=True)
    for f in files:
        if '.cogs' in f and 'processed' not in f and 'truth' not in f:
            print("processing: " + f)
            if os.name == 'nt':
                os.system('"utils\WCC.exe"' + 
                            ' --process ' + 
                            d + f + ' ' + 
                            d + f[:-5] + "_morphology.png" + ' ' + 
                            e)
            else:
                pass


def morphology():
    files = os.listdir(d)
    for f in files:
        if "binarymap" in f:
            f = '_'.join(f.split('_')[:2])
            print("processing: " + f)
            binary_map = cv2.imread(d + f + "_binarymap.png", cv2.IMREAD_GRAYSCALE)
            
            kernel = np.ones((7, 7), np.uint8)
            morph = cv2.erode(binary_map, kernel, iterations = 1)
            #morph = cv2.morphologyEx(binary_map, cv2.MORPH_OPEN, kernel)
            cv2.imwrite(d + f + "_morphology.png", morph) 
            
            #difference = cv2.subtract(binary_map, morph)    
            #print(not np.any(difference))
            

def evaluage():
    def iou(output, target):
        intersection = np.sum(output * target)
        union =  np.sum(output) + np.sum(target) - intersection
        return intersection / union

    files = os.listdir(d)
    morph_iou, morph_precision, morph_recall = [], [], []
    net_iou, net_precision, net_recall = [], [], []
    for f in files:
        if "truthmask" in f:
            f = '_'.join(f.split('_')[:2])
            print("evaluating: " + f)
            
            truth_mask = cv2.imread(d + f + "_truthmask.png", cv2.IMREAD_GRAYSCALE) 
            morph_mask = cv2.imread(d + f + "_morphology.png", cv2.IMREAD_GRAYSCALE)
            net_mask = cv2.imread(d + f + "_prediction.png", cv2.IMREAD_GRAYSCALE)
                      
            kernel = np.ones((7, 7), np.uint8)
            morph_mask = cv2.dilate(morph_mask, kernel, iterations = 1)
            #net_mask = cv2.dilate(net_mask, kernel, iterations = 1)
            
            truth_mask = truth_mask / 255
            morph_mask = morph_mask / 255
            net_mask = net_mask / 255
            
            morph_iou.append(iou(morph_mask, truth_mask))
            net_iou.append(iou(net_mask, truth_mask)) 
            
            morph_precision.append(precision_score(truth_mask.flatten(), morph_mask.flatten()))
            net_precision.append(precision_score(truth_mask.flatten(), net_mask.flatten()))
            
            morph_recall.append(recall_score(truth_mask.flatten(), morph_mask.flatten()))
            net_recall.append(recall_score(truth_mask.flatten(), net_mask.flatten()))
           
    samples = len(morph_iou)
    if not samples:
        print('No GT data found!')
        return 
    avg_morph_iou, avg_morph_precision, avg_morph_recall = sum(morph_iou)/samples, sum(morph_precision)/samples, sum(morph_recall)/samples
    avg_net_iou, avg_net_precision, avg_net_recall = sum(net_iou)/samples, sum(net_precision)/samples, sum(net_recall)/samples        
    
    print()
    print ("AVG Morph - iou: {}, precision: {}, recall: {}".format(avg_morph_iou, avg_morph_precision, avg_morph_recall))
    print ("AVG Netwo - iou: {}, precision: {}, recall: {}".format(avg_net_iou, avg_net_precision, avg_net_recall))
    print()
    print ("MAX Morph - iou: {}, precision: {}, recall: {}".format(max(morph_iou), max(morph_precision), max(morph_recall)))
    print ("MAX Netwo - iou: {}, precision: {}, recall: {}".format(max(net_iou), max(net_precision), max(net_recall)))
    print()
    print ("MIN Morph - iou: {}, precision: {}, recall: {}".format(min(morph_iou), min(morph_precision), min(morph_recall)))
    print ("MIN Netwo - iou: {}, precision: {}, recall: {}".format(min(net_iou), min(net_precision), min(net_recall)))   


if __name__ == "__main__":
    morphology()
    evaluage()
    #cogs_files()

