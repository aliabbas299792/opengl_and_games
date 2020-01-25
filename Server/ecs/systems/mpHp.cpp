#include "../../header/ecs.h"
using namespace ecs::system;
using namespace ecs::component;

void mpHpSystem::damage(){
    mutexs::attackVectorMutex.lock(); //need to lock it as we add attacks for processing from another thread (physics.cpp)

    std::vector<int> entitiesKilled;

    for(auto &entityIDdamagePair : attacks){
        for(auto &entityID : entitiesInRange[entityIDdamagePair.first]){
            int compVecIndex = mpHpObjects.entityToVectorMap(entityID);
            if(compVecIndex != -1){
                if(entity::superEntityManager.getType(entity::entity(entityID)) == ecs::entity::MOB){
                    mpHpObjects.compVec[mpHpObjects.entityToVectorMap(entityID)].hp -= entityIDdamagePair.second;
                    if(mpHpObjects.compVec[mpHpObjects.entityToVectorMap(entityID)].hp < 0){
                        mobSystem::getInstance()->dropItems(entityID);
                        entitiesKilled.push_back(entityID);
                    }
                }
            }
        }
    }

    for(auto &entityID : entitiesKilled){
        sf::Vector2f coordinates = physicsObjects.compVec[physicsObjects.entityToVectorMap(entityID)].coordinates;
        coordinatesStruct chunkCoord(chunkCoordHelperX(coordinates.x, chunkPixelSize_x), chunkCoordHelperY(coordinates.y, chunkPixelSize_y));
        
        auto dropItems = mobs.compVec[mobs.entityToVectorMap(entityID)].dropItems;

        //drop 3 items
        itemSystem::getInstance()->dropItem(coordinates, dropItems.x, {0, 0});
        itemSystem::getInstance()->dropItem(coordinates, dropItems.y, {0, 0});
        itemSystem::getInstance()->dropItem(coordinates, dropItems.z, {0, 0});

        for(int i = 0; i < chunks[chunkCoord].second.size(); i++){
            if(chunks[chunkCoord].second[i].id == entityID){
                chunks[chunkCoord].second.erase(chunks[chunkCoord].second.begin() + i); //erases the entity from the vector
                break;
            }
        }
        chunks[chunkCoord].first.mobCount--; //decrement the mob count because we just killed one

        entity::superEntityManager.destroy(entity::entity(entityID)); //destroys the entity
    }

    entitiesKilled.clear();
    attacks.clear();

    mutexs::attackVectorMutex.unlock();
}