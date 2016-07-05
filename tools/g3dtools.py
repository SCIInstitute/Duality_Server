import sys
from g3d import *
import itertools

def grouper(n, iterable, fillvalue=None):
  "grouper(3, 'ABCDEFG', 'x') --> ABC DEF Gxx"
  args = [iter(iterable)] * n
  return itertools.izip_longest(fillvalue=fillvalue, *args)
	
def writeObj(file, indices, positions):
	for pos in positions:
		file.write('v {} {} {}\n'.format(pos[0], pos[1], pos[2]))
	for x, y, z in grouper(3, indices):
		file.write('f {} {} {}\n'.format(x, y, z))
	
def main():
	inFilename = sys.argv[1]
	g = readG3D(inFilename)
	
	writeG3D(g, 'test.g3d')
	
	#outFile = open(outFilename, 'w')
	#writeObj(outFile, indices, attributes['position'])
	
main()