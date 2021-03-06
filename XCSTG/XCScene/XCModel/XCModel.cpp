#include "XCModel.h"
#include "../../util/ImageArrayLoader.h"
#include "../../util/ShaderReader.h"
#include <GL3/gl3w.h>
#include <glfw/glfw3.h>
using namespace xc_ogl;
GLuint XCModel::programHnd = 0;
bool XCModel::have_program_init = false;
unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		stbi_image_free(data);
	}

	return textureID;
}
void XCModel::Draw()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{ 
		meshes[i].MeshRender();
	}
}
void XCModel::loadModel(std::string const & path)
{
	if (!have_program_init)
	{
		ShaderReader shaderLoader;
		shaderLoader.loadFromFile("Shader/model/generalModel.vert", GL_VERTEX_SHADER);
		shaderLoader.loadFromFile("Shader/model/generalModel.frag", GL_FRAGMENT_SHADER);
		shaderLoader.linkAllShader();
		programHnd = shaderLoader.getProgramHandle();
		have_program_init = true;
	}
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		MessageBox(0, importer.GetErrorString(), "ASSIMP ERROR", MB_ICONERROR);
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

void XCModel::processNode(aiNode * node, const aiScene * scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}

XCMesh XCModel::processMesh(aiMesh * mesh, const aiScene * scene)
{
	// data to fill
	std::vector<XCVertex> vertices;
	std::vector<size_t> indices;
	std::vector<XCTexture> textures;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		XCVertex retvertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		retvertex.Position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		retvertex.Normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			retvertex.TexCoords = vec;
		}
		else {
			retvertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		retvertex.Tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		retvertex.Bitangent = vector;

		vertices.push_back(retvertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process texture material
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	//ambient maps
	std::vector<XCTexture> ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_ambient");
	textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());

	//diffuse maps
	std::vector<XCTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// specular maps
	std::vector<XCTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//normal maps
	std::vector<XCTexture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	//height maps
	std::vector<XCTexture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	//displacement maps
	std::vector<XCTexture> displacementMaps = loadMaterialTextures(material, aiTextureType_DISPLACEMENT, "texture_displacement");
	textures.insert(textures.end(), displacementMaps.begin(), displacementMaps.end());

	//emissive maps
	std::vector<XCTexture> emissiveMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emissive");
	textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());

	//light maps
	std::vector<XCTexture> lightMaps = loadMaterialTextures(material, aiTextureType_LIGHTMAP, "texture_light");
	textures.insert(textures.end(), lightMaps.begin(), lightMaps.end());

	//opacity maps
	std::vector<XCTexture> opacityMaps = loadMaterialTextures(material, aiTextureType_OPACITY, "texture_opacity");
	textures.insert(textures.end(), opacityMaps.begin(), opacityMaps.end());

	//reflection maps
	std::vector<XCTexture> reflectionMaps = loadMaterialTextures(material, aiTextureType_REFLECTION, "texture_reflection");
	textures.insert(textures.end(), reflectionMaps.begin(), reflectionMaps.end());

	//shininess maps
	std::vector<XCTexture> shininessMaps = loadMaterialTextures(material, aiTextureType_SHININESS, "texture_shininess");
	textures.insert(textures.end(), shininessMaps.begin(), shininessMaps.end());

	// return a mesh object created from the extracted mesh data
	return XCMesh(programHnd, vertices, indices, textures);
}
std::vector<XCTexture> XCModel::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	{
		std::vector<XCTexture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool exist = false;
			GLuint exist_id = 0;
			aiString exist_str;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0)
				{
					exist_id = textures_loaded[j].id;
					exist_str = textures_loaded[j].path;
					exist = true;
					break;
				}
			}
			if (!exist)
			{ 
				XCTexture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
			else {
				XCTexture texture;
				texture.id = exist_id;
				texture.type = typeName;
				texture.path = exist_str;
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}
		return textures;
	}
}

GLuint XCModel::getProgramHandle()
{
	if (have_program_init)
		return programHnd;
	else
		return 0;
}
