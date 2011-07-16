
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
	Mesh()	:	initialized(false),
				buffer(1000)
	{
		if( (inFile  = fopen( INFILE3 , "r" )) == NULL )
		{
			DEBUG_PRINT( "The input file was not opened\n" );
		}
	}

	void LoadTestCube()
	{
		ALIGN float	vertexList[3][4];	/* vertex position coordinates */ 
		ALIGN float	normalList[3][4];
		Math::Vector4 rowsVerts[3],rowsNormals[3];
		//PrimitiveBase* toAdd;

		vertexList[0][3] = 1.0f;
		vertexList[1][3] = 1.0f;
		vertexList[2][3] = 1.0f;

		normalList[0][3] = 0.0f;
		normalList[1][3] = 0.0f;
		normalList[2][3] = 0.0f;

	
        // front
        vertexList[0][0] = -0.5f; vertexList[0][1] = 0.5f; vertexList[0][2] = 0.5f;
		vertexList[1][0] = -0.5f; vertexList[1][1] = -0.5f; vertexList[1][2] = 0.5f;
		vertexList[2][0] = 0.5f; vertexList[2][1] = -0.5f; vertexList[2][2] = 0.5f;

		normalList[0][0] = 0.0f; normalList[0][1] = 0.0f; normalList[0][2] =  1.0f;
		normalList[1][0] = 0.0f; normalList[1][1] = 0.0f; normalList[1][2] =  1.0f;
		normalList[2][0] = 0.0f; normalList[2][1] = 0.0f; normalList[2][2] =  1.0f;
					
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);
		rowsNormals[0] = Math::LoadVector4Aligned((float*)&normalList[0]);
		rowsNormals[1] = Math::LoadVector4Aligned((float*)&normalList[1]);
		rowsNormals[2] = Math::LoadVector4Aligned((float*)&normalList[2]);
		Primitive<Math::Vector4> toAdd(rowsVerts,rowsNormals);
		buffer.PushBack(std::forward<Primitive<>>(toAdd));

		vertexList[0][0] = 0.5f; vertexList[0][1] = 0.5f; vertexList[0][2] = 0.5f;
		vertexList[1][0] = -0.5f; vertexList[1][1] = 0.5f; vertexList[1][2] = 0.5f;
		vertexList[2][0] = 0.5f; vertexList[2][1] = -0.5f; vertexList[2][2] = 0.5f;

		normalList[0][0] = 0.0f; normalList[0][1] = 0.0f; normalList[0][2] =  1.0f;
		normalList[1][0] = 0.0f; normalList[1][1] = 0.0f; normalList[1][2] =  1.0f;
		normalList[2][0] = 0.0f; normalList[2][1] = 0.0f; normalList[2][2] =  1.0f;
					
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);
		rowsNormals[0] = Math::LoadVector4Aligned((float*)&normalList[0]);
		rowsNormals[1] = Math::LoadVector4Aligned((float*)&normalList[1]);
		rowsNormals[2] = Math::LoadVector4Aligned((float*)&normalList[2]);
		Primitive<Math::Vector4> toAdd2(rowsVerts,rowsNormals);
		buffer.PushBack(std::forward<Primitive<>>(toAdd2));

		vertexList[0][0] = -0.5f; vertexList[0][1] = 0.5f; vertexList[0][2] = -0.5f;
		vertexList[1][0] = -0.5f; vertexList[1][1] = 0.5f; vertexList[1][2] = 0.5f;
		vertexList[2][0] = 0.5f; vertexList[2][1] = 0.5f; vertexList[2][2] = 0.5f;

		normalList[0][0] = 0.0f; normalList[0][1] = 1.0f; normalList[0][2] =  0.0f;
		normalList[1][0] = 0.0f; normalList[1][1] = 1.0f; normalList[1][2] =  0.0f;
		normalList[2][0] = 0.0f; normalList[2][1] = 1.0f; normalList[2][2] =  0.0f;
					
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);
		rowsNormals[0] = Math::LoadVector4Aligned((float*)&normalList[0]);
		rowsNormals[1] = Math::LoadVector4Aligned((float*)&normalList[1]);
		rowsNormals[2] = Math::LoadVector4Aligned((float*)&normalList[2]);
		Primitive<Math::Vector4> toAdd3(rowsVerts,rowsNormals);
		buffer.PushBack(std::forward<Primitive<>>(toAdd3));

		vertexList[0][0] = 0.5f; vertexList[0][1] = 0.5f; vertexList[0][2] = -0.5f;
		vertexList[1][0] = -0.5f; vertexList[1][1] = 0.5f; vertexList[1][2] = -0.5f;
		vertexList[2][0] = 0.5f; vertexList[2][1] = 0.5f; vertexList[2][2] = 0.5f;

		normalList[0][0] = 0.0f; normalList[0][1] = 1.0f; normalList[0][2] =  0.0f;
		normalList[1][0] = 0.0f; normalList[1][1] = 1.0f; normalList[1][2] =  0.0f;
		normalList[2][0] = 0.0f; normalList[2][1] = 1.0f; normalList[2][2] =  0.0f;
					
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);
		rowsNormals[0] = Math::LoadVector4Aligned((float*)&normalList[0]);
		rowsNormals[1] = Math::LoadVector4Aligned((float*)&normalList[1]);
		rowsNormals[2] = Math::LoadVector4Aligned((float*)&normalList[2]);
		Primitive<Math::Vector4> toAdd4(rowsVerts,rowsNormals);
		buffer.PushBack(std::forward<Primitive<>>(toAdd4));

		vertexList[0][0] = 0.5f; vertexList[0][1] = 0.5f; vertexList[0][2] = 0.5f;
		vertexList[1][0] = 0.5f; vertexList[1][1] = -0.5f; vertexList[1][2] = 0.5f;
		vertexList[2][0] = 0.5f; vertexList[2][1] = -0.5f; vertexList[2][2] = -0.5f;

		normalList[0][0] = 1.0f; normalList[0][1] = 0.0f; normalList[0][2] =  0.0f;
		normalList[1][0] = 1.0f; normalList[1][1] = 0.0f; normalList[1][2] =  0.0f;
		normalList[2][0] = 1.0f; normalList[2][1] = 0.0f; normalList[2][2] =  0.0f;
					
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);
		rowsNormals[0] = Math::LoadVector4Aligned((float*)&normalList[0]);
		rowsNormals[1] = Math::LoadVector4Aligned((float*)&normalList[1]);
		rowsNormals[2] = Math::LoadVector4Aligned((float*)&normalList[2]);
		Primitive<Math::Vector4> toAdd5(rowsVerts,rowsNormals);
		buffer.PushBack(std::forward<Primitive<>>(toAdd5));

		vertexList[0][0] = 0.5f; vertexList[0][1] = 0.5f; vertexList[0][2] = 0.5f;
		vertexList[1][0] = 0.5f; vertexList[1][1] = -0.5f; vertexList[1][2] = -0.5f;
		vertexList[2][0] = 0.5f; vertexList[2][1] = 0.5f; vertexList[2][2] = -0.5f;

		normalList[0][0] = 1.0f; normalList[0][1] = 0.0f; normalList[0][2] =  0.0f;
		normalList[1][0] = 1.0f; normalList[1][1] = 0.0f; normalList[1][2] =  0.0f;
		normalList[2][0] = 1.0f; normalList[2][1] = 0.0f; normalList[2][2] =  0.0f;
					
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);
		rowsNormals[0] = Math::LoadVector4Aligned((float*)&normalList[0]);
		rowsNormals[1] = Math::LoadVector4Aligned((float*)&normalList[1]);
		rowsNormals[2] = Math::LoadVector4Aligned((float*)&normalList[2]);
		Primitive<Math::Vector4> toAdd6(rowsVerts,rowsNormals);
		buffer.PushBack(std::forward<Primitive<>>(toAdd6));

		vertexList[0][0] = -0.5f; vertexList[0][1] = -0.5f; vertexList[0][2] = 0.5f;
		vertexList[1][0] = -0.5f; vertexList[1][1] = 0.5f; vertexList[1][2] = 0.5f;
		vertexList[2][0] = -0.5f; vertexList[2][1] = -0.5f; vertexList[2][2] = -0.5f;

		normalList[0][0] = 1.0f; normalList[0][1] = 0.0f; normalList[0][2] =  0.0f;
		normalList[1][0] = 1.0f; normalList[1][1] = 0.0f; normalList[1][2] =  0.0f;
		normalList[2][0] = 1.0f; normalList[2][1] = 0.0f; normalList[2][2] =  0.0f;
					
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);
		rowsNormals[0] = Math::LoadVector4Aligned((float*)&normalList[0]);
		rowsNormals[1] = Math::LoadVector4Aligned((float*)&normalList[1]);
		rowsNormals[2] = Math::LoadVector4Aligned((float*)&normalList[2]);
		Primitive<Math::Vector4> toAdd7(rowsVerts,rowsNormals);
		buffer.PushBack(std::forward<Primitive<>>(toAdd7));

		vertexList[0][0] = -0.5f; vertexList[0][1] = -0.5f; vertexList[0][2] = -0.5f;
		vertexList[1][0] = -0.5f; vertexList[1][1] = 0.5f; vertexList[1][2] = 0.5f;
		vertexList[2][0] = -0.5f; vertexList[2][1] = 0.5f; vertexList[2][2] = -0.5f;

		normalList[0][0] = 1.0f; normalList[0][1] = 0.0f; normalList[0][2] =  0.0f;
		normalList[1][0] = 1.0f; normalList[1][1] = 0.0f; normalList[1][2] =  0.0f;
		normalList[2][0] = 1.0f; normalList[2][1] = 0.0f; normalList[2][2] =  0.0f;
					
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);
		rowsNormals[0] = Math::LoadVector4Aligned((float*)&normalList[0]);
		rowsNormals[1] = Math::LoadVector4Aligned((float*)&normalList[1]);
		rowsNormals[2] = Math::LoadVector4Aligned((float*)&normalList[2]);
		Primitive<Math::Vector4> toAdd8(rowsVerts,rowsNormals);
		buffer.PushBack(std::forward<Primitive<>>(toAdd8));

		vertexList[0][0] = -0.5f; vertexList[0][1] = -0.5f; vertexList[0][2] = 0.5f;
		vertexList[1][0] = -0.5f; vertexList[1][1] = -0.5f; vertexList[1][2] = -0.5f;
		vertexList[2][0] = 0.5f; vertexList[2][1] = -0.5f; vertexList[2][2] = 0.5f;

		normalList[0][0] = 0.0f; normalList[0][1] = 1.0f; normalList[0][2] =  0.0f;
		normalList[1][0] = 0.0f; normalList[1][1] = 1.0f; normalList[1][2] =  0.0f;
		normalList[2][0] = 0.0f; normalList[2][1] = 1.0f; normalList[2][2] =  0.0f;
					
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);
		rowsNormals[0] = Math::LoadVector4Aligned((float*)&normalList[0]);
		rowsNormals[1] = Math::LoadVector4Aligned((float*)&normalList[1]);
		rowsNormals[2] = Math::LoadVector4Aligned((float*)&normalList[2]);
		Primitive<Math::Vector4> toAdd9(rowsVerts,rowsNormals);
		buffer.PushBack(std::forward<Primitive<>>(toAdd9));

		vertexList[0][0] = -0.5f; vertexList[0][1] = -0.5f; vertexList[0][2] = -0.5f;
		vertexList[1][0] = 0.5f; vertexList[1][1] = -0.5f; vertexList[1][2] = -0.5f;
		vertexList[2][0] = 0.5f; vertexList[2][1] = -0.5f; vertexList[2][2] = 0.5f;

		normalList[0][0] = 0.0f; normalList[0][1] = 1.0f; normalList[0][2] =  0.0f;
		normalList[1][0] = 0.0f; normalList[1][1] = 1.0f; normalList[1][2] =  0.0f;
		normalList[2][0] = 0.0f; normalList[2][1] = 1.0f; normalList[2][2] =  0.0f;
					
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);
		rowsNormals[0] = Math::LoadVector4Aligned((float*)&normalList[0]);
		rowsNormals[1] = Math::LoadVector4Aligned((float*)&normalList[1]);
		rowsNormals[2] = Math::LoadVector4Aligned((float*)&normalList[2]);
		Primitive<Math::Vector4> toAdd10(rowsVerts,rowsNormals);
		buffer.PushBack(std::forward<Primitive<>>(toAdd10));

		vertexList[0][0] = -0.5f; vertexList[0][1] = -0.5f; vertexList[0][2] = -0.5f;
		vertexList[1][0] = -0.5f; vertexList[1][1] = 0.5f; vertexList[1][2] = -0.5f;
		vertexList[2][0] = 0.5f; vertexList[2][1] = -0.5f; vertexList[2][2] = -0.5f;

		normalList[0][0] = 0.0f; normalList[0][1] = 0.0f; normalList[0][2] =  1.0f;
		normalList[1][0] = 0.0f; normalList[1][1] = 0.0f; normalList[1][2] =  1.0f;
		normalList[2][0] = 0.0f; normalList[2][1] = 0.0f; normalList[2][2] =  1.0f;
					
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);
		rowsNormals[0] = Math::LoadVector4Aligned((float*)&normalList[0]);
		rowsNormals[1] = Math::LoadVector4Aligned((float*)&normalList[1]);
		rowsNormals[2] = Math::LoadVector4Aligned((float*)&normalList[2]);
		Primitive<Math::Vector4> toAdd11(rowsVerts,rowsNormals);
		buffer.PushBack(std::forward<Primitive<>>(toAdd11));

		vertexList[0][0] = -0.5f; vertexList[0][1] = 0.5f; vertexList[0][2] = -0.5f;
		vertexList[1][0] = 0.5f; vertexList[1][1] = 0.5f; vertexList[1][2] = -0.5f;
		vertexList[2][0] = 0.5f; vertexList[2][1] = -0.5f; vertexList[2][2] = -0.5f;

		normalList[0][0] = 0.0f; normalList[0][1] = 0.0f; normalList[0][2] =  1.0f;
		normalList[1][0] = 0.0f; normalList[1][1] = 0.0f; normalList[1][2] =  1.0f;
		normalList[2][0] = 0.0f; normalList[2][1] = 0.0f; normalList[2][2] =  1.0f;
					
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);
		rowsNormals[0] = Math::LoadVector4Aligned((float*)&normalList[0]);
		rowsNormals[1] = Math::LoadVector4Aligned((float*)&normalList[1]);
		rowsNormals[2] = Math::LoadVector4Aligned((float*)&normalList[2]);
		Primitive<Math::Vector4> toAdd12(rowsVerts,rowsNormals);
		buffer.PushBack(std::forward<Primitive<>>(toAdd12));
	}

	void LoadMesh()
	{
		ALIGN float	vertexList[3][4];	/* vertex position coordinates */ 
		ALIGN float	normalList[3][4];	/* vertex normals */ 
		GzTextureIndex  	uvList[3];
		
		vertexList[0][3] = 1.0f;
		vertexList[1][3] = 1.0f;
		vertexList[2][3] = 1.0f;

		normalList[0][3] = 0.0f;
		normalList[1][3] = 0.0f;
		normalList[2][3] = 0.0f;

		while( fscanf(inFile, "%s", dummy) == 1) 
		{ 	
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

			Math::Vector4 rowsVerts[3],rowsNormals[3];
			rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
			rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
			rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);

			rowsNormals[0] = Math::LoadVector4Aligned((float*)&normalList[0]);
			rowsNormals[1] = Math::LoadVector4Aligned((float*)&normalList[1]);
			rowsNormals[2] = Math::LoadVector4Aligned((float*)&normalList[2]);
			Primitive<> toAdd(rowsVerts,rowsNormals);
			buffer.PushBack(std::forward<Primitive<>>(toAdd));
		}
#if testing
		Math::Vector4 rowsVerts[3],rowsNormals[3];
		vertexList[0][0] = 0.0f; vertexList[0][1] = 0.0f; vertexList[0][2] = 0.75f;
		vertexList[1][0] = 0.0f; vertexList[1][1] = 1.0f; vertexList[1][2] = 0.75f; 
		vertexList[2][0] = 1.0f; vertexList[2][1] = 0.0f; vertexList[2][2] = 0.75f; 
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);

		rowsNormals[0] = Math::ident1;
		rowsNormals[1] = Math::ident1;
		rowsNormals[2] = Math::ident1;
		PrimitiveBase* toAdd = new Primitive<>(rowsVerts,rowsNormals);
		buffer->PushBack(toAdd);

		vertexList[0][0] = 0.0f; vertexList[0][1] = 0.0f; vertexList[0][2] = 1.0f;
		vertexList[1][0] = 0.0f; vertexList[1][1] = 1.0f; vertexList[1][2] = 0.5f; 
		vertexList[2][0] = 1.0f; vertexList[2][1] = 0.0f; vertexList[2][2] = 0.5f; 
		rowsVerts[0] = Math::LoadVector4Aligned((float*)&vertexList[0]);
		rowsVerts[1] = Math::LoadVector4Aligned((float*)&vertexList[1]);
		rowsVerts[2] = Math::LoadVector4Aligned((float*)&vertexList[2]);

		toAdd = new Primitive<>(rowsVerts,rowsNormals);
		buffer->PushBack(toAdd);
#endif
	}

	VertexBuffer& GetBuffer()
	{
		return buffer;
	}

private:
	bool initialized;
	VertexBuffer buffer;
	GzCoord		vertexList[3];	/* vertex position coordinates */ 
	GzCoord		normalList[3];	/* vertex normals */ 
	GzTextureIndex  	uvList[3];		/* vertex texture map indices */ 
	char		dummy[256];
	FILE* inFile;
};

#endif