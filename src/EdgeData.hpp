#pragma once

#include "TriangleEquations.hpp"

class EdgeData
{
public:
    float ev0;
    float ev1;
    float ev2;

    /// Initialize the edge data values.
    void init(const TriangleEquations& teqn, float x, float y);

    /// Step the edge values in the x direction.
    void stepX(const TriangleEquations& teqn);

    /// Step the edge values in the x direction.
    void stepX(const TriangleEquations& teqn, float stepSize);

    /// Step the edge values in the y direction.
    void stepY(const TriangleEquations& teqn);

    /// Step the edge values in the y direction.
    void stepY(const TriangleEquations& teqn, float stepSize);

    /// Test for triangle containment.
    bool test(const TriangleEquations& teqn);
};