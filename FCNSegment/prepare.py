import os
import sys


def process(dataset_type, dataset_name):    
  raw_path = 'data/raw/' + dataset_type + '/' + dataset_name + '/'
  dataset_parts = os.listdir(raw_path)
  
  for dataset_part in dataset_parts:
    export_path = 'data/' + dataset_type + '/' + dataset_name + '/' + dataset_part + '/'
    if not os.path.exists(export_path):
      os.makedirs(export_path, exist_ok=True)
  
    data_path = raw_path + dataset_part + '/captures/'
    files = os.listdir(data_path)
  
    for file in files:
      if ".ply" in file or ".cogs" in file:  
        name = file.split('.')[0]
        print("processing: " + name)
        labels_file = name + "_labels.png"
        if os.name == 'nt':
          os.system('"utils\WCC.exe"' + 
                    ' --segment ' + 
                    data_path + file + ' ' + 
                    export_path + ' ' + ' ' + 
                    data_path + labels_file)
        else:
          pass


if __name__ == "__main__":
  if len(sys.argv) == 1:
    process('train', 'basic')
  elif len(sys.argv) == 3:
    process(sys.argv[1], sys.argv[2])

