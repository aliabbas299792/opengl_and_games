#include "../header/ecs.h"
using namespace ecs::entity;

unsigned int entityManager::create(std::initializer_list<ecs::component::components> initialiseWithStructs, entityType type){
    nextEntity.id = 0;
    while(alive(nextEntity)){ //loops through all entities until an unused ID is found
        nextEntity.id++;
    }

    entities.insert({nextEntity, type}); //and when it is found, the entity with this ID is inserted

    for(int i = 0;i < initialiseWithStructs.size(); i++){ //loops through the initializer
        //std::cout << "Enum: " << *(initialiseWithStructs.begin()+i) << std::endl;
        if(*(initialiseWithStructs.begin()+i) == ecs::component::components::USER){ //uses the *(struct.begin()+i) rather than struct[i] notation because that notation doesn't work here
            ecs::component::user userTemp;
            ecs::component::users.addComponent(userTemp, nextEntity.id); //appropriate component added to the appropriate structure
        }else if(*(initialiseWithStructs.begin()+i) == ecs::component::components::DRAWABLE){
            ecs::component::drawable drawableStruct;
            ecs::component::drawables.addComponent(drawableStruct, nextEntity.id); //appropriate component added to the appropriate structure
        }else if(*(initialiseWithStructs.begin()+i) == ecs::component::components::PHYSICAL){
            ecs::component::physical physicalStruct;
            ecs::component::physicsObjects.addComponent(physicalStruct, nextEntity.id); //appropriate component added to the appropriate structure
        }else if(*(initialiseWithStructs.begin()+i) == ecs::component::components::THROWN_ITEM){
            ecs::component::thrown_item thrownItem;
            ecs::component::thrown_items.addComponent(thrownItem, nextEntity.id); //appropriate component added to the appropriate structure
        }else if(*(initialiseWithStructs.begin()+i) == ecs::component::components::MISSION){
            ecs::component::mission missionStruct;
            ecs::component::missions.addComponent(missionStruct, nextEntity.id); //appropriate component added to the appropriate structure
        }else if(*(initialiseWithStructs.begin()+i) == ecs::component::components::NPC){
            ecs::component::npc npcStruct;
            ecs::component::npcs.addComponent(npcStruct, nextEntity.id); //appropriate component added to the appropriate structure
        }else if(*(initialiseWithStructs.begin()+i) == ecs::component::components::MOB){
            ecs::component::mob mobStruct;
            ecs::component::mobs.addComponent(mobStruct, nextEntity.id); //appropriate component added to the appropriate structure
        }else if(*(initialiseWithStructs.begin()+i) == ecs::component::components::MP_HP){
            ecs::component::mp_hp mpHpStruct;
            ecs::component::mpHpObjects.addComponent(mpHpStruct, nextEntity.id); //appropriate component added to the appropriate structure
        }
    }
    
    return nextEntity.id; //returns the entity ID
}

unsigned int entityManager::create(entityType type){
    nextEntity.id = 0;
    while(alive(nextEntity)){ //loops through all entities until an unused ID is found
        nextEntity.id++;
    }

    entities.insert({nextEntity, type}); //and when it is found, the entity with this ID is inserted

    if(type == ecs::entity::USER || type == ecs::entity::MOB){
        ecs::component::mp_hp mpHpStruct;
        ecs::component::mpHpObjects.addComponent(mpHpStruct, nextEntity.id);
        //add in any other component objects
    }

    if(type == ecs::entity::USER || type == ecs::entity::MOB || type == ecs::entity::COLLISION_OBJECT || type == ecs::entity::ITEM_THROWN || type == ecs::entity::NPC){
        ecs::component::drawable drawableStruct;
        ecs::component::drawables.addComponent(drawableStruct, nextEntity.id); //appropriate component added to the appropriate structure
        ecs::component::physical physicalStruct; //separate from drawables as we might need drawables separately for say inventory items (not yet though)
        ecs::component::physicsObjects.addComponent(physicalStruct, nextEntity.id); //appropriate component added to the appropriate structure
    }
    
    if(type == ecs::entity::USER){      
        ecs::component::user userTemp;
        ecs::component::users.addComponent(userTemp, nextEntity.id); //appropriate component added to the appropriate structure
    }else if(type == ecs::entity::MOB){
        ecs::component::mob mobStruct;
        ecs::component::mobs.addComponent(mobStruct, nextEntity.id); //appropriate component added to the appropriate structure
    }else if(type == ecs::entity::NPC){
        ecs::component::npc npcStruct;
        ecs::component::npcs.addComponent(npcStruct, nextEntity.id); //appropriate component added to the appropriate structure
    }else if(type == ecs::entity::MISSION){
        ecs::component::mission missionStruct;
        ecs::component::missions.addComponent(missionStruct, nextEntity.id); //appropriate component added to the appropriate structure
    }else if(type == ecs::entity::ITEM_THROWN){
        ecs::component::thrown_item thrownItem;
        ecs::component::thrown_items.addComponent(thrownItem, nextEntity.id); //appropriate component added to the appropriate structure
    }
    
    return nextEntity.id; //returns the entity ID
}

bool entityManager::alive(entity entityStruct){ //returns whether or not the entity ID provided is in use or not basically
    if(entities.find(entityStruct) != entities.end()){
        return true;
    }else{
        return false;
    }
}

void entityManager::destroy(entity entityStruct){ //removes entities and all of the associated components
    ecs::entity::entityType type = entities[entityStruct];
    if(type == ecs::entity::USER){
        ecs::component::users.removeComponent(entityStruct.id);
        ecs::component::drawables.removeComponent(entityStruct.id);
        ecs::component::physicsObjects.removeComponent(entityStruct.id);
        ecs::component::mpHpObjects.removeComponent(entityStruct.id);
        //add in any other component objects
    }else if(type == ecs::entity::COLLISION_OBJECT){
        ecs::component::drawables.removeComponent(entityStruct.id);
        ecs::component::physicsObjects.removeComponent(entityStruct.id);
    }else if(type == ecs::entity::ITEM_THROWN){
        ecs::component::thrown_items.removeComponent(entityStruct.id);
        ecs::component::drawables.removeComponent(entityStruct.id);
        ecs::component::physicsObjects.removeComponent(entityStruct.id);
    }else if(type == ecs::entity::MOB){
        ecs::component::drawables.removeComponent(entityStruct.id);
        ecs::component::physicsObjects.removeComponent(entityStruct.id);
        ecs::component::mpHpObjects.removeComponent(entityStruct.id);
        ecs::component::mobs.removeComponent(entityStruct.id);
    }else if(type == ecs::entity::NPC){
        ecs::component::drawables.removeComponent(entityStruct.id);
        ecs::component::physicsObjects.removeComponent(entityStruct.id);
        ecs::component::npcs.removeComponent(entityStruct.id);
    }else if(type == ecs::entity::MISSION){
        ecs::component::missions.removeComponent(entityStruct.id);
    }

    nextEntity.id = entityStruct.id; //uses the entity ID to basically make a temporary entity using nextEntity
    entities.erase(nextEntity); //erases the entity which is exactly like that
    nextEntity.id = 0; //and sets the entity ID to 0 so that nextEntity can be used elsewhere with no concerns
}

ecs::entity::entityType entityManager::getType(entity entityStruct){
    return entities[entityStruct];
}