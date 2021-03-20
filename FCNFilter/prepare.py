import os
import sys


def process(datasets_path, export_path):
    datasets = os.listdir(datasets_path)
    if not os.path.exists(export_path):
        os.makedirs(export_path, exist_ok=True)
    
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
                    print('OS other than Windows is currently not supported.')


if __name__ == "__main__":
    if len(sys.argv) == 1:
        process('data/_COGS/train/', 'data/train/')
    elif len(sys.argv) == 2:
        process('data/_COGS/' + sys.argv[1] + '/', 'data/' + sys.argv[1] + '/')

