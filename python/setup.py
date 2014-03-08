from distutils.core import setup, Extension
setup(name='helloworld', ext_modules=[ Extension('helloworld', sources=['helloworld.c'])])
