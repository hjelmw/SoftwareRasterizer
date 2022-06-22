#pragma once

#include <vector>
#include "Vertex.hpp"



//
//							View Frustum culling.
//		Clips triangles against a given plane in view frustum 
//		to either completely cull or build a new vertex that is inside it
// 
//		Heavily inspired by code from Markus Trenkwalder's (Trenki) rasterizer dev blog and 
//		uses the Sutherland-Hodgeman algorithm https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm
//



class ViewFrustumClipper
{
private:

	std::vector<int>* m_IndicesIn;
	std::vector<int>* m_IndicesOut;
	std::vector<Vertex> *m_Vertices;
	int m_VarCount;
	
	// Signum function. returns -1 for negative numbers, +1 for positive.
	// Source from SO https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
	template <typename T> int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

public:
	ViewFrustumClipper(std::vector<Vertex>* vertices, int i1, int i2, int i3, int varCount);
	~ViewFrustumClipper();

	std::vector<int>& getIndices();

	// Clip the polygon to the plane given by the formula a * x + b * y + c * z + d * w
	void clipToPlane(float a, float b, float c, float d);

	// Polygon is completely outside view frustum plane
	bool fullyClipped() const;

	// Given two vertices. Return a new one with values in between them
	static Vertex interpolateVertex(const Vertex& v0, const Vertex& v1, float t, int varCount);
};