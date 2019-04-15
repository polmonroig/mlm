

# load required libraries
from sklearn.externals import joblib
from sklearn.base import BaseEstimator
from tensorflow.keras import Model
from tensorflow.keras import models
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


def pull_models():
    """
    Pulls models from current project
    """
    pid = os.fork()
    if pid == 0:
        sp = subprocess.Popen(["/bin/bash", "-i", "-c", "mlm pull models"])
        sp.communicate()
        exit(1)


def load_model(name, version=None):
    """
    Load a model from disk
    :param name: name of the model
    :param version: defines requested version(None == latest)
    :return: model
    """
    pid = os.fork()
    if pid == 0:
        if version is None: version = -1
        sp = subprocess.Popen(["/bin/bash", "-i", "-c", "mlm pull model" + str(version)])
        sp.communicate()
        exit(1)
    os.waitpid(pid)
    path = os.path.join(os.getcwd(), "models/" + name)
    if os.path.exists(path + ".pkl"):
        return joblib.load(path  + ".pkl")
    elif os.path.exists(path + ".h5"):
        return models.load_model(path + ".h5")


def save_model(model, name):
    """
    Saves a model to models directory with name
    :param model: model
    :param name: name of the model
    """
    # if model is from sklearn then save using joblib
    save_path = os.path.join(os.getcwd(), "models/" + name)
    if isinstance(model, BaseEstimator):
        joblib.dump(model, save_path + ".pkl")
    elif isinstance(model, Model):
        model.save(save_path + ".h5")
    else:
        print("Unsuported model type")
