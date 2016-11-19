SRC = Camera.cpp Color.cpp Image.cpp main.cpp Material.cpp Mesh.cpp Octree.cpp PointLight.cpp Ray.cpp Scene.cpp Sphere.cpp Texture.cpp Triangle.cpp Vector3.cpp Vertex.cpp Matrix.cpp

OBJ = $(SRC:.cpp=.o)
	CFLAGS = -std=c++11 -O3

.cpp.o:
		g++ -c $< $(CFLAGS)

all: test

test: $(OBJ)
		g++ $(OBJ) -o raytracer $(CFLAGS)

clean:
		rm -f *.o raytracer *.ppm
