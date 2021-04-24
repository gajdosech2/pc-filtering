import cv2
import numpy as np
import os
from sklearn.metrics import recall_score, precision_score
from statistics import mean

PROCESS_PATH = 'process/'
RESULT_PATH = 'result/'


def cogs_files():
    files = os.listdir(PROCESS_PATH)
    os.makedirs(RESULT_PATH, exist_ok=True)
    for f in files:
        if '.cogs' in f and 'processed' not in f and 'truth' not in f:
            print("processing: " + f)
            if os.name == 'nt':
                os.system('"utils\WCC.exe"' + 
                          ' --process ' +
                          PROCESS_PATH + f + ' ' +
                          PROCESS_PATH + f[:-5] + "_morphology.png" + ' ' +
                          RESULT_PATH)
            else:
                print('OS other than Windows is currently not supported.')


def morphology():
    files = os.listdir(PROCESS_PATH)
    for f in files:
        if 'binarymap' in f:
            f = '_'.join(f.split('_')[:2])
            print('processing: ' + f)
            binary_map = cv2.imread(PROCESS_PATH + f + '_binarymap.png', cv2.IMREAD_GRAYSCALE)
            
            kernel = np.ones((7, 7), np.uint8)
            morph = cv2.erode(binary_map, kernel, iterations = 1)
            #morph = cv2.morphologyEx(binary_map, cv2.MORPH_OPEN, kernel)
            cv2.imwrite(PROCESS_PATH + f + '_morphology.png', morph)
            
            #difference = cv2.subtract(binary_map, morph)
            #print(not np.any(difference))
            

def evaluate():
    def iou(output, target):
        intersection = np.sum(output * target)
        union =  np.sum(output) + np.sum(target) - intersection
        return intersection / union

    files = os.listdir(PROCESS_PATH)
    morph_iou, morph_precision, morph_recall, morph_f1 = [], [], [], []
    net_iou, net_precision, net_recall, net_f1 = [], [], [], []
    gt_iou = []
    for f in files:
        if 'truthmask' in f:
            f = '_'.join(f.split('_')[:2])
            print('evaluating: ' + f)
            
            raw_map = cv2.imread(PROCESS_PATH + f + '_binarymap.png', cv2.IMREAD_GRAYSCALE)
            truth_mask = cv2.imread(PROCESS_PATH + f + '_truthmask.png', cv2.IMREAD_GRAYSCALE)
            morph_mask = cv2.imread(PROCESS_PATH + f + '_morphology.png', cv2.IMREAD_GRAYSCALE)
            net_mask = cv2.imread(PROCESS_PATH + f + '_prediction.png', cv2.IMREAD_GRAYSCALE)
                      
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
            
            m_p = precision_score(truth_mask.flatten(), morph_mask.flatten())
            n_p = precision_score(truth_mask.flatten(), net_mask.flatten())
            morph_precision.append(m_p)
            net_precision.append(n_p)
            
            m_r = recall_score(truth_mask.flatten(), morph_mask.flatten())
            n_r = recall_score(truth_mask.flatten(), net_mask.flatten())
            morph_recall.append(m_r)
            net_recall.append(n_r)
            
            morph_f1.append(2 * (m_p * m_r) / (m_p + m_r))
            net_f1.append(2 * (n_p * n_r) / (n_p + n_r))
           
    if len(morph_iou) == 0:
        print('No GT data found!')
        return  
    
    print(f'\nError rating: {round((1 - mean(gt_iou)) * 100)}\n')
    print(f'AVG Morph - iou: {mean(morph_iou)}, precision: {mean(morph_precision)}, recall: {mean(morph_recall)}, f1: {mean(morph_f1)}')
    print(f'AVG Netwo - iou: {mean(net_iou)}, precision: {mean(net_precision)}, recall: {mean(net_recall)}, f1: {mean(net_f1)}\n')
    print(f'MAX Morph - iou: {max(morph_iou)}, precision: {max(morph_precision)}, recall: {max(morph_recall)}, f1: {max(morph_f1)}')
    print(f'MAX Netwo - iou: {max(net_iou)}, precision: {max(net_precision)}, recall: {max(net_recall)}, f1: {max(net_f1)}\n')
    print(f'MIN Morph - iou: {min(morph_iou)}, precision: {min(morph_precision)}, recall: {min(morph_recall)}, f1: {min(morph_f1)}')
    print(f'MIN Netwo - iou: {min(net_iou)}, precision: {min(net_precision)}, recall: {min(net_recall)}, f1: {min(net_f1)}')


if __name__ == "__main__":
    morphology()
    evaluate()
    #cogs_files()

