
#ifndef MESH_H_
#define MESH_H_

#include "VertexBuffer.h"
#include "Primitive.h"
#include "Debugging.h"
#include <stdio.h>

#define INFILE3  "pot4.asc"

typedef float   GzCoord[3];
typedef float	GzTextureIndex[2];

class Mesh
{
public:
	Mesh()	:	initialized(false)
	{
		if( (inFile  = fopen( INFILE3 , "r" )) == NULL )
		{
			DEBUG_PRINT( "The input file was not opened\n" );
		}
		buffer = new VertexBuffer(500);
	}

	void LoadMesh()
	{
		ALIGN float	vertexList[3][4];	/* vertex position coordinates */ 
		GzCoord		normalList[3];	/* vertex normals */ 
		GzTextureIndex  	uvList[3];
		
		vertexList[0][3] = 1.0f;
		vertexList[1][3] = 1.0f;
		vertexList[2][3] = 1.0f;

		while( fscanf(inFile, "%s", dummy) == 1) 
		{ 	/* read in tri word */
			fscanf(inFile, "%f %f %f %f %f %f %f %f", 
			&(vertexList[0][0]), &(vertexList[0][1]),  
			&(vertexList[0][2]), 
			&(normalList[0][0]), &(normalList[0][1]), 	
			&(normalList[0][2]), 
			&(uvList[0][0]), &(uvList[0][1]) ); 
			fscanf(inFile, "%f %f %f %f %f %f %f %f", 
			&(vertexList[1][0]), &(vertexList[1][1]), 	
			&(vertexList[1][2]), 
			&(normalList[1][0]), &(normalList[1][1]), 	
			&(normalList[1][2]), 
			&(uvList[1][0]), &(uvList[1][1]) ); 
			fscanf(inFile, "%f %f %f %f %f %f %f %f", 
			&(vertexList[2][0]), &(vertexList[2][1]), 	
			&(vertexList[2][2]), 
			&(normalList[2][0]), &(normalList[2][1]), 	
			&(normalList[2][2]), 
			&(uvList[2][0]), &(uvList[2][1]) );

			Math::Vector4 rows[3];
			rows[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
			rows[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
			rows[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);
			PrimitiveBase* toAdd = new Primitive<Math::Vector4,3>(rows);
			buffer->PushBack(toAdd);
		}
	}

	VertexBuffer* GetBuffer()
	{
		return buffer;
	}

private:
	bool initialized;
	VertexBuffer* buffer;
	GzCoord		vertexList[3];	/* vertex position coordinates */ 
	GzCoord		normalList[3];	/* vertex normals */ 
	GzTextureIndex  	uvList[3];		/* vertex texture map indices */ 
	char		dummy[256];
	FILE* inFile;
};

#endif