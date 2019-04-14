

# load required libraries
from sklearn.externals import joblib
import os
import subprocess


def push_models():
    """
    Pushes models from current project
    """
    pid = os.fork()
    if pid == 0:
        sp = subprocess.Popen(["/bin/bash", "-i", "-c", "mlm push models"])
        sp.communicate()
        exit(1)
    else:
        print("Models pushed")


def pull_models():
    """
    Pulls models from current project
    """
    sp = subprocess.Popen(["/bin/bash", "-i", "-c", "mlm pull models"])
    sp.communicate()


def load_model(path, version=None):
    """
    Load a model from disk
    :param path: location of the model
    :param version: defines requested version(None == latest)
    :return: model
    """
    return 3


def save_model(model, path):
    """
    Saves a model to disk
    :param model: model
    :param path: save location
    """
    x = 3
