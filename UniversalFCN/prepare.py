import os
import sys


def process(datasets_path, export_path):
  datasets = os.listdir(datasets_path)
  for d in datasets:
    if 'dirty' in d:
      dataset_name = d.split('_')[0]
      files = os.listdir(datasets_path + dataset_name + '_dirty')
      for f in files:
        print('processing: ' + f)
        dirty_file = datasets_path + dataset_name + '_dirty/' + f
        clean_file = datasets_path + dataset_name + '_clean/truth_' + f
        if os.name == 'nt':
          os.system('"utils\WCC.exe"' + 
                    ' --generate ' + 
                    dirty_file + ' ' + 
                    export_path + ' ' + 
                    clean_file)
        else:
          pass


if __name__ == "__main__":
  if len(sys.argv) == 1:
    process('data/cogs/train/', 'data/train/')
  elif len(sys.argv) == 2:
    process('data/cogs/' + sys.argv[1] + '/', 'data/' + sys.argv[1] + '/')

