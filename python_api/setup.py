import setuptools

with open("../README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="mlmpy",
    version="0.0.9.5",
    author="Pol Monroig",
    author_email="polmonroig@gmail.com",
    description="mlm python API",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/polmonroig/mlm",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: Apache Software License",
        "Operating System :: OS Independent",
    ],
)