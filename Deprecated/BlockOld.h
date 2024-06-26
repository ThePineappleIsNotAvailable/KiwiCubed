//#pragma once
//
//#include <GLError.h>
//#include <GLAD/glad.h>
//
//#include <iostream>
//#include <vector>
//
//#include <FastNoise/FastNoise.h>
//#include <glm/vec3.hpp>
//
//
//enum FaceDirection {
//	FRONT,
//	BACK,
//	LEFT,
//	RIGHT,
//	TOP,
//	BOTTOM
//};
//
//
//class Block {
//	public:
//		char vertexSize = 5; // 3 for position, 2 for texture coordinates
//
//		Block() : blockX(0), blockY(0), blockZ(0), isSolid(-1) {}
//		Block(int blockX, int blockY, int blockZ);
//
//		void GenerateBlock (int blockX, int blockY, int blockZ, int chunkX, int chunkY, int chunkZ, int chunkSize);
//		void SetSolid(int solid);
//		int GetType();
//
//		void AddFace(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, FaceDirection faceDirection, int chunkX, int chunkY, int chunkZ, int chunkSize);
//
//	private:
//		int blockX;
//		int blockY;
//		int blockZ;
//
//		char isSolid;
//};