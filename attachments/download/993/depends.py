#!/usr/bin/python

r"""depends.py - find the ordered dependancies of a library/executable (deepest first)

To use this:
	depends.py <pathtolibrary/pathtoexecutable>

"""
__author__	= "Robin Mills <robin@clanmills.com>"

import string
import os
import sys
from sets import Set

##
# recursively update dependdict for libname (which can be @executable_path etc)
# execdir is the directory of @executable_path
# loaddir is the directory of @loader_path
# returns path to libname or '' (empty string)
def depend(execdir,loaddir,libname,dependdict):
	""" recursive descent of the libraries """
	# print "depend: execdir=%s, loadpath=%s, libname=%s" % (execdir,loadpath,libname)

	##
	# discover libpath for libname
	epath='@executable_path/'
	lpath='@loader_path/'
	libpath=libname
	if libname[:len(epath)] == epath:
		libpath=os.path.join(execdir,libname[len(epath):])
	if libname[:len(lpath)] == lpath:
		libpath=os.path.join(loadpath,libname[len(lpath):])
	libpath=os.path.abspath(libpath)

	if os.path.isfile(libpath):
		if not dependdict.has_key(libpath):
			dependdict[libpath]=Set([])                 # push now to prevent infinite loop in recursion
			cmd = 'otool -L "%s"' % libpath         	# otool -L prints library dependancies:
														# libpath:
														# <tab>dependancy (metadata) ...
			for line in os.popen(cmd).readlines():      # run cmd
				if line[:1]=='\t':                      # parse <tab>dependancy (metadata)
					dependancy=line.split()[0]
					# print libpath,' => ',dependancy
					# recurse to find dependancies of dependancy
					dpath=depend(execdir,libpath,dependancy,dependdict)
					dependdict[libpath].add(dpath)      # update dependdict from recursion
	else:
		print "*** error NOT a FILE libname=%s libpath=%s ***" % (libname,libpath)
		libpath=''

	return libpath

##
#
def tsort(dependdict): # returns (ordered) array of libraries
	filename='/tmp/tsortXX.txt'
	file=open(filename,'w')
	for key in dependdict.keys():
		depends=dependdict[key]
		for d in depends:
			if len(d) > 0:
				file.write( '%s %s\n' % (key,d) )
	file.close()
	cmd='tsort "%s" 2>/dev/null' % (filename)
	lines=os.popen(cmd).readlines()
	lines.reverse()

	result=[]
	for line in lines:
		result.append(line[:-1])
	return result;
#
##

##
#
def main(argv):
	##
	# dependdict key is a library path.  Value is a set of dependant library paths
	dependdict = {}  # dependdict['/usr/lib/foo.dylib'] = Set([ '/usr/lib/a.dylib', ... ])
	libname    = argv[0]
	execdir    = os.path.abspath(os.path.join(libname,'..'))

	##
	# recursively build the dependency dictionary
	depend(execdir,execdir,libname,dependdict)

	##
	# sort and report
	results=tsort(dependdict)
	for result in results:
		print result
#
##

if __name__ == '__main__':
	main(sys.argv[1:])

# That's all Folks!
##
