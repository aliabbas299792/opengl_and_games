#include "header/assimp.h"
#include "header/helper.h"

model::model(const std::string& path) {
  if(path.empty()) return; //this is for initialising class members
  loadModel(path);
}

void model::draw() {
  for(auto &mesh : meshes){
    mesh.draw();
  }
}

void model::loadModel(const std::string& path) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
  //ensures the entire model consists of triangles and that each vertex has a normal, and flips textures if they are imported upside down
  //OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side of the image, but images usually have 0.0 at the top of the y-axis (quoted directly from learnopengl.com)

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){ //if it didn't load properly for whatever reason spit out an error
    std::cout << "Error: Assimp: " << importer.GetErrorString() << "\n";
    emscripten_force_exit(-1); //exit
  }

  directory = path.substr(0, path.find_last_of('/')); //stores the directory

  processNode(scene->mRootNode, scene); //processNode recursively traverses through the assimp structure to get all the relavent data
}

std::vector<texture> model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName) {
  std::vector<texture> textures;
  for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
    aiString string;
    mat->GetTexture(type, i, &string);
    bool skip = false;

    for(auto &texture : textures_loaded){
      if(std::strcmp(texture.path.data(), string.C_Str()) != 0){
        textures.push_back(texture);
        skip = true;
        break;
      }
    }

    if(!skip){ //load the texture if it hasn't already been loaded
      texture Texture;
      Texture.id = loadTextureFromFile(this->directory, string.C_Str());
      Texture.type = typeName;
      Texture.path = string.C_Str();
      //and add the texture to the two vectors
      textures.push_back(Texture);
      textures_loaded.push_back(Texture);
    }

    return textures; //return the textures array
  }

  return std::vector<texture>();
}

mesh model::processMesh(aiMesh *Mesh, const aiScene *scene) {
  std::vector<vertex> verticesLocal;
  std::vector<unsigned int> indicesLocal;
  std::vector<texture> textures;

  for(unsigned int i = 0; i < Mesh->mNumVertices; i++){ //process vertex positions, normals and texture coordinates
    vertex Vertex{};
    Vertex.position = glm::vec3(
      Mesh->mVertices[i].x,
      Mesh->mVertices[i].y,
      Mesh->mVertices[i].z
    );

    Vertex.normal = glm::vec3(
      Mesh->mNormals[i].x,
      Mesh->mNormals[i].y,
      Mesh->mNormals[i].z
    );

    if(Mesh->mTextureCoords[0]){ //if it has texture coordinates
      Vertex.texCoords = glm::vec2(
        Mesh->mTextureCoords[0][i].x,
        Mesh->mTextureCoords[0][i].y
      );
    }else{
      Vertex.texCoords = glm::vec2(0.0f, 0.0f); //default texture coordinates
    }

    verticesLocal.push_back(Vertex);
  }

  for(unsigned int i = 0;i < Mesh->mNumFaces; i++){ //populates the indices array
    aiFace face = Mesh->mFaces[i];
    for(unsigned int j = 0; j < face.mNumIndices; j++){
      indicesLocal.push_back(face.mIndices[j]); //gets each index from each face and puts it into the indices array
    }
  }

  if(Mesh->mMaterialIndex >= 0){ //process materials
    aiMaterial *material = scene->mMaterials[Mesh->mMaterialIndex];

    std::vector<texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end()); //inserts all the diffuse textures

    std::vector<texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end()); //inserts all the specular textures
  }

  mesh ModelMesh(verticesLocal, indicesLocal, textures); //return a mesh

  //increments the number of vertices and indices
  this->vertices += ModelMesh.vertices.size();
  this->indices += ModelMesh.indices.size();

  return ModelMesh;
}

void model::processNode(aiNode *node, const aiScene *scene) { //this actually breaks the recursive relationship, so you could just draw it as is
  for(unsigned int i = 0; i < node->mNumMeshes; i++){
    const unsigned int meshIndex = node->mMeshes[i]; //gets the mesh index
    aiMesh *mesh = scene->mMeshes[meshIndex]; //gets the mesh

    this->meshes.push_back(processMesh(mesh, scene)); //adds the processed mesh to the meshes array
  }

  for(unsigned  int i = 0; i < node->mNumChildren; i++){
    processNode(node->mChildren[i], scene); //this way we recursively gets all of the meshes
  }
}

void model::setInstancing(const std::array<glm::mat4, MAX_ENTITIES> *transformationMatrices, unsigned int maxIndex) {
  for(auto &mesh : meshes){
    mesh.setInstancing(transformationMatrices, maxIndex); //applies instancing stuff to each mesh
  }
}

