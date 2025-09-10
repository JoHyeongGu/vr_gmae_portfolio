#include "VRGamePortfolio/Cook/Foods/Ingredient.h"
#include "VRGamePortfolio/Components/GrabSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSourceData.h"

AIngredient::AIngredient()
{
	PrimaryActorTick.bCanEverTick = true;
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
	ProceMesh->SetCollisionProfileName("BlockAll");
	ProceMesh->SetSimulatePhysics(true);
	ProceMesh->SetNotifyRigidBodyCollision(true);
	ProceMesh->bUseComplexAsSimpleCollision = false;
	RootComponent = ProceMesh;

	// Static Mesh 기반 Procedural Mesh 모양 생성
	if (!MeshPath.IsEmpty()) {
		static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(*MeshPath);
		CopyProceduralMeshFromStaticMesh(ProceMesh, MeshRef.Object);
	}
	// Slice 단면 Material 설정
	if (!SlicedMaterialPath.IsEmpty()) {
		static ConstructorHelpers::FObjectFinder<UMaterial> MaterialRef(*SlicedMaterialPath);
		SlicedMaterial = MaterialRef.Object;
	}

}

void AIngredient::CopyProceduralMeshFromStaticMesh(UProceduralMeshComponent* ProceduralMesh, UStaticMesh* SourceMesh)
{
    const TArray<FStaticMaterial> MaterialArray = SourceMesh->GetStaticMaterials();
    const FStaticMeshRenderData* RenderData = SourceMesh->GetRenderData();

	TArray<FVector> CollisionVertices;
    for (int32 LODIndex = 0; LODIndex < RenderData->LODResources.Num(); LODIndex++)
    {
        const FStaticMeshSectionArray Sections = RenderData->LODResources[LODIndex].Sections;
        for (int32 SectionIndex = 0; SectionIndex < Sections.Num(); SectionIndex++)
        {
            // Section 정보 할당
            TArray<int32> Triangles;
			TArray<FVector2D> UVs;
			TArray<FVector> Normals;
            TArray<FVector> Vertices;
			TArray<FProcMeshTangent> Tangents;
            UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(SourceMesh, LODIndex, SectionIndex, Vertices, Triangles, Normals, UVs, Tangents);

            // 새로운 Section 생성
            ProceduralMesh->CreateMeshSection(SectionIndex, Vertices, Triangles, Normals, UVs, TArray<FColor>(), Tangents, true);

            // Material 복사
            const FStaticMeshSection& Section = Sections[SectionIndex];
            ProceduralMesh->SetMaterial(SectionIndex, MaterialArray[Section.MaterialIndex].MaterialInterface);

			CollisionVertices.Append(Vertices);
        }
    }
	// Set Collision
    ProceduralMesh->ClearCollisionConvexMeshes();
    ProceduralMesh->AddCollisionConvexMesh(CollisionVertices);
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
	SlicedMesh->DestroyComponent();
}

