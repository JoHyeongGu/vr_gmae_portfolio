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
	// Static Mesh 기반 Procedural Mesh 모양 생성
	if (!MeshPath.IsEmpty()) {
		static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(*MeshPath);
		UStaticMeshComponent* StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		StaticMeshComponent->SetStaticMesh(MeshRef.Object);
		UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(StaticMeshComponent, 0, ProceMesh, true);
		StaticMeshComponent->DestroyComponent();
	}
	// Slice 단면 Material 설정
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

	// 새 객체 소환
	AIngredient* SlicedActor = GetWorld()->SpawnActor<AIngredient>(AIngredient::StaticClass(), SlicedMesh->GetComponentTransform(), FActorSpawnParameters());
	GrabSpawner->AttachGrabComponent(SlicedActor);

	// collision 생성을 위해 정점 수집
	TArray<FVector> TotalVertices;
	for (int SectionIndex = 0; SectionIndex < SlicedMesh->GetNumSections(); SectionIndex++) {
		TArray<FVector2D> UV0s;
		TArray<int32> Triangles;
		TArray<FVector> Normals;
		TArray<FVector> Vertices;
		TArray<FProcMeshTangent> Tangents;
		// ProceduralMesh 데이터 수집
		UKismetProceduralMeshLibrary::GetSectionFromProceduralMesh(
			SlicedMesh,
			SectionIndex,
			Vertices,
			Triangles,
			Normals,
			UV0s,
			Tangents
		);
		// 새 Actor Mesh section 생성
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
		TotalVertices.Append(Vertices); // 정점 수집
		// Material 복사
		SlicedActor->ProceMesh->SetMaterial(SectionIndex, SlicedMesh->GetMaterial(SectionIndex)); 
	}
	// 정점 개수 0개 초과면 Collision 복사
	if (TotalVertices.Num() > 0)
	{
		SlicedActor->ProceMesh->ClearCollisionConvexMeshes();
		SlicedActor->ProceMesh->AddCollisionConvexMesh(TotalVertices);
	}
	// Component로 등록
	SlicedActor->ProceMesh->RegisterComponent();
	SlicedActor->ProceMesh->MarkRenderStateDirty(); // 렌더링 상태 갱신
}

