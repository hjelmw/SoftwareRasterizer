#pragma once

#include "TriangleEquations.hpp"
#include "FragmentData.hpp"

#pragma warning (disable: 6201 6294) 



//
//					Fragment shader base class. 
// 
//		Derive your own pixel shaders from this class and redefine the static
//		variables to match your pixel shader requirements.
// 
//		See Main.cpp for an example
//



template <class Derived>
class FragmentShaderBase {
public:
	/// Tells the rasterizer to interpolate the z component.
	static const int interpolateZ = false;

	/// Tells the rasterizer to interpolate the w component.
	static const int interpolateW = false;

	/// Tells the rasterizer how many per vertex vars to interpolate.
	static const int varCount = 3;

	/// This is called per pixel. 
	/** Implement this in your derived class to display single pixels. */
	static void processFragment(const FragmentData& p)
	{

	}

};

class NullFragmentShader : public FragmentShaderBase<NullFragmentShader> {};
