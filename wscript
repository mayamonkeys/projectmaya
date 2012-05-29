#! /usr/bin/env python
# encoding: utf-8

import datetime
import subprocess
import sys

top = '.'
out = 'build'

name = 'Project Maya'

def options(opt):
	opt.load('compiler_cxx')

def configure(conf):
	conf.load('compiler_cxx')
	
	conf.env.NAME = name
	
	# check compiler flags
	conf.check_cxx(cxxflags='-Wall', uselib_store='DEFAULT')
	conf.check_cxx(cxxflags='-std=c++0x', uselib_store='DEFAULT')
	conf.env.append_unique('CXXFLAGS', conf.env.CXXFLAGS_DEFAULT)
	
	# check stdlib headers
	conf.check_cxx(header_name='thread', cxxflags = conf.env.CXXFLAGS)
	
	# config libs
	conf.recurse('lib')
	
	# check used libs
	for stlibname in conf.env.STLIB:
		conf.check_cxx(stlib=stlibname, cxxflags = conf.env.CXXFLAGS)
	for libname in conf.env.LIB:
		conf.check_cxx(lib=libname, cxxflags = conf.env.CXXFLAGS)
	
	# get build informations
	conf.define('REVISION', get_git_rev())
	conf.define('BUILDTIME', datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
	conf.define('NAME', conf.env.NAME)
	
	# store config
	conf.env.append_unique('INCLUDES', conf.path.find_dir(out).abspath())
	conf.write_config_header('config.h')

def build(bld):
	# build libs
	bld.recurse('lib')
	
	# build program
	bld.recurse('src')

def get_git_rev():
	try:
		p = subprocess.Popen(['git', 'rev-parse', 'HEAD'], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=False, shell=True)
		stdout = p.communicate()[0]
		
		if p.returncode == 0:
			stdout_string = str(stdout, sys.stdout.encoding)
			lines = stdout_string.splitlines()
			return lines[0]
		else:
			return 'n/a'
	except:
		return 'n/a'
