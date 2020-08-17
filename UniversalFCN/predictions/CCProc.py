import os

files = os.listdir('.')
for f in files:
    if '.cogs' in f and 'processed' not in f:
        name = f[:-5]
        os.system('./CC ' + f + ' ' + name + "_prediction.png")



