import time
from PIL import Image, ImageDraw
from renderer.vec3 import Vec3
from renderer.camera import Camera, Screen
from renderer.polygon import Polygon
from renderer.mesh import Mesh
from renderer.object import Object
from renderer.rasterize import render

# TODO: trace your steps carefully through the render pipeline and figure out what is breaking.

dim = (500, 500)

# Use PIL as render medium
img = Image.new("RGB", dim)
img_draw = ImageDraw.Draw(img)



cam = Camera(Vec3(0.0,0.0,0.0), *dim, 200)
screen = Screen(500, 500)
print("load mesh")
teapot = Object(Mesh.from_obj("meshes\pirate_ship\pirate_ship.obj"),
    Vec3(0.0,-70.0,140), Vec3(0,0,0))
print("Loaded mesh!")

#create gif
frames = []

t1 = time.time()
cppt1 = time.time()
teapot.rotation.y += 0.1
teapot.render(cam, screen)
cppt2 = time.time()
print(f"cpp time to render: {(cppt2-cppt1) * 1000:0.4f} ms")
render(img_draw, cam)
t2 = time.time()
print(f"time to render: {(t2-t1) * 1000:0.4f} ms")
frames.append(img.copy())
img_draw.rectangle((0,0,dim[0]-1,dim[1]-1), "black")

for i in range(60):
    t1 = time.time()
    cppt1 = time.time()
    teapot.rotation.y += 0.1
    teapot.render(cam, screen)
    cppt2 = time.time()
    print(f"cpp time to render: {(cppt2-cppt1) * 1000:0.4f} ms")
    render(img_draw, cam)
    t2 = time.time()
    print(f"time to render: {(t2-t1) * 1000:0.4f} ms")
    frames.append(img.copy())
    img_draw.rectangle((0,0,dim[0]-1,dim[1]-1), "black")

t1 = time.time()
cppt1 = time.time()
teapot.rotation.y += 0.1
teapot.render(cam, screen)
cppt2 = time.time()
print(f"cpp time to render: {(cppt2-cppt1) * 1000:0.4f} ms")
render(img_draw, cam)
t2 = time.time()
print(f"time to render: {(t2-t1) * 1000:0.4f} ms")
frames.append(img.copy())

img.save("result.gif", save_all = True, append_images = frames, duration = 5, loop = 0)