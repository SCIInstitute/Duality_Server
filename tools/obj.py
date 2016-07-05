class obj:
	def __init__(self, vertices, normals, faces):
		self.vertices = vertices
		self.normals = normals
		self.faces = faces

def writeObj(filename, obj):
	file = open(filename, 'w')
	for v in obj.vertices:
		file.write('v {} {} {}\n'.format(v[0], v[1], v[2]))
	for n in obj.normals:
		file.write('vn {} {} {}\n'.format(n[0], n[1], n[2]))
	for f in obj.faces:
		file.write('f {} {} {}\n'.format(f[0], f[1], f[2]))
		
def readObj(filename):
	with open(filename) as file:
		lines = file.readlines()
	vertices = []
	normals = []
	faces = []
	for line in lines:
		s = line.split()
		if len(s) ==  0:
			continue
		type = s[0]
		if type == '#':
			continue
		if type == 'v':
			vertices.append([float(s[1]), float(s[2]), float(s[3])])
		if type == 'vn':
			normals.append([float(s[1]), float(s[2]), float(s[3])])
		if type == 'f':
			p = []
			for j in range(1, 4):
				fs = s[j].split('//')
				p.append(int(fs[0])) # assume that indices for position and normal are the same
			faces.append(p)			
	return obj(vertices, normals, faces)