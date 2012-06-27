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
	group = opt.add_option_group(name + ' specific options')
	group.add_option('-d', '--debug', dest='debug', default=False, action='store_true', help='enables debug build (used by developers)')
	group.add_option('-r', '--release', dest='release', default=False, action='store_true', help='enables optimized release build')
	group.add_option('-s', '--sanitizer', dest='sanitizer', default=False, action='store_true', help='Special (slow) build using clang AdressSanitizer')

def configure(conf):
	conf.load('compiler_cxx')
	
	conf.env.NAME = name
	
	# collect compiler flag sets
	flags_cpp11 = True
	flags_debug = False
	flags_nice_output = True
	flags_optimize = False
	flags_sanitizer = False
	flags_warnings = False
	if conf.options.debug:
		flags_debug = True
		flags_warnings = True
	if conf.options.release:
		flags_optimize = True
	if conf.options.sanitizer:
		flags_debug = True
		flags_optimize = True
		flags_sanitizer = True
	
	# check/add compiler flags
	if flags_cpp11:
		conf.check_cxx(cxxflags='-std=c++0x', uselib_store='DEFAULT')
	if flags_debug:
		conf.check_cxx(cxxflags='-g', uselib_store='DEFAULT')
		conf.check_cxx(cxxflags='-fno-omit-frame-pointer', uselib_store='DEFAULT')
		conf.check_cxx(cxxflags='-fno-optimize-sibling-calls', uselib_store='DEFAULT')
	if flags_nice_output:
		conf.check_cxx(cxxflags='-fcolor-diagnostics', uselib_store='DEFAULT')
		conf.check_cxx(cxxflags='-fdiagnostics-show-category=name', uselib_store='DEFAULT')
	if flags_optimize:
		conf.check_cxx(cxxflags='-O2', uselib_store='DEFAULT')
	if flags_sanitizer:
		conf.check_cxx(cxxflags='-faddress-sanitizer', uselib_store='DEFAULT')
	if flags_warnings:
		conf.check_cxx(cxxflags='-Wall', uselib_store='DEFAULT')
		conf.check_cxx(cxxflags='-Wextra', uselib_store='DEFAULT')
	conf.env.append_unique('CXXFLAGS', conf.env.CXXFLAGS_DEFAULT)
	
	# check stdlib headers
	stdheaders = ['atomic', 'chrono', 'forward_list', 'functional', 'iostream', 'memory', 'mutex', 'queue', 'sstream', 'stdexcept', 'string', 'thread', 'unordered_map', 'utility']
	for header in stdheaders:
		conf.check_cxx(header_name=header, cxxflags = conf.env.CXXFLAGS)
	
	# config extern libs
	conf.recurse('extern')
	
	# check used libs
	stlibs = conf.env.STLIB
	libs = conf.env.LIB
	conf.env.STLIB = []
	conf.env.LIB = []
	for stlibname in stlibs:
		conf.check_cxx(stlib=stlibname, cxxflags = conf.env.CXXFLAGS)
	for libname in libs:
		conf.check_cxx(lib=libname, cxxflags = conf.env.CXXFLAGS)
	conf.env.append_unique('STLIB', stlibs)
	conf.env.append_unique('LIB', libs)
	
	# config sources
	conf.recurse('source')
	
	# get build informations
	conf.define('REVISION', get_git_rev())
	conf.define('BUILDTIME', datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
	conf.define('NAME', conf.env.NAME)
	
	# store config
	conf.env.append_unique('INCLUDES', conf.path.find_dir(out).abspath())
	conf.write_config_header('config.h')

def build(bld):
	# build extern libs
	bld.recurse('extern')
	
	# build program
	bld.recurse('source')

def get_git_rev():
	try:
		p = subprocess.Popen(['git rev-parse HEAD'], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=False, shell=True)
		stdout = p.communicate()[0]
		
		if p.returncode == 0:
			stdout_string = str(stdout.decode(sys.stdout.encoding))
			lines = stdout_string.splitlines()
			return lines[0]
		else:
			return 'n/a'
	except:
		return 'n/a'
