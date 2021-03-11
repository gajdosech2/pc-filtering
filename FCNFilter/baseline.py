# create a simple binary map from images and perform mathematical erosion with opencv
# calculate precision and recall between the result and truth!

d = 'process/'
e = 'result/'

def cogs_files():
    files = os.listdir(d)
    os.makedirs(e, exist_ok=True)
    for f in files:
        if '.cogs' in f and 'processed' not in f and 'truth' not in f:
            if os.name == 'nt':
                os.system('"utils\WCC.exe"' + 
                            ' --process ' + 
                            d + f + ' ' + 
                            d + f[:-5] + "_prediction.png" + ' ' + 
                            e)
            else:
                pass


def perform_morphology():
    pass

                
def input_files():
    files = os.listdir(d)
    for f in files:
        if '.cogs' in f and 'processed' not in f and 'truth' not in f:
            print("processing: " + f)
            t = ""
            if os.path.isfile(d + "truth_" + f):
                t = d + "truth_" + f
            if os.name == 'nt':
                os.system('"utils\WCC.exe"' + 
                            ' --generate ' + 
                            d + f + ' ' + 
                            d + ' ' +
                            t)
            else:
                pass
                
                
if __name__ == "__main__":
    #input_files()
    start = time.time()
    inference()
    cogs_files()
    stop = time.time()
    #evaluation()
    #clean_up()
    print(f"Elapsed time: {stop - start} seconds")