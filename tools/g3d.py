import struct
from collections import namedtuple

G3DHeader = namedtuple('G3DHeader', 'isOpaque numberPrimitives primitiveType numberSemantics numberIndices indexSize numberVertices vertexSize vertexType attributeSemantics') 
G3DVertexAttribute = namedtuple('G3DVertexAttribute', 'name size')
vertexAttributeInfo = {
	0 : G3DVertexAttribute('position', 3),
	1 : G3DVertexAttribute('normal', 3),
	2 : G3DVertexAttribute('tangent', 3),
	3 : G3DVertexAttribute('color', 4),
	4 : G3DVertexAttribute('texture', 2),
	5 : G3DVertexAttribute('alpha', 1)}	

class g3d:
	def __init__(self, header, indices, attributes):
		self.header = header
		self.indices = indices
		self.attributes = attributes
		
def readHeader(file):
	format = '=c8I'
	data = file.read(struct.calcsize(format))
	dummy =  ([],) # dummy 1-element tuple containing empty list
	header = G3DHeader._make(struct.unpack_from(format, data) + dummy)
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

def readG3D(filename):
	file = open(filename, 'rb')
	header = readHeader(file)
	indices = readIndices(file, header.numberIndices, header.indexSize)
	attributes = readAttributes(file, header.numberVertices, header.attributeSemantics)
	return g3d(header, indices, attributes)

def writeHeader(file, header):
	format = '=c8I' + str(len(header.attributeSemantics)) + 'I'
	bin = struct.pack(format, *(list(header[:-1]) + header.attributeSemantics))
	file.write(bin)
	
def writeIndices(file, indices):
	format = '=' + str(len(indices)) + 'I'
	bin = struct.pack(format, *indices)
	file.write(bin)

def writeAttributes(file, attributes):
	for a in attributes.itervalues():
		for v in a:
			format = str(len(v)) + 'f'
			bin = struct.pack(format, *v)
			file.write(bin)

def writeG3D(g, filename):
	file = open(filename, 'wb')
	writeHeader(file, g.header)
	writeIndices(file, g.indices)
	writeAttributes(file, g.attributes)