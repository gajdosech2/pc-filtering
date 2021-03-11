import tensorflow as tf
import time

from model import generate_model
from generator import Generator


def enable_gpu():
    gpus = tf.config.experimental.list_physical_devices('GPU')
    if gpus:
        for gpu in gpus:
            tf.config.experimental.set_memory_growth(gpu, True)


def train(model, train_generator, val_generator, epochs=50):
    history = model.fit(train_generator,
                        steps_per_epoch=len(train_generator),
                        epochs=epochs,
                        validation_data=val_generator,
                        validation_steps=len(val_generator))
    return history

if __name__ == "__main__":
    model = generate_model(channels=5)

    train_dir = 'data/train'
    val_dir = 'data/val'

    BATCH_SIZE = 1
    train_generator = Generator(train_dir, BATCH_SIZE)
    val_generator = Generator(val_dir, BATCH_SIZE)

    EPOCHS = 12
    start = time.time()
    history = train(model, train_generator, val_generator, epochs=EPOCHS)
    print(f"Elapsed time: {time.time() - start} seconds")
    model.save_weights('weights.keras')
