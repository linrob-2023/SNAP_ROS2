from setuptools import find_packages
from setuptools import setup

setup(
    name='linrob_axis',
    version='1.1.15',
    packages=find_packages(
        include=('linrob_axis', 'linrob_axis.*')),
)
