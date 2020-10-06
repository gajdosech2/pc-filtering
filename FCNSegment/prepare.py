import os
import sys


def process(dataset_type):
  export_path = 'data/' + dataset_type
  raw_path = 'data/raw/'
  
  cogs_path = raw_path + dataset_type + 'cogs/'
  labels_path = raw_path + dataset_type + 'labels/'
  cogs_files = os.listdir(cogs_path)
  
  if not os.path.exists(export_path):
    os.makedirs(export_path, exist_ok=True)
    
  for cogs_file in cogs_files:
    name = cogs_file.split('.')[0]
    print("processing: " + name)
    labels_file = name + "_labels.png"
    if os.name == 'nt':
      os.system('"utils\WCC.exe"' + 
                ' --segment ' + 
                cogs_path + cogs_file + ' ' + 
                export_path + ' ' + 
                labels_path + labels_file)
    else:
      pass


if __name__ == "__main__":
  if len(sys.argv) == 1:
    process('train/')
  elif len(sys.argv) == 2:
    process(sys.argv[1] + '/')

