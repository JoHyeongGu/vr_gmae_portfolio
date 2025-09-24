#include "VRGamePortfolio/Cook/Tools/Knife.h"
#include "VRGamePortfolio/Components/GrabSpawner.h"
#include "VRGamePortfolio/Cook/Foods/Ingredient.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AKnife::AKnife()
{
	PrimaryActorTick.bCanEverTick = true;
	InitComponents();
	BindCollisionFunction();
}

void AKnife::BeginPlay()
{
	Super::BeginPlay();

	// Add Grab Component
	AActor* GrabSpawner = UGameplayStatics::GetActorOfClass(GetWorld(), AGrabSpawner::StaticClass());
	if (GrabSpawner) ((AGrabSpawner*)GrabSpawner)->AttachGrabComponent(this);
}

void AKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKnife::InitComponents()
{
	// Load Assets
	const FString KnifeMeshPath = "/Game/Meshes/Prototype/PSM_Knife.PSM_Knife";
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KnifeMeshRef(*KnifeMeshPath);

	// Set Component
	KnifeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KnifeMesh"));
	KnifeMesh->SetStaticMesh(KnifeMeshRef.Object);
	KnifeMesh->SetCollisionProfileName("BlockAll");
	KnifeMesh->SetSimulatePhysics(true);
	KnifeMesh->SetNotifyRigidBodyCollision(true);
	RootComponent = KnifeMesh;

	// Create Collision
	SliceCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SliceCollision"));
	SliceCollision->SetupAttachment(KnifeMesh);
	SliceCollision->InitBoxExtent(FVector(18.2f, 0.1f, 1.7f));
	SliceCollision->SetRelativeLocation(FVector(12.8f, 0.f, -6.f));
	SliceCollision->SetCollisionProfileName("OverlapAll");
	SliceCollision->ComponentTags.Add("KnifeSlice");
}

void AKnife::BindCollisionFunction()
{
	FScriptDelegate OverlapDelegate;
	OverlapDelegate.BindUFunction(this, FName("SliceCollisionStart"));
	SliceCollision->OnComponentBeginOverlap.Add(OverlapDelegate);

	OverlapDelegate.BindUFunction(this, FName("SliceCollisionEnd"));
	SliceCollision->OnComponentEndOverlap.Add(OverlapDelegate);
}

void AKnife::SliceCollisionStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Ingredient Ãæµ¹ ½Ã, Slice
	if (OtherActor->ActorHasTag("Ingredient"))
	{
		const FVector Position = SliceCollision->GetComponentLocation();
		const FVector Normal = SliceCollision->GetRightVector();
		AIngredient* Ingredient = (AIngredient*)OtherActor;
		Ingredient->Slice(Position, Normal);
	}
}

void AKnife::SliceCollisionEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("Collision End"));
}
