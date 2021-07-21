#include "EdgeData.hpp"

/// Initialize the edge data values.
EdgeData::EdgeData(const TriangleEquations& teqn, float x, float y)
{
    this->ev0 = teqn.e0.evaluate(x, y);
    this->ev1 = teqn.e1.evaluate(x, y);
    this->ev2 = teqn.e2.evaluate(x, y);
}

/// Step the edge values in the x direction.
void EdgeData::stepX(const TriangleEquations& teqn)
{
    this->ev0 = teqn.e0.stepX(ev0);
    this->ev1 = teqn.e1.stepX(ev1);
    this->ev2 = teqn.e2.stepX(ev2);
}

/// Step the edge values in the x direction.
void EdgeData::stepX(const TriangleEquations& teqn, float stepSize)
{
    this->ev0 = teqn.e0.stepX(ev0, stepSize);
    this->ev1 = teqn.e1.stepX(ev1, stepSize);
    this->ev2 = teqn.e2.stepX(ev2, stepSize);
}

/// Step the edge values in the y direction.
void EdgeData::stepY(const TriangleEquations& teqn)
{
    this->ev0 = teqn.e0.stepY(ev0);
    this->ev1 = teqn.e1.stepY(ev1);
    this->ev2 = teqn.e2.stepY(ev2);
}

/// Step the edge values in the y direction.
void EdgeData::stepY(const TriangleEquations& teqn, float stepSize)
{
    this->ev0 = teqn.e0.stepY(ev0, stepSize);
    this->ev1 = teqn.e1.stepY(ev1, stepSize);
    this->ev2 = teqn.e2.stepY(ev2, stepSize);
}

/// Test for triangle containment.
bool EdgeData::test(const TriangleEquations& teqn)
{
    return teqn.e0.test(this->ev0) && teqn.e1.test(this->ev1) && teqn.e2.test(this->ev2);
}