#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../header/classDefines.h"
#include <glm/ext.hpp>

chunksHolder::chunksHolder(glm::vec3 pos, Shader* shader, Player* player) {
	this->player = player;
	this->shader = shader;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				chunksLoaded.push_back(
					chunks(
						pos.x + 60 * i,
						pos.y + 60 * j,
						pos.z + 60 * k,
						shader,
						player
					)
				);
			}
		}
	}
}

void chunksHolder::liveChunks() {
	for (int i = 0; i < chunksLoaded.size(); i++) {
		chunksLoaded[i].chunkUpdate();
	}
}

void chunksHolder::updateVirtualChunk() {
	virtualChunk.x = floor((player->pos.x) / 60) * 60;
	virtualChunk.y = floor((player->pos.y) / 60) * 60;
	virtualChunk.z = floor((player->pos.z) / 60) * 60;

	virtualChunkHelper(glm::vec3(virtualChunk.x - 60, virtualChunk.y - 60, virtualChunk.z - 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y - 60, virtualChunk.z - 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 60, virtualChunk.y - 60, virtualChunk.z - 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x - 60, virtualChunk.y, virtualChunk.z - 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y, virtualChunk.z - 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 60, virtualChunk.y, virtualChunk.z - 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x - 60, virtualChunk.y + 60, virtualChunk.z - 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y + 60, virtualChunk.z - 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 60, virtualChunk.y + 60, virtualChunk.z - 60));

	virtualChunkHelper(glm::vec3(virtualChunk.x - 60, virtualChunk.y - 60, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y - 60, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 60, virtualChunk.y - 60, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x - 60, virtualChunk.y, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 60, virtualChunk.y, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x - 60, virtualChunk.y + 60, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y + 60, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 60, virtualChunk.y + 60, virtualChunk.z));

	virtualChunkHelper(glm::vec3(virtualChunk.x - 60, virtualChunk.y - 60, virtualChunk.z + 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y - 60, virtualChunk.z + 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 60, virtualChunk.y - 60, virtualChunk.z + 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x - 60, virtualChunk.y, virtualChunk.z + 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y, virtualChunk.z + 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 60, virtualChunk.y, virtualChunk.z + 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x - 60, virtualChunk.y + 60, virtualChunk.z + 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y + 60, virtualChunk.z + 60));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 60, virtualChunk.y + 60, virtualChunk.z + 60));
	//assuming -60k is back, 0k is center, 60k is front (from i,j,k)

	/*
	for (int i = 0; i < chunksLoaded.size(); i++) {
		if (chunksLoaded[i].active == false) {
			chunksLoaded[i].~chunks();
			chunksLoaded.pop_back();
			//std::cout << "Deleted " << glm::to_string(chunksLoaded[i].chunkCoords) << " -- " << glm::to_string(virtualChunk) << std::endl;
			//system("PAUSE");
		}
		chunksLoaded[i].statusChanged = false;
		//std::cout << glm::to_string(chunksLoaded[i].chunkCoords) << std::endl;
	}
	*/
	//std::cout << glm::to_string(virtualChunk) << std::endl;
	//system("PAUSE");
	
}

void chunksHolder::virtualChunkHelper(glm::vec3 pos) {
	//std::cout << glm::to_string(pos) << std::endl;
	bool found = false;
	for (int i = 0; i < chunksLoaded.size(); i++) {
		if (chunksLoaded[i].chunkCoords == pos) {
			chunksLoaded[i].statusChanged = true;
			found = true;
			break;
		}
	}
	//std::cout << found << " --- " << glm::to_string(pos) << std::endl;
	if (found == false) {
		chunksLoaded.push_back(
			chunks(
				pos.x,
				pos.y,
				pos.z,
				shader,
				player
			)
		);
	}
}