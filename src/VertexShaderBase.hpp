#pragma once

#include "Vertex.hpp"
#include "VectorMath.hpp"

/// Base class for vertex shaders.

/** Derive your own vertex shaders from this class and redefine attribCount etc. */

template <class Derived>
class VertexShaderBase {
public:
	/// Number of vertex attribute pointers this vertex shader uses.
	static const int attribCount = 0;

	/// Number of output variables.
	static const int varCount = 0;

	/// Process a single vertex.
	/** Implement this in your own vertex shader. */
	static void processVertex(void* in[], Vertex out)
	{

	}
};

class NullVertexShader : public VertexShaderBase<NullVertexShader> {};
