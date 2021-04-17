from matplotlib import pyplot as plt
import json


HIST_FILE = 'training/history.json'

def calculate_f1(history):
    f1_train = [2 * (s[0] * s[1]) / (s[0] + s[1]) for s in zip(history['precision'], history['recall'])]
    f1_val = [2 * (s[0] * s[1]) / (s[0] + s[1]) for s in zip(history['val_precision'], history['val_recall'])]
    
    history['f1'] = f1_train
    history['val_f1'] = f1_val
    

def plot_param(history, param):
    #plt.yscale('log')
    plt.plot(history[param][1:])
    plt.plot(history['val_' + param][1:])
    
    #plt.title('model ' + param)
    plt.ylabel(param)
    plt.xlabel('epoch')
    
    plt.legend(['train', 'val'])
    plt.minorticks_on()
    #plt.xticks(range(1, 130, 10))
    #plt.yticks([0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0])
    plt.grid(b=True, which='minor', color='#999999', linestyle='-', alpha=0.2)
    plt.grid(b=True, which='major', color='#666666', linestyle='-', alpha=0.4)
    plt.savefig('training/plot_' + param + '.pdf', bbox_inches='tight')
    plt.show()
 
        
if __name__ == '__main__':
    with open(HIST_FILE, 'r') as histfile:
        history = json.load(histfile)
     
    plot_param(history, 'loss')
    #calculate_f1(history)
    #plot_param(history, 'precision')
    #plot_param(history, 'recall')
    #plot_param(history, 'f1')
    

       