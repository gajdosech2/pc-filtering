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
          os.system('CC.exe --generate ' + dirty_file + ' ' + clean_file + ' ' + export_path)
        else:
          os.system('./LCC --generate ' + dirty_file + ' ' + clean_file + ' ' + export_path)


if __name__ == "__main__":
  if len(sys.argv) == 1:
    process('cogs/train/', 'train/')
  elif len(sys.argv) == 2:
    process('cogs/' + sys.argv[1] + '/', sys.argv[1] + '/')

