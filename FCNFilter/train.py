import time
import os
import sys
import json
from keras.callbacks import ModelCheckpoint, ReduceLROnPlateau, EarlyStopping
from keras.optimizers import Adam
from keras.metrics import *

from model import generate_model, setup_gpu
from generator import Generator
from losses import scaled_binary_crossentropy, weighted_binary_crossentropy, binary_focal_loss

WEIGHTS_FILE = 'weights.h5'

#os.environ["CUDA_VISIBLE_DEVICES"]="-1"    


def train_simple(batch_size=1, epochs=8, lr_exp=4):      
    lr = 10**(-lr_exp)
    model = generate_model()  
    if os.path.exists(WEIGHTS_FILE):
        print('Loading saved weights!')
        model.load_weights(WEIGHTS_FILE)

    train_generator = Generator('data/train', batch_size)
    val_generator = Generator('data/val', batch_size) 
    
    model.compile(optimizer=Adam(learning_rate=lr),
                  loss=binary_focal_loss(alpha=0.07, gamma=5),
                  metrics=[Precision(name='precision'), Recall(name='recall')])
    
    history = model.fit(train_generator,
                        steps_per_epoch=len(train_generator),
                        epochs=epochs,
                        validation_data=val_generator,
                        validation_steps=len(val_generator))
        
    model.save_weights(WEIGHTS_FILE)
    return history


def train(batch_size=1, epochs=40, lr_exp=5):
    lr = 10**(-lr_exp)
    model = generate_model()  
    if os.path.exists(WEIGHTS_FILE):
        print('Loading saved weights!')
        model.load_weights(WEIGHTS_FILE)

    train_generator = Generator('data/train', batch_size)
    val_generator = Generator('data/val', batch_size) 
    
    checkpoint_callback = ModelCheckpoint(filepath='weights/{precision:.4f}_{recall:.4f}_weights.h5', 
                                          save_weights_only=True,
                                          monitor='precision',
                                          mode='max',
                                          save_best_only=True)                                  
    #reduce_lr = ReduceLROnPlateau(monitor='loss', factor=0.25, patience=4, verbose=1)
    early_stopping = EarlyStopping(monitor='loss', min_delta=0.0001, patience=8, verbose=1)
    start = time.time()  
    
    history = dict()
    for param in ['val_loss', 'val_precision', 'val_recall', 'loss', 'precision', 'recall']:
        history[param] = list()
        
    for step in range(4):
        model.compile(optimizer=Adam(learning_rate=lr),
                      loss=binary_focal_loss(alpha=0.07, gamma=5),
                      metrics=[#'accuracy',
                      Precision(name='precision'),
                      Recall(name='recall'),
                      #SpecificityAtSensitivity(0.5, num_thresholds=1, name='specificity'),
                      #SensitivityAtSpecificity(0.5, num_thresholds=1, name='sensitivity'),
                      #TrueNegatives(), TruePositives(), FalseNegatives(), FalsePositives()
                      ])
        
        h = model.fit(train_generator,
                      steps_per_epoch=len(train_generator),
                      epochs=epochs,
                      validation_data=val_generator,
                      validation_steps=len(val_generator),
                      callbacks=[early_stopping])
        
        for param in ['val_loss', 'val_precision', 'val_recall', 'loss', 'precision', 'recall']:
            history[param] += [float(v) for v in h.history[param]]
        
        lr = lr / 5
        train_generator.create_image_groups()
        
    with open('training/history.json', 'w') as histfile:
        json.dump(history, histfile)
    model.save_weights(f'weights.h5')          
            
    print(f'Elapsed time: {time.time() - start} seconds')


if __name__ == '__main__':
    print('\n ARGUMENTS: ' + str(sys.argv) + '\n')
    setup_gpu()
    
    if int(sys.argv[1]) == 1:
        train(*map(int, sys.argv[2:]))
    else:
        train_simple(*map(int, sys.argv[2:]))
