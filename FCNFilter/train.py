import time
import os
import sys
from keras.callbacks import ModelCheckpoint
from keras.optimizers import Adam
from tensorflow.keras.optimizers.schedules import ExponentialDecay
from keras.metrics import *

from model import generate_model, setup_gpu
from generator import Generator
from losses import scaled_binary_crossentropy, weighted_binary_crossentropy, binary_focal_loss

WEIGHTS_FILE = 'weights.h5'


def train_simple(batch_size=1, epochs=8, lr_exp=4):      
    lr = 10**(-lr_exp)
    model = generate_model()  
    if os.path.exists(WEIGHTS_FILE):
        print('Loading saved weights!')
        model.load_weights(WEIGHTS_FILE)

    train_generator = Generator('data/train', batch_size)
    val_generator = Generator('data/val', batch_size) 
    
    model.compile(optimizer=Adam(learning_rate=lr),
                  loss=binary_focal_loss(alpha=0.05, gamma=5),
                  metrics=[Precision(name='precision'), Recall(name='recall')])
    
    history = model.fit(train_generator,
                        steps_per_epoch=len(train_generator),
                        epochs=epochs,
                        validation_data=val_generator,
                        validation_steps=len(val_generator))
        
    model.save_weights(WEIGHTS_FILE)
    return history


def train(batch_size=1, epochs=32, lr_exp=4):
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
        
    lr_schedule = ExponentialDecay(initial_learning_rate=lr, 
                                   decay_steps=len(train_generator),
                                   decay_rate=0.85,
                                   staircase=True)

    start = time.time()  
    
    for g in [3, 4, 5]:
        for a in [0.05, 0.06, 0.07, 0.09, 0.12]:
            model.compile(optimizer=Adam(learning_rate=lr_schedule),
                          loss=binary_focal_loss(alpha=a, gamma=g),
                          metrics=[#'accuracy',
                          Precision(name='precision'),
                          Recall(name='recall'),
                          #SpecificityAtSensitivity(0.5, num_thresholds=1, name='specificity'),
                          #SensitivityAtSpecificity(0.5, num_thresholds=1, name='sensitivity'),
                          #TrueNegatives(), TruePositives(), FalseNegatives(), FalsePositives()
                          ])
    
            history = model.fit(train_generator,
                                steps_per_epoch=len(train_generator),
                                epochs=epochs,
                                validation_data=val_generator,
                                validation_steps=len(val_generator),
                                callbacks=[checkpoint_callback])
        
            model.save_weights(f'weights/gamma={g}_alpha={a}_weights.h5')
            
    print(f'Elapsed time: {time.time() - start} seconds')


if __name__ == '__main__':
    print('\n ARGUMENTS: ' + str(sys.argv) + '\n')
    setup_gpu()
    
    if int(sys.argv[1]) == 1:
        train(*map(int, sys.argv[2:]))
    else:
        train_simple(*map(int, sys.argv[2:]))
