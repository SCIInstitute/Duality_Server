import sys
from g3d import *
from obj import *
import itertools

def grouper(n, iterable, fillvalue=None):
  "grouper(3, 'ABCDEFG', 'x') --> ABC DEF Gxx"
  args = [iter(iterable)] * n
  return itertools.izip_longest(fillvalue=fillvalue, *args)
	
def g3dToObj(inFile, outFile):
	g = readG3D(inFile)
	vertices = g.attributes['position']
	normals = g.attributes['normal']
	faces = []
	for x,y,z in grouper(3, g.indices):
		faces.append([x,y,z])
	o = obj(vertices, faces)
	writeObj(outFile, o)

def objToG3D(inFile, outFile, color):
	o = readObj(inFile)
	header = G3DHeader(chr(0), len(o.faces), 2, 3, len(o.faces) * 3, 4, len(o.vertices), 40, 0, [0, 1, 3])
	indices = []
	attributes = {}
	attributes['position'] = []
	attributes['normal'] = []
	attributes['color'] = []
	for f in o.faces:
		indices.extend(f)
	for v in o.vertices:
		attributes['position'].append(v)
	for n in o.normals:
		attributes['normal'].append(n)
	for i in range(0, len(o.vertices)):
		attributes['color'].append(color)
	g = g3d(header, indices, attributes)
	writeG3D(g, outFile)
	
def main():
	inFile = sys.argv[2]
	outFile = sys.argv[3]
	if (sys.argv[1] == 'g2o'):
		g3dToObj(inFile, outFile)
	elif (sys.argv[1] == 'o2g'):
		objToG3D(inFile, outFile, [float(sys.argv[4]), float(sys.argv[5]), float(sys.argv[6]), float(sys.argv[7])])

main()