from setuptools import find_packages
from setuptools import setup

setup(
    name='linrob_axis',
    version='0.1.2',
    packages=find_packages(
        include=('linrob_axis', 'linrob_axis.*')),
)
