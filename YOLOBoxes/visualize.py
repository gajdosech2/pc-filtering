import os
import sys
import numpy as np
import cv2
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import matplotlib.gridspec as gridspec
import random

DIM = 2
PROB = False

def visualize(images_path, labels_path):
    images = os.listdir(images_path)
    random.shuffle(images)
    
    plt.subplots_adjust(left=0, bottom=0, right=1, top=1, wspace=0, hspace=0)
    
    j = 0
    for image_file in images:
        if (".png" not in image_file and ".PNG" not in image_file) or "labels" in image_file or "normalmap" in image_file:
            continue
            
        if j > (DIM*DIM - 1):
            break
        
        name = image_file.split(".")[0]
        if "_" in image_file:
            name = "_".join(image_file.split("_")[:2])
            
        img = cv2.imread(images_path + "/" + image_file)
        
        ax = plt.subplot(DIM, DIM, j+1, autoscale_on=True, frame_on=False, xmargin=0, ymargin=0)
        ax.axis('off') 
        imgplot = plt.imshow(img)
        with open(labels_path + "/" + name + ".txt", "r") as f:
            line = f.readline()
            while line and line != "---\n":
                clss = np.argmax([i for i in line.split()]) 
                [x, y, w, h] = [float(i) for i in f.readline().split()]       
                conf = [float(i) for i in f.readline().split()][0]
                    
                if clss != 2:
                    color = ['g','r','b','0'][clss]
                    rect = patches.Rectangle((x-w/2, y-h/2), w, h, linewidth=2, edgecolor=color, facecolor='none')
                    ax.add_patch(rect)
                    
                    if PROB:
                        ax.text(x, y, "%0.2f" % (conf), color=color)
                    
                line = f.readline()
        
        j += 1

    plt.show()

if __name__ == "__main__":
  if len(sys.argv) == 1:
    visualize('Images', 'Labels')
  elif len(sys.argv) == 2:
    visualize('data/train/basic/dataset_0', 'data/train/basic/dataset_0')
  elif len(sys.argv) == 3:
    visualize(sys.argv[1], sys.argv[2])





