#pragma once

#include "ViewFrustumClipper.hpp"

ViewFrustumClipper::ViewFrustumClipper(std::vector<Vertex>* vertices, int i1, int i2, int i3, int varCount)
{
	m_IndicesIn = new std::vector<int>();
	m_IndicesOut = new std::vector<int>();
	m_Vertices = vertices;

	// Add the 3 original vertex indices
	this->m_IndicesIn->push_back(i1);
	this->m_IndicesIn->push_back(i2);
	this->m_IndicesIn->push_back(i3);

	this->m_VarCount = varCount;
}

ViewFrustumClipper::~ViewFrustumClipper()
{
	delete m_IndicesIn;
	delete m_IndicesOut;
}

std::vector<int>& ViewFrustumClipper::getIndices()
{
	return *m_IndicesIn;
}

Vertex ViewFrustumClipper::interpolateVertex(const Vertex &v0, const Vertex &v1, float t, int varCount)
{
	Vertex result;

	result.x = v0.x * (1.0f - t) + v1.x * t;
	result.y = v0.y * (1.0f - t) + v1.y * t;
	result.z = v0.z * (1.0f - t) + v1.z * t;
	result.w = v0.w * (1.0f - t) + v1.w * t;

	for (int i = 0; i < varCount; ++i)
		result.var[i] = v0.var[i] * (1.0f - t) + v1.var[i] * t;

	return result;
}

bool ViewFrustumClipper::fullyClipped() const
{
	return m_IndicesIn->size() < 3;
}

void ViewFrustumClipper::clipToPlane(float a, float b, float c, float d)
{
	// All vertices outside frustum, don't do anything
	if (fullyClipped())
		return;

	m_IndicesOut->clear();

	int prevIndex = (*m_IndicesIn)[0];
	m_IndicesIn->push_back(prevIndex);

	Vertex* prevVertex = &(*m_Vertices)[prevIndex];

	// Planar equation for previous vertex
	float dPrevPlane = a * prevVertex->x + b * prevVertex->y + c * prevVertex->z + d * prevVertex->w;

	for (int i = 1; i < m_IndicesIn->size(); i++)
	{
		int index = (*m_IndicesIn)[i];
		Vertex* vertex = &(*m_Vertices)[index];
		
		// Planar equation for current vertex
		float dPlane = a * vertex->x + b * vertex->y + c * vertex->z + d * vertex->w;

		// Vertex is on the left side (inside) of clip plane
		if (dPrevPlane >= 0)
			m_IndicesOut->push_back(prevIndex);

		// One triangle vertex is on the right side (outside) of clip plane
		if (sgn(dPlane) != sgn(dPrevPlane))
		{	
			// Which vertex is outside? 
			float t = dPlane < 0 ? dPrevPlane / (dPrevPlane - dPlane) : -dPrevPlane / (dPlane - dPrevPlane);

			// Create a new vertex on the boundary of the plane
			Vertex vertexOut = interpolateVertex(*prevVertex, *vertex, t, m_VarCount);

			m_Vertices->push_back(vertexOut);
			m_IndicesOut->push_back((int)(m_Vertices->size() - 1));
		}

		// Move previous vertex/index one iteration
		prevIndex = index;
		dPrevPlane = dPlane;
		prevVertex = &(*m_Vertices)[prevIndex];
	}

	// Replace with clipped indexList
	std::swap(m_IndicesIn, m_IndicesOut);
}