#pragma once
#include"Model.h"
#include"Usefull.h"
using namespace std;
#define PI 3.1415926

#ifdef _DEBUG
#pragma comment(lib, "assimp/Debug/assimp-vc140-mt.lib")
#else
#pragma comment(lib, "assimp/Release/assimp-vc140-mt.lib")
#endif

bool CompWieght(pair<unsigned int, float> &a, pair<unsigned int, float> &b)
{
	return a.second > b.second;
}

Node::Node()
{
	name = "";
	id = -1;
	localTransformMatrix = aiMatrix4x4();
	//globalTransformMatrix = aiMatrix4x4();
	parentID = -1;
}

void NodeList::GetGlobalMatrixRecur(const aiMatrix4x4 & parentGlobal, int nodeID, vector<aiMatrix4x4>& outNodeGlobals)
{
	Node &node = (*this)[nodeID];
	outNodeGlobals[nodeID] = parentGlobal*node.localTransformMatrix;
	for (int i = 0; i < node.childrenID.size(); i++)
	{
		int &childID = node.childrenID[i];
		GetGlobalMatrixRecur(outNodeGlobals[nodeID], childID, outNodeGlobals);
	}
}
void NodeList::GetGlobalMatrix(vector<aiMatrix4x4>& outNodeGlobals)
{
	outNodeGlobals.resize(this->size());
	GetGlobalMatrixRecur(aiMatrix4x4(), 0, outNodeGlobals);
}

BindingBone::BindingBone()
{
	name = "";
	nodeID = -1;
	offset = aiMatrix4x4();
}

Mesh::Mesh()
{
	name = "";
	materialID = -1;
	nodeID = -1;
}
void Mesh::Purge()
{
	vertexPositions.clear();
	vertexNormals.clear();
	vertexTangent.clear();
	vertexBitangent.clear();
	vertexTexCoords.clear();
	vertexBindID.clear();
	vertexBindWight.clear();
	indices.clear();
}

VecKey::VecKey()
{
	value = aiVector3D(0, 0, 0);
	timeFrame = 0;
}

QuatKey::QuatKey()
{
	value = aiQuaternion(1, 0, 0);
	timeFrame = 0;
}

aiMatrix4x4 NodeFrame::ToMatrix()
{
	aiMatrix4x4 matrix = aiMatrix4x4(rotation.GetMatrix());
	matrix.a1 *= scaling.x;
	matrix.b1 *= scaling.x;
	matrix.c1 *= scaling.x;
	matrix.a2 *= scaling.y;
	matrix.b2 *= scaling.y;
	matrix.c2 *= scaling.y;
	matrix.a3 *= scaling.z;
	matrix.b3 *= scaling.z;
	matrix.c3 *= scaling.z;
	matrix.a4 = position.x;
	matrix.b4 = position.y;
	matrix.c4 = position.z;
	return matrix;
}

NodeAnimation::NodeAnimation()
{
	tickDuration = 0;
	nodeID = -1;
}
aiVector3D NodeAnimation::EvaluateVecKey(vector<VecKey> &keys, double tick)
{
	aiVector3D result(0, 0, 0);
	//Binary search
	int start = 0, end = keys.size() - 1;
	for (int middle = (start + end) / 2; start + 1 < end; middle = (start + end) / 2)
	{
		if (keys[middle].timeFrame <= tick)
			start = middle;
		else
			end = middle;
	}
	if (end == start)
	{
		return keys[start].value;
	}
	float gap = float(keys[end].timeFrame - keys[start].timeFrame);
	float factor = (tick - keys[start].timeFrame) / gap;
	result = factor*keys[end].value + (1 - factor)*keys[start].value;

	return result;
}
aiQuaternion NodeAnimation::EvaluateQuatKey(vector<QuatKey>& keys, double tick)
{
	aiQuaternion result(1, 0, 0);
	//Binary search
	int start = 0, end = keys.size() - 1;
	for (int middle = (start + end) / 2; start + 1 < end; middle = (start + end) / 2)
	{
		if (keys[middle].timeFrame <= tick)
			start = middle;
		else
			end = middle;
	}
	float gap = float(keys[end].timeFrame - keys[start].timeFrame);
	float factor = (tick - keys[start].timeFrame) / gap;
	aiQuaternion::Interpolate(result, keys[start].value, keys[end].value, factor);

	return result;
}
NodeFrame NodeAnimation::Evaluate(double tick)
{
	NodeFrame result;
	result.position = EvaluateVecKey(positionKeys, tick);
	result.scaling = EvaluateVecKey(scalingKeys, tick);
	result.rotation = EvaluateQuatKey(rotationKeys, tick);
	return result;
}
void NodeAnimation::Clear()
{
	positionKeys.clear();
	scalingKeys.clear();
	rotationKeys.clear();
	tickDuration = 0;
	nodeID = -1;
}

Animation::Animation()
{
	name = "";
	duration = 0;
	ticksPerSecond = 25.0;
}

void Model::ApplyAnimation(unsigned int animationID, double animationTime, vector<aiMatrix4x4>& outNodeGlobals)
{
	if (animationID >= animationList.size())
		return;
	Animation &animation = animationList[animationID];
	animationTime = fmod(animationTime, animation.duration);
	double tick = animationTime*animation.ticksPerSecond;

	for (int i = 0; i < animation.nodeAnimationList.size(); i++)
	{
		NodeAnimation &nodeAnimation = animation.nodeAnimationList[i];
		aiMatrix4x4 &localTransformMatrix = nodeList[nodeAnimation.nodeID].localTransformMatrix;
		localTransformMatrix = nodeAnimation.Evaluate(tick).ToMatrix();
	}
	nodeList.GetGlobalMatrix(outNodeGlobals);
}
unsigned int Model::GetVisibleInstancesNum()
{
	map<int, Instance> ::iterator it = instances.begin();
	unsigned int count = 0;
	while (it != instances.end())
	{
		if (it->second.visable)
		{
			count++;
		}
		it++;
	}
	return count;
}
unsigned int Model::UpdateVisableInstances()
{
	map<int, Instance> ::iterator it = instances.begin();
	unsigned int count = 0;
	while (it != instances.end())
	{
		if (it->second.visable)
		{
			it->second.Apply();
			count++;
		}
		it++;
	}
	return count;
}
bool Model::LoadFileD3D(string filePath)
{
	return LoadFileD3D(filePath, 4);
}
bool Model::LoadFileD3D(string filePath, int maxBonePerVertex)
{
	this->modelFilePath = filePath;
	Assimp::Importer modelLoader;
	const aiScene* model = modelLoader.ReadFile(filePath,
		aiProcess_MakeLeftHanded | //For DirectX
		aiProcess_FlipUVs |//For DirectX
		aiProcess_FlipWindingOrder |//For DirectX
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenSmoothNormals |
		aiProcess_SortByPType);
	if (!model)
	{
		return false;
	}
	if (!Load(model, maxBonePerVertex))
	{
		return false;
	}

	return true;
}
bool Model::Load(const aiScene * source, int maxBonePerVertex)
{
	nodeList.clear();
	hasAnimation = source->HasAnimations();
	this->maxBonePerVertex = maxBonePerVertex;
	LoadNodeListRecur(-1, source->mRootNode);

	if (hasAnimation)
	{
		LoadAnimationList(source);
	}

	LoadMesh(source);

	LoadMaterial(source);

	return true;
}
void Model::LoadNodeListRecur(int parentID, aiNode * ainode)
{
	Node node;
	node.id = nodeList.size();
	node.localTransformMatrix = ainode->mTransformation;
	node.name = ainode->mName.C_Str();
	node.parentID = parentID;
	nodeList.push_back(node);
	nameIDTable[node.name] = node.id;
	for (int i = 0; i < ainode->mNumMeshes; i++)
	{
		meshNodeTable[ainode->mMeshes[i]] = node.id;
	}
	if (parentID >= 0)
	{
		nodeList[parentID].childrenID.push_back(node.id);
	}
	for (int i = 0; i < ainode->mNumChildren; i++)
	{
		aiNode *child = ainode->mChildren[i];
		LoadNodeListRecur(node.id, child);
	}
}
void Model::LoadAnimationList(const aiScene * source)
{
	animationList.clear();
	animationList.resize(source->mNumAnimations);
	for (int i = 0; i < source->mNumAnimations; i++)
	{
		aiAnimation *srcAnim = source->mAnimations[i];
		Animation &animation = animationList[i];
		animation.name = srcAnim->mName.C_Str();
		animation.duration = srcAnim->mDuration;
		animation.ticksPerSecond = srcAnim->mTicksPerSecond ? srcAnim->mTicksPerSecond : 25.0;
		animation.nodeAnimationList.resize(srcAnim->mNumChannels);
		for (int j = 0; j < srcAnim->mNumChannels; j++)
		{
			NodeAnimation &nodeAnimation = animation.nodeAnimationList[j];
			aiNodeAnim *srcNodeAnim = srcAnim->mChannels[j];
			nodeAnimation.nodeID = nameIDTable[srcNodeAnim->mNodeName.C_Str()];
			nodeAnimation.positionKeys.resize(srcNodeAnim->mNumPositionKeys);
			nodeAnimation.scalingKeys.resize(srcNodeAnim->mNumScalingKeys);
			nodeAnimation.rotationKeys.resize(srcNodeAnim->mNumRotationKeys);
			for (int z = 0; z < srcNodeAnim->mNumPositionKeys; z++)
			{
				nodeAnimation.positionKeys[z].timeFrame = srcNodeAnim->mPositionKeys[z].mTime;
				nodeAnimation.positionKeys[z].value = srcNodeAnim->mPositionKeys[z].mValue;
			}
			for (int z = 0; z < srcNodeAnim->mNumScalingKeys; z++)
			{
				nodeAnimation.scalingKeys[z].timeFrame = srcNodeAnim->mScalingKeys[z].mTime;
				nodeAnimation.scalingKeys[z].value = srcNodeAnim->mScalingKeys[z].mValue;
			}
			for (int z = 0; z < srcNodeAnim->mNumRotationKeys; z++)
			{
				nodeAnimation.rotationKeys[z].timeFrame = srcNodeAnim->mRotationKeys[z].mTime;
				nodeAnimation.rotationKeys[z].value = srcNodeAnim->mRotationKeys[z].mValue;
			}
		}
	}
}
void Model::LoadMesh(const aiScene * source)
{
	meshList.clear();
	meshList.resize(source->mNumMeshes);
	for (int i = 0; i < source->mNumMeshes; i++)
	{
		Mesh &mesh = meshList[i];
		aiMesh *srcMesh = source->mMeshes[i];
		mesh.nodeID = meshNodeTable[i];
		mesh.name = srcMesh->mName.C_Str();
		//bone
		mesh.boneList.resize(srcMesh->mNumBones);
		if (srcMesh->HasBones())
		{
			for (int j = 0; j < srcMesh->mNumBones; j++)
			{
				BindingBone &bone = mesh.boneList[j];
				aiBone *srcBone = srcMesh->mBones[j];
				bone.name = srcBone->mName.C_Str();
				bone.nodeID = nameIDTable[srcBone->mName.C_Str()];
				bone.offset = srcBone->mOffsetMatrix;
			}
		}
		//vertex
		if (srcMesh->HasPositions())
		{
			mesh.vertexPositions.resize(srcMesh->mNumVertices);
			for (int z = 0; z < srcMesh->mNumVertices; z++)
			{
				mesh.vertexPositions[z] = srcMesh->mVertices[z];
			}
		}
		if (srcMesh->HasNormals())
		{
			mesh.vertexNormals.resize(srcMesh->mNumVertices);
			for (int z = 0; z < srcMesh->mNumVertices; z++)
			{
				mesh.vertexNormals[z] = srcMesh->mNormals[z];
			}
		}
		if (srcMesh->HasTangentsAndBitangents())
		{
			mesh.vertexTangent.resize(srcMesh->mNumVertices);
			mesh.vertexBitangent.resize(srcMesh->mNumVertices);
			for (int z = 0; z < srcMesh->mNumVertices; z++)
			{
				mesh.vertexTangent[z] = srcMesh->mTangents[z];
				mesh.vertexBitangent[z] = srcMesh->mBitangents[z];
			}
		}
		if (srcMesh->HasTextureCoords(0) && 2 == srcMesh->mNumUVComponents[0])//only support 1 set of 2D texcoords for now!
		{
			mesh.vertexTexCoords.resize(srcMesh->mNumVertices);
			for (int z = 0; z < srcMesh->mNumVertices; z++)
			{
				mesh.vertexTexCoords[z].x = srcMesh->mTextureCoords[0][z].x;
				mesh.vertexTexCoords[z].y = srcMesh->mTextureCoords[0][z].y;
			}
		}
		//Load indices
		mesh.indices.resize(srcMesh->mNumFaces * 3);
		for (int z = 0; z < srcMesh->mNumFaces; z++)
		{
			mesh.indices[3 * z + 0] = srcMesh->mFaces[z].mIndices[0];
			mesh.indices[3 * z + 1] = srcMesh->mFaces[z].mIndices[1];
			mesh.indices[3 * z + 2] = srcMesh->mFaces[z].mIndices[2];
		}
		if (srcMesh->HasBones())
		{
			mesh.vertexBindID.resize(maxBonePerVertex*srcMesh->mNumVertices, 0xFFFFFF);
			mesh.vertexBindWight.resize(maxBonePerVertex * srcMesh->mNumVertices, 0.0f);
			vector<vector<pair<unsigned int, float>>> boneWeights;//pervertex[perbone[]]
			boneWeights.resize(srcMesh->mNumVertices);
			//Build vertex weight table
			for (int z = 0; z < srcMesh->mNumBones; z++)
			{
				aiBone *bone = srcMesh->mBones[z];
				pair<unsigned int, double> bw;
				bw.first = z;
				for (int u = 0; u < bone->mNumWeights; u++)
				{
					bw.second = bone->mWeights[u].mWeight;
					boneWeights[bone->mWeights[u].mVertexId].push_back(bw);
				}
			}
			//build bindID and bindWeight array
			for (int vertexID = 0; vertexID < boneWeights.size(); vertexID++)//per vertex loop
			{
				unsigned int vindex = maxBonePerVertex*vertexID;
				//If bones number exceed the limit
				if (true)
				{
					float sum = 0;
					sort(boneWeights[vertexID].begin(), boneWeights[vertexID].end(), CompWieght);
					for (int z = 0; z < maxBonePerVertex&&z<boneWeights[vertexID].size(); z++)//per binded bone loop, get the sum of usefull weight
					{
						sum += boneWeights[vertexID][z].second;
					}

					for (int z = 0; z < maxBonePerVertex&&z<boneWeights[vertexID].size(); z++)
					{
						mesh.vertexBindID[vindex + z] = boneWeights[vertexID][z].first;
						mesh.vertexBindWight[vindex + z] = boneWeights[vertexID][z].second / sum; //normalize weight
					}
				}
				else //Bones number under the limit
				{
					for (int z = 0; z < boneWeights[vertexID].size(); z++)
					{
						mesh.vertexBindID[vindex + z] = boneWeights[vertexID][z].first;
						mesh.vertexBindWight[vindex + z] = boneWeights[vertexID][z].second;
					}
				}
			}
		}
		if (source->HasMaterials())
		{
			mesh.materialID = srcMesh->mMaterialIndex;
		}
	}
}
void Model::LoadMaterial(const aiScene * source)
{
	if (!source->HasMaterials())
	{
		return;
	}
	materialList.resize(source->mNumMaterials);

	string folderPath = GetFolderPath(modelFilePath);
	aiString textureFileName;
	aiColor3D color;
	float fl;
	for (int i = 0; i < source->mNumMaterials; i++)
	{
		Material &material = materialList[i];
		aiMaterial *srcMaterial = source->mMaterials[i];

		color = aiColor3D(0, 0, 0);
		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material.diffuse[0] = color.r;
		material.diffuse[1] = color.g;
		material.diffuse[2] = color.b;
		material.diffuse[3] = 0;

		color = aiColor3D(0, 0, 0);
		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material.specular[0] = color.r;
		material.specular[1] = color.g;
		material.specular[2] = color.b;
		material.specular[3] = 0;

		color = aiColor3D(0, 0, 0);
		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material.ambient[0] = color.r;
		material.ambient[1] = color.g;
		material.ambient[2] = color.b;
		material.ambient[3] = 0;

		fl = 0;
		srcMaterial->Get(AI_MATKEY_SHININESS, fl);
		material.shininess = fl;

		fl = 1;
		srcMaterial->Get(AI_MATKEY_OPACITY, fl);
		material.opacity = fl;

		textureFileName.Clear();
		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &textureFileName);
		if (textureFileName.length)
		{
			if (!material.diffuseMap.LoadFromFile(folderPath + textureFileName.C_Str()))
				Message("Diffuse Texture error", 1);
			else
				material.hasDiffuseMap = true;
		}
		textureFileName.Clear();
		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &textureFileName);
		if (textureFileName.length)
		{
			if (!material.specularMap.LoadFromFile(folderPath + textureFileName.C_Str()))
				Message("Specular Texture error", 1);
			else
				material.hasSpecularMap = true;
		}
		textureFileName.Clear();
		srcMaterial->GetTexture(aiTextureType_AMBIENT, 0, &textureFileName);
		if (textureFileName.length)
		{
			if (!material.ambientMap.LoadFromFile(folderPath + textureFileName.C_Str()))
				Message("Ambient Texture error", 1);
			else
				material.hasAmbientMap = true;
		}
		textureFileName.Clear();
		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &textureFileName);
		if (textureFileName.length)
		{
			if (!material.normalMap.LoadFromFile(folderPath + textureFileName.C_Str()))
				Message("Normal Texture error", 1);
			else
				material.hasNormalMap = true;
		}
	}

}

Model::Model()
{
	hasAnimation = false;
	maxBonePerVertex = 4;
}
Instance * Model::CreateInstance()
{
	unsigned int i;
	for (i = 0; i < INT_MAX; i++)
	{
		if (instances.count(i) == 0)
			break;
	}
	if (i == INT_MAX)
	{
		return NULL;
	}
	instances[i] = Instance(this, i);
	
	return &instances[i];
}
void Model::DeletInstance(unsigned int instanceID)
{
	instances.erase(instanceID);
}
void Model::Purge()
{
	nameIDTable.clear();
	meshNodeTable.clear();
	for (int i = 0; i < meshList.size(); i++)
	{
		meshList[i].Purge();
	}
	for (int i = 0; i < materialList.size(); i++)
	{
		materialList[i].Clear();
	}
}

InstanceMaterial::InstanceMaterial()
{
	diffusePower = 1;
	specularHardness = 1;
	specularPower = 0;
	emissivity = 0;
	refractiveIndex = 1;
	diffuseColor[0] = 1;
	diffuseColor[1] = 1;
	diffuseColor[2] = 1;
	emissiveColor[0] = 1;
	emissiveColor[1] = 1;
	emissiveColor[2] = 1;
	specularColor[0] = 1;
	specularColor[1] = 1;
	specularColor[2] = 1;
	opacity=1;
	textureEnableFlags=0xFF;
}

Instance::Instance(Model *model, unsigned int id)
{
	animationID = 0;
	animationTime = 0;
	instanceID = id;
	useInstanceMaterial = false;
	visable = true;

	this->model = model;
}
Instance::Instance()
{
	animationID = -1;
	animationTime = 0;
	visable = true;
	useInstanceMaterial = false;
	instanceID = 0;

	this->model = NULL;
}
double Instance::GetAnimationDuration()
{
	if (animationID >= model->animationList.size()|| animationID<0)
	{
		return 0.0;
	}
	return model->animationList[animationID].duration;
}
unsigned int Instance::GetMeshNumber()
{
	return model->meshList.size();
}
double Instance::GetAnimationNumber()
{
	return model->animationList.size();
	return 0.0;
}
void Instance::Apply()
{
	ApplyAnimation();
	UpdateTransform();
}
void Instance::ApplyAnimation()
{
	model->ApplyAnimation(animationID, animationTime, nodesGlobal);
}
vector<aiMatrix4x4> Instance::GetBindMatrix(unsigned int meshID)
{
	if (meshID >= model->meshList.size())
	{
		return vector<aiMatrix4x4>();
	}
	vector<aiMatrix4x4> result;
	const Mesh &mesh = model->meshList[meshID];
	result.resize(mesh.boneList.size());

	const NodeList &nl = model->nodeList;
	aiMatrix4x4 globalInverse = nodesGlobal[mesh.nodeID];
	globalInverse.Inverse();
	
	for (int i = 0; i < mesh.boneList.size(); i++)
	{
		const int &boneNodeID = mesh.boneList[i].nodeID;
		aiMatrix4x4 &boneMatrix = nodesGlobal[boneNodeID];
		result[i] = globalInverse*boneMatrix*mesh.boneList[i].offset;
	}

	return result;
}

unsigned int Instance::GetinstanceID()
{
	return instanceID;
}
Model * Instance::GetModelPtr()
{
	return model;
}


Transform::Transform()
{
	position=aiVector3D(0,0,0);
	rotation = aiQuaternion(0, 0, 0);
	scaling=1;
	verticalLock = true;

	initUp= aiVector3D(0, 1, 0);
	initFront = leftHanded? aiVector3D(0, 0, 1): aiVector3D(0, 0, -1);
	initRight = aiVector3D(1, 0, 0);

	lockedUp = initUp;
	up = initUp;
	front = initFront;
	right = initRight;

	UpdateTransform();
}

float Transform::DegToRad(float degree)
{
	while (degree > 360)
	{
		degree -= 360;
	}
	while (degree < -360)
	{
		degree += 360;
	}

	degree *= (PI / 180);

	return degree;
}

void Transform::SpinYaw(float degree)
{
	float rad;
	rad = leftHanded ? -DegToRad(degree) : DegToRad(degree);
	aiVector3D axis;
	axis = verticalLock ? lockedUp : up;
	Spin(aiQuaternion(axis, rad));
}

void Transform::SpinPitch(float degree)
{
	float rad;
	rad =DegToRad(degree);
	if (verticalLock)
	{
		float ang = acosf(lockedUp*front);
		if (!ang || rad >= ang || rad <= ang - PI)
			rad = 0;
	}
	rad = leftHanded ? -rad : rad;
	Spin(aiQuaternion(right, rad));

}

void Transform::SpinRoll(float degree)
{
	if (verticalLock)
		return;
	float rad;
	rad = leftHanded ? -DegToRad(degree) : DegToRad(degree);
	Spin(aiQuaternion(front, rad));
}

void Transform::Spin(aiQuaternion q)
{
	rotation = q*rotation;
	UpdateTransform();
}

void Transform::UpdateTransform()
{
	up = rotation.Rotate(initUp);
	right = rotation.Rotate(initRight);
	front = rotation.Rotate(initFront);

	transformMatrix = aiMatrix4x4(rotation.GetMatrix());
	transformMatrix.a1 *= scaling;
	transformMatrix.b1 *= scaling;
	transformMatrix.c1 *= scaling;
	transformMatrix.a2 *= scaling;
	transformMatrix.b2 *= scaling;
	transformMatrix.c2 *= scaling;
	transformMatrix.a3 *= scaling;
	transformMatrix.b3 *= scaling;
	transformMatrix.c3 *= scaling;

	//Column vector matrix
	transformMatrix.a4 = position.x;
	transformMatrix.b4 = position.y;
	transformMatrix.c4 = position.z;

}

void Transform::DirectTurnTo(aiVector3D vec)
{
	vec.Normalize();
	float dot = vec*front;

	//Check parallel
	if (dot > 0.999999)
	{
		return;
	}
	else if (dot < -0.999999)
	{
		rotation = aiQuaternion(up, PI);
		UpdateTransform();
		return;
	}
	aiVector3D axis = front^vec;
	aiQuaternion deta = aiQuaternion(axis, acosf(dot));
	Spin(deta);
}

void Transform::LockedTurnTo(aiVector3D vec)
{
	//Calculate Target Right Vector
	float dot = lockedUp*vec;
	if (dot > 0.999999)
	{
		DirectTurnTo(lockedUp);
		UpdateTransform();
		return;
	}
	else if (dot < -0.999999)
	{
		DirectTurnTo(-lockedUp);
		UpdateTransform();
		return;
	}
	aiVector3D t_right = vec^lockedUp;
	//Left handed system
	if (leftHanded)
		t_right = -t_right;

	t_right.Normalize();

	//Rotate around lockedUp 
	dot = right*t_right;
	//Check parallel
	if (dot > 0.999999)
	{
		//Do nothing
	}
	else if (dot < -0.999999)
	{
		aiQuaternion deta = aiQuaternion(lockedUp, PI);
		//deta.Normalize();
		Spin(deta);
		return;
	}
	else
	{
		aiVector3D axis = right^t_right;
		aiQuaternion deta = aiQuaternion(axis, acosf(dot));
		Spin(deta);
	}

	DirectTurnTo(vec);
}

void Transform::FaceTo(aiVector3D vec)
{
	if (vec.Length() < 0.000001)
		return;
	vec.Normalize();
	if (verticalLock)
	{
		LockedTurnTo(vec);
	}
	else
	{
		DirectTurnTo(vec);
	}

}

void Transform::LookAt(aiVector3D point)
{
	aiVector3D vec = point - position;
	FaceTo(vec);
}

void Transform::SetRotation(aiQuaternion q)
{
	rotation = q;
	UpdateTransform();
}

void Transform::Move(aiVector3D GlobalVec)
{
	position = position + GlobalVec;
	UpdateTransform();
}

void Transform::MoveFront(float distance)
{
	position = position + front*distance;
	UpdateTransform();
}

void Transform::MoveRight(float distance)
{
	position = position + right*distance;
	UpdateTransform();
}

void Transform::MoveUp(float distance)
{
	position = position + up*distance;
	UpdateTransform();
}

void Transform::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	UpdateTransform();
}

void Transform::SetScaling(float size)
{
	scaling = size;
	UpdateTransform();
}

aiVector3D Transform::GetPosition()
{
	return position;
}

float Transform::GetScaling()
{
	return scaling;
}

aiQuaternion Transform::GetRotation()
{
	return rotation;
}

Camera::Camera()
{
	fovY = 90;
	screenAspect = 4.0f / 3.0f;
	zNear = 0.1f;
	zFar = 1000;
	UpdateProjectionMatrix();
}

void Camera::UpdateProjectionMatrix()
{
	if (fovY < 0.00001 || screenAspect < 0.00001)
		return;
	float rad = DegToRad(fovY);
	projectionMatrix = aiMatrix4x4();

	//Column vector matrix
	projectionMatrix.a1 = (leftHanded ? 1.0f : -1.0f) / tanf(0.5f*rad) / screenAspect;
	projectionMatrix.b2 = 1.0f / tanf(0.5f*rad);
	projectionMatrix.c3 = zFar / (zFar - zNear);

	projectionMatrix.d3 = 1;
	projectionMatrix.c4 = -zNear*zFar / (zFar - zNear);


	projectionMatrix.d4 = 0;

	

}

aiMatrix4x4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

aiMatrix4x4 Camera::GetViewMatrix()
{
	aiMatrix4x4 vm = transformMatrix;
	vm.Inverse();
	return vm;
}