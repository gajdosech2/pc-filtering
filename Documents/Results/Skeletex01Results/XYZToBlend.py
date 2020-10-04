import bpy

def point_cloud(ob_name, coords, edges=[], faces=[]):
    """Create point cloud object based on given coordinates and name.

    Keyword arguments:
    ob_name -- new object name
    coords -- float triplets eg: [(-1.0, 1.0, 0.0), (-1.0, -1.0, 0.0)]
    """

    # Create new mesh and a new object
    me = bpy.data.meshes.new(ob_name + "Mesh")
    ob = bpy.data.objects.new(ob_name, me)

    # Make a mesh from a list of vertices/edges/faces
    me.from_pydata(coords, edges, faces)

    # Display name and update the mesh
    ob.show_name = True
    me.update()
    return ob


points = []

with open('/home/luk/Desktop/clean.xyz') as f:
  for line in f:
    s = line.split(' ')
    points.append((float(s[0]), float(s[1]), float(s[2][:-1])))

# Create the object
pc = point_cloud("point-cloud", points)

# Link object to the scene
bpy.context.scene.objects.link(pc)
