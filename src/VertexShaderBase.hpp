#pragma once

#include "Vertex.hpp"
#include "VectorMath.hpp"



//
//						Vertex shader base class. 
//
//		Derive your own pixel shaders from this class and redefine the static
//		variables to match your vertex shader requirements.
// 
//		See Main.cpp for an example
//



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
