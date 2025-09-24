#include "VRGamePortfolio/Cook/Foods/Ingredient.h"
#include "VRGamePortfolio/Components/GrabSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSourceData.h"
#include "PhysicsEngine/BodySetup.h"

AIngredient::AIngredient()
{
	PrimaryActorTick.bCanEverTick = true;
	this->Tags.Add("Ingredient");

	InitComponents();
}

void AIngredient::BeginPlay()
{
	Super::BeginPlay();

	// Add Grab Component
	GrabSpawner = (AGrabSpawner*)UGameplayStatics::GetActorOfClass(GetWorld(), AGrabSpawner::StaticClass());
	if (GrabSpawner) GrabSpawner->AttachGrabComponent(this);
}

void AIngredient::InitComponents()
{
	ProceMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ShapeMesh"));
	SetProceMeshAttr();
	RootComponent = ProceMesh;
}

void AIngredient::InitMesh()
{
	// Static Mesh ��� Procedural Mesh ��� ����
	if (!MeshPath.IsEmpty()) {
		static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(*MeshPath);
		UStaticMeshComponent* StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		StaticMeshComponent->SetStaticMesh(MeshRef.Object);
		UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(StaticMeshComponent, 0, ProceMesh, true);
		StaticMeshComponent->DestroyComponent();
	}
	// Slice �ܸ� Material ����
	if (!SlicedMaterialPath.IsEmpty()) {
		static ConstructorHelpers::FObjectFinder<UMaterial> MaterialRef(*SlicedMaterialPath);
		SlicedMaterial = MaterialRef.Object;
	}
}

void AIngredient::SetProceMeshAttr()
{
	ProceMesh->SetCollisionProfileName("BlockAll");
	ProceMesh->SetSimulatePhysics(true);
	ProceMesh->SetNotifyRigidBodyCollision(true);
	ProceMesh->bUseComplexAsSimpleCollision = false;
}

void AIngredient::Slice(FVector Position, FVector Normal)
{
	UProceduralMeshComponent* SlicedMesh = NewObject<UProceduralMeshComponent>(this);
	UKismetProceduralMeshLibrary::SliceProceduralMesh(
		ProceMesh,
		Position,
		Normal,
		true, // bCreateOtherHalf
		SlicedMesh,
		EProcMeshSliceCapOption::CreateNewSectionForCap,
		SlicedMaterial
	);

	// �� ��ü ��ȯ
	AIngredient* SlicedActor = GetWorld()->SpawnActor<AIngredient>(AIngredient::StaticClass(), SlicedMesh->GetComponentTransform(), FActorSpawnParameters());
	GrabSpawner->AttachGrabComponent(SlicedActor);

	// collision ������ ���� ���� ����
	TArray<FVector> TotalVertices;
	for (int SectionIndex = 0; SectionIndex < SlicedMesh->GetNumSections(); SectionIndex++) {
		TArray<FVector2D> UV0s;
		TArray<int32> Triangles;
		TArray<FVector> Normals;
		TArray<FVector> Vertices;
		TArray<FProcMeshTangent> Tangents;
		// ProceduralMesh ������ ����
		UKismetProceduralMeshLibrary::GetSectionFromProceduralMesh(
			SlicedMesh,
			SectionIndex,
			Vertices,
			Triangles,
			Normals,
			UV0s,
			Tangents
		);
		// �� Actor Mesh section ����
		SlicedActor->ProceMesh->CreateMeshSection(
			SectionIndex, 
			Vertices, 
			Triangles, 
			Normals, 
			UV0s,
			TArray<FColor>(),
			Tangents,
			true
		);
		TotalVertices.Append(Vertices); // ���� ����
		// Material ����
		SlicedActor->ProceMesh->SetMaterial(SectionIndex, SlicedMesh->GetMaterial(SectionIndex)); 
	}
	// ���� ���� 0�� �ʰ��� Collision ����
	if (TotalVertices.Num() > 0)
	{
		SlicedActor->ProceMesh->ClearCollisionConvexMeshes();
		SlicedActor->ProceMesh->AddCollisionConvexMesh(TotalVertices);
	}
	// Component�� ���
	SlicedActor->ProceMesh->RegisterComponent();
	SlicedActor->ProceMesh->MarkRenderStateDirty(); // ������ ���� ����
}

