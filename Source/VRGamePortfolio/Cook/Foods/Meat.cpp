#include "VRGamePortfolio/Cook/Foods/Meat.h"

AMeat::AMeat()
{
	InitMesh();
}

void AMeat::InitMesh()
{
	MeshPath = "/Game/Meshes/Prototype/PSM_Meat.PSM_Meat";
	SlicedMaterialPath = "/Game/Materials/Prototype/PM_Meat.PM_Meat";
	Super::InitMesh();
}
