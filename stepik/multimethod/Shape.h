#ifndef MULTIMETHOD_SHAPE_H
#define MULTIMETHOD_SHAPE_H

#include <iostream>
#include <cassert>

struct Shape
{
    virtual std::string name() const = 0;
};

/*--------------------------------------------------------------------------------------------------------------------*/

struct Rectangle : Shape
{
    std::string name() const override { return "Rectangle"; }
};

/*--------------------------------------------------------------------------------------------------------------------*/

struct Triangle : Shape
{
    std::string name() const override { return "Triangle"; }
};

/*--------------------------------------------------------------------------------------------------------------------*/

// intersect between Rectangle and Rectangle
bool is_intersect_r_r ( Shape * a, Shape * b );

// intersect between Rectangle and Triangle
bool is_intersect_r_t ( Shape * a, Shape * b );

/*--------------------------------------------------------------------------------------------------------------------*/

// intersect between Triangle and Triangle
bool is_intersect_t_t ( Shape * a, Shape * b );

// intersect between Triangle and Rectangle
bool is_intersect_t_r ( Shape * a, Shape * b );

#endif // MULTIMETHOD_SHAPE_H //
