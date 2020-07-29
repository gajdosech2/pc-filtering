import os

export = 'train'
files = os.listdir('cogs')
for f in files:
  if 'truth' not in f:
    print('processing: ' + f) 
    os.system('./LCC ' + 'cogs/' + f + ' ' + 'cogs/truth_' + f + ' ' + export)
