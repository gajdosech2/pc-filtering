PC-Filtering
============


Commands
--------

*Linux:*

```
rocm-smi - show gpus
rocminfo
lspci - show devices
export PYTHONPATH=$PYTHONPATH:~/models
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/rocm/hip/lib - dynamic library error fix
sudo apt install clang-8
```

*Python:*

```
from tensorflow.python.client import device_lib
device_lib.list_local_devices()
tf.add(1, 2).numpy()
tf.test.is_gpu_available()
tf.config.list_physical_devices('GPU')
tf.__version__
```

Sources
-------

*Ignored files:*
- Model/DataFiles (.csv)
- Raw PCFiltering/Images (.imgs)

*Nana GUI:*
 - http://nanapro.org/en-us/
 
*Libraries:*
 - https://github.com/tensorflow/models
 - https://github.com/plaidml/plaidml
 
*RoCM:*
 - https://github.com/ROCmSoftwarePlatform/tensorflow-upstream
 - https://rocmdocs.amd.com/en/latest/
 - https://github.com/tensorflow/tensorflow/issues/40751
 - https://github.com/RadeonOpenCompute/ROCm/issues/125
 
*Keras:*
 - https://towardsdatascience.com/step-by-step-implementation-3d-convolutional-neural-network-in-keras-12efbdd7b130
 - https://towardsdatascience.com/implementing-a-fully-convolutional-network-fcn-in-tensorflow-2-3c46fb61de3b
 - https://keras.io/examples/

*Python Documentation:*
 - https://stackoverflow.com/questions/3898572/what-is-the-standard-python-docstring-format
 - http://daouzli.com/blog/docstring.html
 - http://queirozf.com/entries/python-docstrings-reference-examples
 
 *Ubuntu:*
 - https://stackoverflow.com/questions/40177006/c-variant-no-such-file-or-directory
 - https://askubuntu.com/questions/508934/how-to-install-libpng-and-zlib
 
 *Loss Functions:*
 - https://towardsdatascience.com/understanding-binary-cross-entropy-log-loss-a-visual-explanation-a3ac6025181a
 - https://towardsdatascience.com/intuitive-explanation-of-cross-entropy-5d45fc9fd240
