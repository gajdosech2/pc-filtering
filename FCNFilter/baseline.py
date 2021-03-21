import cv2
import numpy as np
import os
from sklearn.metrics import recall_score, precision_score
from statistics import mean

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
                print('OS other than Windows is currently not supported.')


def morphology():
    files = os.listdir(d)
    for f in files:
        if 'binarymap' in f:
            f = '_'.join(f.split('_')[:2])
            print('processing: ' + f)
            binary_map = cv2.imread(d + f + '_binarymap.png', cv2.IMREAD_GRAYSCALE)
            
            kernel = np.ones((7, 7), np.uint8)
            morph = cv2.erode(binary_map, kernel, iterations = 1)
            #morph = cv2.morphologyEx(binary_map, cv2.MORPH_OPEN, kernel)
            cv2.imwrite(d + f + '_morphology.png', morph) 
            
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
    gt_iou = []
    for f in files:
        if 'truthmask' in f:
            f = '_'.join(f.split('_')[:2])
            print('evaluating: ' + f)
            
            raw_map = cv2.imread(d + f + '_binarymap.png', cv2.IMREAD_GRAYSCALE) 
            truth_mask = cv2.imread(d + f + '_truthmask.png', cv2.IMREAD_GRAYSCALE) 
            morph_mask = cv2.imread(d + f + '_morphology.png', cv2.IMREAD_GRAYSCALE)
            net_mask = cv2.imread(d + f + '_prediction.png', cv2.IMREAD_GRAYSCALE)
                      
            kernel = np.ones((7, 7), np.uint8)
            morph_mask = cv2.dilate(morph_mask, kernel, iterations = 1)
            #net_mask = cv2.dilate(net_mask, kernel, iterations = 1)
            
            raw_map = raw_map / 255
            truth_mask = truth_mask / 255
            morph_mask = morph_mask / 255
            net_mask = net_mask / 255
            
            gt_iou.append(iou(raw_map, truth_mask))
            #print(iou(raw_map, truth_mask))
            
            morph_iou.append(iou(morph_mask, truth_mask))
            net_iou.append(iou(net_mask, truth_mask)) 
            
            morph_precision.append(precision_score(truth_mask.flatten(), morph_mask.flatten()))
            net_precision.append(precision_score(truth_mask.flatten(), net_mask.flatten()))
            
            morph_recall.append(recall_score(truth_mask.flatten(), morph_mask.flatten()))
            net_recall.append(recall_score(truth_mask.flatten(), net_mask.flatten()))
           
    if len(morph_iou) == 0:
        print('No GT data found!')
        return  
    
    print (f'\nError rating: {round((1 - mean(gt_iou)) * 100)}\n')
    print (f'AVG Morph - iou: {mean(morph_iou)}, precision: {mean(morph_precision)}, recall: {mean(morph_recall)}')
    print (f'AVG Netwo - iou: {mean(net_iou)}, precision: {mean(net_precision)}, recall: {mean(net_recall)}\n')
    print (f'MAX Morph - iou: {max(morph_iou)}, precision: {max(morph_precision)}, recall: {max(morph_recall)}')
    print (f'MAX Netwo - iou: {max(net_iou)}, precision: {max(net_precision)}, recall: {max(net_recall)}\n')
    print (f'MIN Morph - iou: {min(morph_iou)}, precision: {min(morph_precision)}, recall: {min(morph_recall)}')
    print (f'MIN Netwo - iou: {min(net_iou)}, precision: {min(net_precision)}, recall: {min(net_recall)}')   


if __name__ == "__main__":
    morphology()
    evaluage()
    #cogs_files()

