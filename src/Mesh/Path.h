#pragma once
#include <vector>
struct Vertex;
class Path {
public:

	std::vector<Vertex> GetVertices() {
		return vertices;
	}
private:
	std::vector<Vertex> vertices;
};