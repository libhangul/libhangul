#!/usr/bin/env python

import sys
from distutils.core import setup, Extension 

classifiers = """\
Development Status :: 5 - Production/Stable
Intended Audience :: Developers
License :: OSI Approved :: GNU GPL
Programming Language :: Python
Programming Language :: C
Topic :: Software Development :: Libraries :: Python Modules
Operating System :: Microsoft :: Windows
Operating System :: Unix
"""
 
if sys.platform == "win32": # for MinGW
	include_dirs = [r'\MinGW\include', r'..\hangul']
	library_dirs = [r'\MinGW\lib', r'..\hangul']
	libraries = ['hangul']
	data_files = []
	
else:
	include_dirs = [
		'../hangul',
		'/usr/include',
		'/usr/local/include'
	]
	library_dirs = [
		'../hangul',
		'/usr/lib',
		'/usr/local/lib',
	]
	libraries = ['hangul']
	data_files = []

if sys.version_info < (2, 3):
	_setup = setup
	def setup(**kwargs):
		if kwargs.has_key("classifiers"): 
			del kwargs["classifiers"]
			_setup(**kwargs)

setup(name = "pyhangul", 
	version = "0.0.1", 
	description="libhangul for Python.",
	author = "Joon-cheol Park", 
	author_email="jooncheol@gmail.com", 
	license = "GPL",
	url="http://hangul.kldp.net", 
	ext_modules=[Extension("hangul", ["pyhangul.c"],
                                include_dirs = include_dirs,
                                library_dirs = library_dirs,
                                libraries = libraries)],
	classifiers = filter(None, classifiers.split("\n")),
	data_files=data_files
	) 
 
