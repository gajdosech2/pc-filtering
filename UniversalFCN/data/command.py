import os

export = 'train'
files = os.listdir('cogs')
for f in files:
  os.system('./LCC ' + 'cogs/' + f + ' ' + export + ' --trim')
