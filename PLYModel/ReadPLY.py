from plyfile import PlyData, PlyElement

plydata = PlyData.read('scan_000.ply')
print(len(plydata.elements))
print(len(plydata['vertex']['x']))

print(plydata['vertex'][0])
print(type(plydata['vertex'][0]))
xyz = list()
for i, v in enumerate(plydata['vertex']):
    if v['x'] != 0:
        xyz.append((v['x'], v['y'], v['z']))

print(len(xyz))
print(xyz[0:5])
