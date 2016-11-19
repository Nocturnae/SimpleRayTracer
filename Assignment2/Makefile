SRC = Camera.cpp Color.cpp Image.cpp main.cpp Material.cpp Mesh.cpp Octree.cpp PointLight.cpp Ray.cpp Scene.cpp Sphere.cpp Texture.cpp Triangle.cpp Vector3.cpp Vertex.cpp Matrix.cpp

OBJ = $(SRC:.cpp=.o)
	CFLAGS = -std=c++11 -O3

.cpp.o:
	g++ -c $< $(CFLAGS)

all: test

aoa_jpeg.o: jpeg_reader/aoa_jpeg.c
			gcc -c $< -O3

test: $(OBJ) aoa_jpeg.o
			g++ $(OBJ) aoa_jpeg.o -o raytracer $(CPPFLAGS) -ljpeg

clean: 
	rm -f *.o raytracer *.ppm
