from setuptools import find_packages, setup
import os 
from glob import glob

package_name = 'screen_images'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        # Include non-code directories in package share
        (os.path.join('share', package_name, 'logos'), glob('logos/*')),
        (os.path.join('share', package_name, 'test_images'), glob('test_images/*')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Amy Kibara',
    maintainer_email='amy.kibara@gmail.com',
    description='Package containing display nodes and images',
    license='Apache-2.0',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'stream_node = screen_images.stream_node:main',
            'screen_manager_node = screen_images.screen_manager_node:main',
        ],
    },
)
