

# load required libraries
import os
import subprocess


def push_models():
    """
    Pushes models from current project
    """
    create_process("mlm push models")


def pull_models():
    """
    Pulls models from current project
    """
    create_process("mlm pull models")


def pull_model(model_name, version, force):
    """
    Pull a model with the specified params from backup
    :param model_name: model name
    :param version: version of the model wanted
    :param force: force overwrite model
    """
    if not force:
        create_process("mlm pull model" + model_name + version)
    else:
        create_process("mlm pull model" + model_name +" " +  version + " --force")


def push_model(model_name):
    """
    Push the model with specified name
    :param model_name: name of the model
    """
    create_process("mlm push model " + model_name)


def create_process(context):
    pid = os.fork()
    if pid == 0:
        sp = subprocess.Popen(["/bin/bash", "-i", "-c", context])
        sp.communicate()
        exit(1)
