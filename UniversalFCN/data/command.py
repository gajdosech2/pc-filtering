import os

export = 'train'
files = os.listdir('cogs')
for f in files:
  print('processing: ' + f) 
  os.system('./LCC ' + 'cogs/' + f + ' ' + export + ' --trim')
