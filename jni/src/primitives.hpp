//
//  primitives.hpp
//  aldebaran
//
//  Created by David Siaw on 2014/10/19.
//  Copyright (c) 2014年 David Siaw. All rights reserved.
//

#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#pragma pack(push,1)
struct Vertex
{
    GLfloat x,y,z;
};

struct Color
{
    uint8_t r,g,b,a;
};

struct TexCoord
{
    GLfloat x,y;
};

struct Element
{
    Vertex v;
    Color c;
    TexCoord t;
    float tileOffset;	// The width of each tile
    float numtiles;		// Number of tiles in texture
};

struct Triangle
{
    Element e[3];
};

#pragma pack(pop)

#endif // PRIMITIVES_HPP
