import imageio
import os
import numpy as np
import random

WHITE = np.array([255, 255, 255])


def random_color():
  rgb = [random.randrange(0, 256), random.randrange(0, 256), random.randrange(70, 256)]
  random.shuffle(rgb)
  return tuple(rgb)
  

def colorize(path, file):
  image = imageio.imread(path + file)
  image = np.array(image)
  
  colorized = np.stack((image,)*3, axis=-1) 
  for y in range(colorized.shape[0]):
    for x in range(colorized.shape[1]):
      if np.array_equal(colorized[y, x, :], WHITE):
         color = random_color()
         colorized[y, x, :] = color
         
         stack = list()
         stack.append((y, x))
         while stack:
           y1, x1 = stack[-1]
           stack = stack[:-1]
           for offset_x in [-1, 0, 1]:
             for offset_y in [-1, 0, 1]:
               x2, y2 = x1 + offset_x, y1 + offset_y
               if offset_x == offset_y == 0 or x2 < 0 or y2 < 0 or x2 >= colorized.shape[1] or y2 >= colorized.shape[0]:
                 continue
               if np.array_equal(colorized[y2, x2, :], WHITE):
                 colorized[y2, x2, :] = color
                 stack.append((y2, x2))
                 
  imageio.imwrite(path + file.split(".")[0] + "color.png", colorized)
  

def process():
  path = "result/"
  files = os.listdir(path)
  for file in files:
    if "color" not in file:
      print("processing: " + file)
      colorize(path, file)
               
  
if __name__ == "__main__":
    process()