from setuptools import find_packages
from setuptools import setup

setup(
    name='screen_interfaces',
    version='0.0.0',
    packages=find_packages(
        include=('screen_interfaces', 'screen_interfaces.*')),
)
