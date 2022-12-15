#ifndef MODEL_ANIM_H
#define MODEL_ANIM_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <meshAnim.h>
#include <model.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class ModelAnim 
{
public:
    /*  Model Data */
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<MeshAnim> meshes;
    string directory;
    bool gammaCorrection;

	/* Importacion base */
	Assimp::Importer importer;
	const aiScene* scene;

	/* Huesos */
	static const uint MAX_BONES = 100;

	map<string, uint> m_bone_mapping; // maps a bone name and their index
	uint m_num_bones = 0;
	vector<BoneMatrix> m_bone_matrices;
	aiMatrix4x4 m_global_inverse_transform;

	GLuint m_bone_location[MAX_BONES];
	float ticks_per_second = 0.0f;

    /*  Functions   */
    // constructor, expects a filepath to a 3D model.
    ModelAnim(string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

	void initShaders(GLuint shader_program)
	{
		for (uint i = 0; i < MAX_BONES; i++) // get location all matrices of bones
		{
			string name = "bones[" + to_string(i) + "]";// name like in shader
			m_bone_location[i] = glGetUniformLocation(shader_program, name.c_str());
		}

		// rotate head AND AXIS(y_z) about x !!!!!  Not be gimbal lock
		//rotate_head_xz *= glm::quat(cos(glm::radians(-45.0f / 2)), sin(glm::radians(-45.0f / 2)) * glm::vec3(1.0f, 0.0f, 0.0f));
	}

    // draws the model, and thus all its meshes
    void Draw(Shader shader)
    {
		// Calculo de las animaciones
		vector<aiMatrix4x4> transforms;
		boneTransform((double)SDL_GetTicks() / 1000.0f, transforms);

		for (uint i = 0; i < transforms.size(); i++) // move all matrices for actual model position to shader
		{
			glUniformMatrix4fv(m_bone_location[i], 1, GL_TRUE, (const GLfloat*)&transforms[i]);
		}

        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
    
private:

    /*  Functions   */
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path)
    {
        // read file via ASSIMP
        scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }

		m_global_inverse_transform = scene->mRootNode->mTransformation;
		m_global_inverse_transform.Inverse();

		if (scene->mAnimations[0]->mTicksPerSecond != 0.0)
		{
			ticks_per_second = scene->mAnimations[0]->mTicksPerSecond;
		}
		else
		{
			ticks_per_second = 25.0f;
		}

        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

		cout << "scene->HasAnimations() 1: " << scene->HasAnimations() << endl;
		cout << "scene->mNumMeshes 1: " << scene->mNumMeshes << endl;
		cout << "scene->mAnimations[0]->mNumChannels 1: " << scene->mAnimations[0]->mNumChannels << endl;
		cout << "scene->mAnimations[0]->mDuration 1: " << scene->mAnimations[0]->mDuration << endl;
		cout << "scene->mAnimations[0]->mTicksPerSecond 1: " << scene->mAnimations[0]->mTicksPerSecond << endl << endl;

		cout << "		name nodes : " << endl;
		showNodeName(scene->mRootNode);
		cout << endl;

		cout << "		name bones : " << endl;
		//processNode(scene->mRootNode, scene);
		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);

		cout << "		name nodes animation : " << endl;
		for (uint i = 0; i < scene->mAnimations[0]->mNumChannels; i++)
		{
			cout << scene->mAnimations[0]->mChannels[i]->mNodeName.C_Str() << endl;
		}
		cout << endl;
    }

	void showNodeName(aiNode* node)
	{
		cout << node->mName.data << endl;
		for (uint i = 0; i < node->mNumChildren; i++)
		{
			showNodeName(node->mChildren[i]);
		}
	}

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene)
    {
        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    MeshAnim processMesh(aiMesh *mesh, const aiScene *scene)
    {
		std::cout << "bones: " << mesh->mNumBones << " vertices: " << mesh->mNumVertices << std::endl;
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
		vector<VertexBoneData> bones_id_weights_for_each_vertex;

		
		//Tal vez haya que hacer resize de los vectores
		vertices.reserve(mesh->mNumVertices);
		indices.reserve(mesh->mNumVertices);
		bones_id_weights_for_each_vertex.resize(mesh->mNumVertices);
		

        // Walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// load bones
		for (uint i = 0; i < mesh->mNumBones; i++)
		{
			uint bone_index = 0;
			string bone_name(mesh->mBones[i]->mName.data);

			// Impresión de los nombres de los huesos
			cout << mesh->mBones[i]->mName.data << endl;

			if (m_bone_mapping.find(bone_name) == m_bone_mapping.end()) // ��������� ��� �� � ������� ��������
			{
				// Allocate an index for a new bone
				bone_index = m_num_bones;
				m_num_bones++;
				BoneMatrix bi;
				m_bone_matrices.push_back(bi);

				m_bone_matrices[bone_index].offset_matrix = mesh->mBones[i]->mOffsetMatrix; // Aqui hay que agregar el giro HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
				m_bone_mapping[bone_name] = bone_index;

				//cout << "bone_name: " << bone_name << "			 bone_index: " << bone_index << endl;
			}
			else
			{
				bone_index = m_bone_mapping[bone_name];
			}

			for (uint j = 0; j < mesh->mBones[i]->mNumWeights; j++)
			{
				uint vertex_id = mesh->mBones[i]->mWeights[j].mVertexId; // �� ������� �� ������ ����� �������� �����
				float weight = mesh->mBones[i]->mWeights[j].mWeight;
				bones_id_weights_for_each_vertex[vertex_id].addBoneData(bone_index, weight); // � ������ ������� ����� ����� � �� ���

				// ������ ������� vertex_id �� ������ ����� � �������� bone_index  ����� ��� weight
				//cout << " vertex_id: " << vertex_id << "	bone_index: " << bone_index << "		weight: " << weight << endl;
			}
		}
        
        // return a mesh object created from the extracted mesh data
        return MeshAnim(vertices, indices, textures, bones_id_weights_for_each_vertex);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for(unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if(!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }

	uint findPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		// ����� ���� ������� ����� ����� ����� ������� ���������� ����� ������ ��������
		for (uint i = 0; i < p_node_anim->mNumPositionKeys - 1; i++) // �������� ����� ��������
		{
			if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime) // �������� �� �������� ��������� !!!
			{
				return i; // �� ������� ������ �������� !!!!!!!!!!!!!!!!!! ����������������������������
			}
		}

		assert(0);
		return 0;
	}

	uint findRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		// ����� ���� ������� ����� ����� ����� ������� ���������� ����� ������ ��������
		for (uint i = 0; i < p_node_anim->mNumRotationKeys - 1; i++) // �������� ����� ��������
		{
			if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime) // �������� �� �������� ��������� !!!
			{
				return i; // �� ������� ������ �������� !!!!!!!!!!!!!!!!!! ����������������������������
			}
		}

		assert(0);
		return 0;
	}

	uint findScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		// ����� ���� ������� ����� ����� ����� ������� ���������� ����� ������ ��������
		for (uint i = 0; i < p_node_anim->mNumScalingKeys - 1; i++) // �������� ����� ��������
		{
			if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime) // �������� �� �������� ��������� !!!
			{
				return i; // �� ������� ������ �������� !!!!!!!!!!!!!!!!!! ����������������������������
			}
		}

		assert(0);
		return 0;
	}

	aiVector3D calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		if (p_node_anim->mNumPositionKeys == 1) // Keys ��� ������� �����
		{
			return p_node_anim->mPositionKeys[0].mValue;
		}

		uint position_index = findPosition(p_animation_time, p_node_anim); // ������ ������ �������� ����� ������� ������
		uint next_position_index = position_index + 1; // ������ ��������� �������� �����
		assert(next_position_index < p_node_anim->mNumPositionKeys);
		// ���� ����� �������
		float delta_time = (float)(p_node_anim->mPositionKeys[next_position_index].mTime - p_node_anim->mPositionKeys[position_index].mTime);
		// ������ = (���� ������� ������ �� ������ �������� ��������� �����) / �� ���� ����� �������
		float factor = (p_animation_time - (float)p_node_anim->mPositionKeys[position_index].mTime) / delta_time;
		assert(factor >= 0.0f && factor <= 1.0f);
		aiVector3D start = p_node_anim->mPositionKeys[position_index].mValue;
		aiVector3D end = p_node_anim->mPositionKeys[next_position_index].mValue;
		aiVector3D delta = end - start;

		return start + factor * delta;
	}

	aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		if (p_node_anim->mNumRotationKeys == 1) // Keys ��� ������� �����
		{
			return p_node_anim->mRotationKeys[0].mValue;
		}

		uint rotation_index = findRotation(p_animation_time, p_node_anim); // ������ ������ �������� ����� ������� ������
		uint next_rotation_index = rotation_index + 1; // ������ ��������� �������� �����
		assert(next_rotation_index < p_node_anim->mNumRotationKeys);
		// ���� ����� �������
		float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - p_node_anim->mRotationKeys[rotation_index].mTime);
		// ������ = (���� ������� ������ �� ������ �������� ��������� �����) / �� ���� ����� �������
		float factor = (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) / delta_time;

		//cout << "p_node_anim->mRotationKeys[rotation_index].mTime: " << p_node_anim->mRotationKeys[rotation_index].mTime << endl;
		//cout << "p_node_anim->mRotationKeys[next_rotaion_index].mTime: " << p_node_anim->mRotationKeys[next_rotation_index].mTime << endl;
		//cout << "delta_time: " << delta_time << endl;
		//cout << "animation_time: " << p_animation_time << endl;
		//cout << "animation_time - mRotationKeys[rotation_index].mTime: " << (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) << endl;
		//cout << "factor: " << factor << endl << endl << endl;

		assert(factor >= 0.0f && factor <= 1.0f);
		aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
		aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

		return nlerp(start_quat, end_quat, factor);
	}

	aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		if (p_node_anim->mNumScalingKeys == 1) // Keys ��� ������� �����
		{
			return p_node_anim->mScalingKeys[0].mValue;
		}

		uint scaling_index = findScaling(p_animation_time, p_node_anim); // ������ ������ �������� ����� ������� ������
		uint next_scaling_index = scaling_index + 1; // ������ ��������� �������� �����
		assert(next_scaling_index < p_node_anim->mNumScalingKeys);
		// ���� ����� �������
		float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - p_node_anim->mScalingKeys[scaling_index].mTime);
		// ������ = (���� ������� ������ �� ������ �������� ��������� �����) / �� ���� ����� �������
		float  factor = (p_animation_time - (float)p_node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
		//cout << "p_animation_time: " << p_animation_time << " " << "mTime: " << (float)p_node_anim->mScalingKeys[scaling_index].mTime << endl << endl << endl;
		assert(factor >= 0.0f && factor <= 1.0f);
		aiVector3D start = p_node_anim->mScalingKeys[scaling_index].mValue;
		aiVector3D end = p_node_anim->mScalingKeys[next_scaling_index].mValue;
		aiVector3D delta = end - start;

		return start + factor * delta;
	}

	const aiNodeAnim * findNodeAnim(const aiAnimation * p_animation, const string p_node_name)
	{
		// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
		// numChannels == numBones
		for (uint i = 0; i < p_animation->mNumChannels; i++)
		{
			const aiNodeAnim* node_anim = p_animation->mChannels[i]; // ��������� ������� ������ node
			if (string(node_anim->mNodeName.data) == p_node_name)
			{
				return node_anim;// ���� ����� �������� �� ������� ����� (� ������� ����������� node) ������������ ���� node_anim
			}
		}

		return nullptr;
	}

	// start from RootNode
	void readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform)
	{

		string node_name(p_node->mName.data);

		//������� node, �� ������� ������������ �������, ������������� �������� ���� ������(aiNodeAnim).
		const aiAnimation* animation = scene->mAnimations[0];
		aiMatrix4x4 node_transform = p_node->mTransformation; // AQUI AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH

		const aiNodeAnim* node_anim = findNodeAnim(animation, node_name); // ����� ������� �� ����� ����

		if (node_anim)
		{

			//scaling
			//aiVector3D scaling_vector = node_anim->mScalingKeys[2].mValue;
			aiVector3D scaling_vector = calcInterpolatedScaling(p_animation_time, node_anim);
			aiMatrix4x4 scaling_matr;
			aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

			//rotation
			//aiQuaternion rotate_quat = node_anim->mRotationKeys[2].mValue;
			aiQuaternion rotate_quat = calcInterpolatedRotation(p_animation_time, node_anim);
			aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

			//translation
			//aiVector3D translate_vector = node_anim->mPositionKeys[2].mValue;
			aiVector3D translate_vector = calcInterpolatedPosition(p_animation_time, node_anim);
			aiMatrix4x4 translate_matr;
			aiMatrix4x4::Translation(translate_vector, translate_matr);

			//if (p_node->mName == scene->mRootNode->mName) {
			//	node_transform = translate_matr * (rotate_matr * aiMatrix4x4 (aiQuaternion(-90.0f, 0.0f, 0.0f).GetMatrix())) * scaling_matr;
			//}

			node_transform = translate_matr * rotate_matr * scaling_matr;
		}

		aiMatrix4x4 global_transform = parent_transform * node_transform;

		// ���� � node �� �������� ����������� bone, �� �� node ������ ��������� � ������ bone !!!
		if (m_bone_mapping.find(node_name) != m_bone_mapping.end()) // true if node_name exist in bone_mapping
		{
			uint bone_index = m_bone_mapping[node_name];
			m_bone_matrices[bone_index].final_world_transform = m_global_inverse_transform * global_transform * m_bone_matrices[bone_index].offset_matrix;
		}

		for (uint i = 0; i < p_node->mNumChildren; i++)
		{
			readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform);
		}

	}

	void boneTransform(double time_in_sec, vector<aiMatrix4x4>& transforms)
	{
		aiMatrix4x4 identity_matrix; // = mat4(1.0f);

		double time_in_ticks = time_in_sec * ticks_per_second;
		float animation_time = fmod(time_in_ticks, scene->mAnimations[0]->mDuration); //������� �� ����� (������� �� ������)
		// animation_time - ���� ������� ������ � ���� ������ �� ������ �������� (�� ������� �������� ����� � �������� )

		readNodeHierarchy(animation_time, scene->mRootNode, identity_matrix);

		transforms.resize(m_num_bones);

		for (uint i = 0; i < m_num_bones; i++)
		{
			transforms[i] = m_bone_matrices[i].final_world_transform;
		}
	}

	glm::mat4 aiToGlm(aiMatrix4x4 ai_matr)
	{
		glm::mat4 result;
		result[0].x = ai_matr.a1; result[0].y = ai_matr.b1; result[0].z = ai_matr.c1; result[0].w = ai_matr.d1;
		result[1].x = ai_matr.a2; result[1].y = ai_matr.b2; result[1].z = ai_matr.c2; result[1].w = ai_matr.d2;
		result[2].x = ai_matr.a3; result[2].y = ai_matr.b3; result[2].z = ai_matr.c3; result[2].w = ai_matr.d3;
		result[3].x = ai_matr.a4; result[3].y = ai_matr.b4; result[3].z = ai_matr.c4; result[3].w = ai_matr.d4;

		//cout << " " << result[0].x << "		 " << result[0].y << "		 " << result[0].z << "		 " << result[0].w << endl;
		//cout << " " << result[1].x << "		 " << result[1].y << "		 " << result[1].z << "		 " << result[1].w << endl;
		//cout << " " << result[2].x << "		 " << result[2].y << "		 " << result[2].z << "		 " << result[2].w << endl;
		//cout << " " << result[3].x << "		 " << result[3].y << "		 " << result[3].z << "		 " << result[3].w << endl;
		//cout << endl;

		//cout << " " << ai_matr.a1 << "		 " << ai_matr.b1 << "		 " << ai_matr.c1 << "		 " << ai_matr.d1 << endl;
		//cout << " " << ai_matr.a2 << "		 " << ai_matr.b2 << "		 " << ai_matr.c2 << "		 " << ai_matr.d2 << endl;
		//cout << " " << ai_matr.a3 << "		 " << ai_matr.b3 << "		 " << ai_matr.c3 << "		 " << ai_matr.d3 << endl;
		//cout << " " << ai_matr.a4 << "		 " << ai_matr.b4 << "		 " << ai_matr.c4 << "		 " << ai_matr.d4 << endl;
		//cout << endl;

		return result;
	}


	aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend)
	{
		//cout << a.w + a.x + a.y + a.z << endl;
		a.Normalize();
		b.Normalize();

		aiQuaternion result;
		float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		float one_minus_blend = 1.0f - blend;

		if (dot_product < 0.0f)
		{
			result.x = a.x * one_minus_blend + blend * -b.x;
			result.y = a.y * one_minus_blend + blend * -b.y;
			result.z = a.z * one_minus_blend + blend * -b.z;
			result.w = a.w * one_minus_blend + blend * -b.w;
		}
		else
		{
			result.x = a.x * one_minus_blend + blend * b.x;
			result.y = a.y * one_minus_blend + blend * b.y;
			result.z = a.z * one_minus_blend + blend * b.z;
			result.w = a.w * one_minus_blend + blend * b.w;
		}

		return result.Normalize();
	}
};
#endif