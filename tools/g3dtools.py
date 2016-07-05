import sys
import struct
from collections import namedtuple
import itertools

G3DHeader = namedtuple('G3DHeader', 'isOpaque numberPrimitives primitiveType numberSemantics numberIndices indexSize numberVertices vertexSize vertexType attributeSemantics') 
G3DVertexAttribute = namedtuple('G3DVertexAttribute', 'name size')
vertexAttributeInfo = {
	0 : G3DVertexAttribute('position', 3),
	1 : G3DVertexAttribute('normal', 3),
	2 : G3DVertexAttribute('tangent', 3),
	3 : G3DVertexAttribute('color', 4),
	4 : G3DVertexAttribute('texture', 2),
	5 : G3DVertexAttribute('alpha', 1)}	

def readHeader(file):
	G3DHeaderFormat = '=c8I'
	data = file.read(struct.calcsize(G3DHeaderFormat))
	dummy =  ([],) # dummy 1-element tuple containing empty list
	header = G3DHeader._make(struct.unpack_from(G3DHeaderFormat, data) + dummy)
	for i in range(0, header.numberSemantics):
		header.attributeSemantics.append(struct.unpack("=I", file.read(4))[0])
	return header
	
def readIndices(file, numberIndices, indexSize):
	indices = []
	for i in range(0, numberIndices):
		index = struct.unpack_from('=I', file.read(indexSize))[0]
		indices.append(index)
	return indices
	
def readAttributes(file, numberVertices, attributeSemantics):
	attributes = {}
	for i in attributeSemantics:
		name = vertexAttributeInfo[i].name
		size = vertexAttributeInfo[i].size
		attributes[name] = []
		for v in range(0, numberVertices):
			attributes[name].append([])
			for s in range(0,size):
				attribute = struct.unpack_from('f', file.read(4))[0]
				attributes[name][v].append(attribute)
	return attributes

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
	outFilename = sys.argv[2]
	inFile = open(inFilename, 'rb')
	header = readHeader(inFile)
	print header
	indices = readIndices(inFile, header.numberIndices, header.indexSize)
	#print indices
	attributes = readAttributes(inFile, header.numberVertices, header.attributeSemantics)
	#print attributes
	
	outFile = open(outFilename, 'w')
	writeObj(outFile, indices, attributes['position'])
	
main()