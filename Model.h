//--------------------Model Class with Animation and Camera Class------------------
//---Model file importer and matrix calculation logic currently depend on Assimp---
//---This part is platform independent

#pragma once
#include <vector>
#include <map>
//Assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include"Texture.h"
#include"Material.h"
using namespace std;

//-------------------------------Skeleton Structure----------------------------------
class Node
{
public:
	string name;
	int id;
	aiMatrix4x4 localTransformMatrix;
	//aiMatrix4x4 globalTransformMatrix;
	int parentID;
	vector<int> childrenID;
	Node();
};

class NodeList : public vector<Node>
{
public:
	void GetGlobalMatrix(vector<aiMatrix4x4> &outNodeGlobals);
private:
	void GetGlobalMatrixRecur(const aiMatrix4x4 &parentGlobal, int nodeID, vector<aiMatrix4x4> &outNodeGlobals);
};

//Bones indicate how a mesh bind with a node A.K.A "bone offsetMatrix"
class BindingBone
{
public:
	string name;
	int nodeID;
	aiMatrix4x4 offset;
	BindingBone();
};

//-------------------------------Animation ----------------------------------
//Keyframe: time-value pair
class VecKey
{
public:
	aiVector3D value;
	double timeFrame;
	VecKey();
};

class QuatKey
{
public:
	aiQuaternion value;
	double timeFrame;
	QuatKey();
};

//Describe a node use position, scaling and rotation
class NodeFrame
{
public:
	aiVector3D position;
	aiVector3D scaling;
	aiQuaternion rotation;
	aiMatrix4x4 ToMatrix();
};

//NodeAnimation stores  animation Keyframe for single node. Provide evaluate method
class NodeAnimation
{
public:
	vector<VecKey> positionKeys;
	vector<VecKey> scalingKeys;
	vector<QuatKey> rotationKeys;
	double tickDuration;
	int nodeID;
	NodeFrame Evaluate(double tick);

	NodeAnimation();
	void Clear();
protected:
	static aiVector3D EvaluateVecKey(vector<VecKey> &keys, double tick);
	static aiQuaternion EvaluateQuatKey(vector<QuatKey> &keys, double tick);

};

//Animation stores single animation
class Animation
{
public:
	string name;
	vector<NodeAnimation> nodeAnimationList;
	double duration;
	double ticksPerSecond;
	Animation();
};
//-------------------------------Model ----------------------------------
class Mesh
{
public:
	//Vertex data: can be deleted by Purge() after been loaded to graphic memory;
	vector<aiVector3D> vertexPositions;
	vector<aiVector3D> vertexNormals;
	vector<aiVector3D> vertexTangent;
	vector<aiVector3D> vertexBitangent;
	vector<aiVector2D> vertexTexCoords;
	vector<unsigned int> vertexBindID;
	vector<float> vertexBindWight;
	vector<unsigned int> indices;

	//Other data will not be deleted by Purge();
	vector<BindingBone> boneList;
	int materialID;
	int nodeID;
	string name;

	Mesh();
	void Purge();
};

class Model;
class Instance;
class Transform;

class Model
{
	friend class Instance;
public:
	string modelFilePath;
	NodeList nodeList;
	vector<Animation> animationList;
	vector<Mesh> meshList;
	vector<Material> materialList;
	map<int, Instance> instances;
	bool hasAnimation;

	Instance* CreateInstance();
	void DeletInstance(unsigned int instanceID);
	unsigned int GetVisibleInstancesNum();
	unsigned int  UpdateVisableInstances();
	void Purge();

	bool LoadFileD3D(string filePath);
	bool LoadFileD3D(string filePath, int maxBonePerVertex);
protected:
	int maxBonePerVertex;
	map<string, int> nameIDTable;
	map<unsigned int, int> meshNodeTable;
	void ApplyAnimation(unsigned int animationID, double animationTime, vector<aiMatrix4x4> &outNodeGlobals);

	bool Load(const aiScene *source, int maxBonePerVertex);
	void LoadAnimationList(const aiScene *source);
	void LoadMesh(const aiScene *source);
	void LoadNodeListRecur(int parentID, aiNode *ainode);
	void LoadMaterial(const aiScene *source);
};

class Transform
{
public:

	static const bool leftHanded = true;
	aiMatrix4x4 transformMatrix;
	//For Object On Ground
	bool verticalLock;

	Transform();

	void SetPosition(float x, float y, float z);
	void SetScaling(float size);
	void SetRotation(aiQuaternion q);

	aiVector3D GetPosition();
	float GetScaling();
	aiQuaternion GetRotation();

	void SpinYaw(float degree);
	void SpinPitch(float degree);
	void SpinRoll(float degree);

	void Move(aiVector3D GlobalVec);
	void MoveFront(float distance);
	void MoveRight(float distance);
	void MoveUp(float distance);

	void Spin(aiQuaternion q);

	void FaceTo(aiVector3D vec);
	void LookAt(aiVector3D point);

	//Degree to Radains
	static float DegToRad(float degree);
	
protected:

	aiVector3D position;
	float scaling;

	//Initial orientation
	aiVector3D initUp;
	aiVector3D initFront;
	aiVector3D initRight;

	//Rotaion quaternion against  initial orientation
	aiQuaternion rotation;

	//Current orientation
	aiVector3D up;
	aiVector3D front;
	aiVector3D right;

	//Vertical Vector (Ground Normal)
	aiVector3D lockedUp;

	//Turn to a direction with shortist arc
	void DirectTurnTo(aiVector3D vec);
	//Turn to a direction with feet still on the ground.
	void LockedTurnTo(aiVector3D vec);

	void UpdateTransform();
};

class Instance : public Transform
{
public:

	vector<aiMatrix4x4> nodesGlobal;
	unsigned int animationID;
	double animationTime;
	aiColor4D color;
	bool visable;

	Instance();
	Instance(Model *model, unsigned int id);
	double GetAnimationDuration();
	unsigned int GetMeshNumber();
	double GetAnimationNumber();
	unsigned int GetinstanceID();
	Model* GetModelPtr();

	void Apply();
	void ApplyAnimation();

	vector<aiMatrix4x4> GetBindMatrix(unsigned int meshID);
protected:
	unsigned int instanceID;
	Model *model;
};

class Camera :public Transform
{
public:
	float fovY;
	float screenAspect;
	float zNear;
	float zFar;

	Camera();
	void UpdateProjectionMatrix();
	aiMatrix4x4 GetProjectionMatrix();
	aiMatrix4x4 GetViewMatrix();

private:
	aiMatrix4x4 projectionMatrix;
};