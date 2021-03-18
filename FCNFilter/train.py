import tensorflow as tf
import time
import os
import sys

from model import generate_model
from generator import Generator
from keras.callbacks import ModelCheckpoint
from keras.optimizers import Adam
from keras.metrics import *

from losses import scaled_binary_crossentropy, weighted_binary_crossentropy, binary_focal_loss


def setup_gpu():
    gpus = tf.config.list_physical_devices('GPU')
    if gpus:
        try:
            for gpu in gpus:
                tf.config.experimental.set_memory_growth(gpu, True)
        except RuntimeError as e:
            print(e)


def train(gpu_init=0, batch_size=1, epochs=16, steps=6, lr=1e-3):
    if gpu_init:
        setup_gpu()
        
    WEIGHTS_FILE = 'weights.keras'
    
    model = generate_model(channels=5)  
    if (os.path.exists(WEIGHTS_FILE)):
        model.load_weights(WEIGHTS_FILE)

    train_generator = Generator('data/train', batch_size)
    val_generator = Generator('data/val', batch_size)

    callbacks = [
        ModelCheckpoint(WEIGHTS_FILE, save_best_only=True)
    ]

    start = time.time()  
    for i in range(steps): 
        model.compile(optimizer=Adam(lr=lr),
                      loss=binary_focal_loss(alpha=0.08, gamma=4),
                      metrics=[#'accuracy',
                      Precision(),
                      Recall(),
                      #SpecificityAtSensitivity(0.5, num_thresholds=1, name='specificity'), 
                      #SensitivityAtSpecificity(0.5, num_thresholds=1, name='sensitivity'),
                      #TrueNegatives(), TruePositives(), FalseNegatives(), FalsePositives()
                      ])
    
        history = model.fit(train_generator,
                            steps_per_epoch=len(train_generator),
                            epochs=epochs,
                            validation_data=val_generator,
                            validation_steps=len(val_generator))
        
        lr = lr / 10
        model.save_weights(WEIGHTS_FILE + str(i))
        
    print(f'Elapsed time: {time.time() - start} seconds')
    model.save_weights(WEIGHTS_FILE)
    return history


if __name__ == '__main__':
    print('\n ARGUMENTS: ' + str(sys.argv) + '\n')
    train(*map(int, sys.argv[1:]))
