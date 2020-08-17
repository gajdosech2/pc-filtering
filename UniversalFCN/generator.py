import os
#os.environ["KERAS_BACKEND"] = "plaidml.keras.backend"

import numpy as np
import cv2
import imageio
import keras
import matplotlib.pyplot as plt


class Generator(keras.utils.Sequence):

    def __init__(self, dataset_path, batch_size=2, shuffle_images=True, image_min_side=24):
        self.feature_images = self.masks = self.image_groups = self.mask_groups = []
        self.masks = []
        self.load_data(dataset_path)
        self.image_min_side = image_min_side
        self.batch_size = batch_size
        self.shuffle_images = shuffle_images
        self.create_image_groups()

    def load_data(self, dataset_path):
        files = os.listdir(dataset_path)
        for file in files:
            if "bitmap" in file:
                first_underscore = file.find("_")
                second_underscore = file[first_underscore + 1:].find("_")
                name = file[:first_underscore + second_underscore + 1]
                intensity_image = imageio.imread(dataset_path + "/" + name + "_intensitymap.png") / 255
                normals_image = imageio.imread(dataset_path + "/" + name + "_normalmap.png") / 255
                print(intensity_image.shape)
                print(file)
                feature_image = np.dstack((intensity_image[:, :, 0],
                                           normals_image[:, :, 0], normals_image[:, :, 1], normals_image[:, :, 2]))
                self.feature_images.append(feature_image)
                mask_image = imageio.imread(dataset_path + "/" + file) / 255
                mask_image = mask_image[:, :, 0]
                mask_image = 0.6 * mask_image
                #print(mask_image[:10][:10])
                #mask_image = mask_image.ravel()
                print(mask_image.shape)
                mask_image = np.expand_dims(mask_image, axis=2)
                self.masks.append(mask_image)

    def create_image_groups(self):
        if self.shuffle_images:
            seed = 4321
            np.random.seed(seed)
            np.random.shuffle(self.feature_images)
            np.random.seed(seed)
            np.random.shuffle(self.masks)

        self.image_groups = [[self.feature_images[x % len(self.feature_images)] for x in range(i, i + self.batch_size)]
                             for i in range(0, len(self.feature_images), self.batch_size)]
        self.mask_groups = [[self.masks[x % len(self.masks)] for x in range(i, i + self.batch_size)]
                            for i in range(0, len(self.masks), self.batch_size)]

    def enlarge_image(self, img):
        h, w, c = img.shape
        if min(h, w) < self.min_side_len:
            im_scale = float(self.min_side_len) / h if h < w else float(self.min_side_len) / w
        else:
            im_scale = 1.

        new_h = int(h * im_scale)
        new_w = int(w * im_scale)
        re_im = cv2.resize(img, (new_w, new_h), interpolation=cv2.INTER_AREA)
        return re_im, new_h / h, new_w / w

    def construct_batch(self, image_group):
        max_shape = tuple(max(image.shape[d] for image in image_group) for d in range(len(image_group[0].shape)))

        #if max_shape[0] % 2 == 1:
        #    max_shape = (max_shape[0]+1, max_shape[1], max_shape[2])

        #if max_shape[1] % 2 == 1:
        #    max_shape = (max_shape[0], max_shape[1]+1, max_shape[2])

        batch = np.zeros((self.batch_size,) + max_shape, dtype='float32')

        for image_index, image in enumerate(image_group):
            if len(image.shape) == 1:
                batch[image_index, :image.shape[0]] = image
            elif len(image.shape) == 2:
                batch[image_index, :image.shape[0], :image.shape[1]] = image
            else:
                batch[image_index, :image.shape[0], :image.shape[1], :image.shape[2]] = image

        return batch

    def __len__(self):
        return len(self.image_groups)

    def __getitem__(self, index):
        image_group = self.image_groups[index]
        mask_group = self.mask_groups[index]
        image_batch = self.construct_batch(image_group)
        mask_batch = self.construct_batch(mask_group)

        return np.array(image_batch), np.array(mask_batch)


def show(image, mask):
    plt.figure(figsize=(4, 8))

    plt.subplot(3, 1, 1)
    plt.axis('off')
    plt.imshow(mask[:, :, 0]).set_cmap('gray')

    plt.subplot(3, 1, 2)
    plt.axis('off')
    plt.imshow(image[:, :, 0]).set_cmap('gray')

    plt.subplot(3, 1, 3)
    plt.axis('off')
    plt.imshow(image[:, :, 1:])
    plt.show()


if __name__ == "__main__":
    train_generator = Generator('data/train')
    print(len(train_generator))
    image_batch, mask_batch = train_generator[0]
    print(image_batch.shape)
    print(mask_batch.shape)
    show(image_batch[0], mask_batch[0])


