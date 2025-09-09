#include "VRGamePortfolio/Cook/Foods/Ingredient.h"

#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSourceData.h"

AIngredient::AIngredient()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AIngredient::InitComponents()
{
    if (MeshPath.IsEmpty()) return;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(*MeshPath);

	ProceMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ShapeMesh"));
	ProceMesh->SetCollisionProfileName("BlockAll");
	ProceMesh->bUseComplexAsSimpleCollision = false;
	ProceMesh->SetSimulatePhysics(true);
	ProceMesh->SetNotifyRigidBodyCollision(true);
	CopyProceduralMeshFromStaticMesh(ProceMesh, MeshRef.Object);
	RootComponent = ProceMesh;
}

void AIngredient::CopyProceduralMeshFromStaticMesh(UProceduralMeshComponent* ProceduralMesh, UStaticMesh* SourceMesh)
{
    const TArray<FStaticMaterial> MaterialArray = SourceMesh->GetStaticMaterials();
    const FStaticMeshRenderData* RenderData = SourceMesh->GetRenderData();

    TArray<FVector2D> UVs;
    TArray<FVector> Normals;
    TArray<FProcMeshTangent> Tangents;

    for (int32 LODIndex = 0; LODIndex < RenderData->LODResources.Num(); LODIndex++)
    {
        const FStaticMeshSectionArray Sections = RenderData->LODResources[LODIndex].Sections;
        for (int32 SectionIndex = 0; SectionIndex < Sections.Num(); SectionIndex++)
        {
            // Section 정보 할당
            TArray<int32> Triangles;
            TArray<FVector> Vertices;
            UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(SourceMesh, LODIndex, SectionIndex, Vertices, Triangles, Normals, UVs, Tangents);

            // 새로운 Section 생성
            ProceduralMesh->CreateMeshSection(SectionIndex, Vertices, Triangles, Normals, UVs, TArray<FColor>(), Tangents, true);

            // Material 복사
            const FStaticMeshSection& Section = Sections[SectionIndex];
            ProceduralMesh->SetMaterial(SectionIndex, MaterialArray[Section.MaterialIndex].MaterialInterface);
        }
    }
    TArray<FVector> CollisionVertices;
    TArray<int32> CollisionTriangles;
    UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(SourceMesh, 0, 0, CollisionVertices, CollisionTriangles, Normals, UVs, Tangents);
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

	// FTransform MeshTransform = SlicedMesh->GetComponentTransform();
	// MeshTransform.SetScale3D(FVector(0.99f, 0.99f, 0.99f));

	//FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;
	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//AJewel* NewJewel = GetWorld()->SpawnActor<AJewel>(AJewel::StaticClass(), MeshTransform, SpawnParams);
	//NewJewel->CopyProceduralMeshFromAnother(SlicedMesh);

	//GrabSpawner->AttachGrabComponent(NewJewel);

	//SlicedMesh->DestroyComponent();

	//JewelMesh->SetMaterial(JewelMesh->GetNumMaterials(), SlicedMaterial);
}