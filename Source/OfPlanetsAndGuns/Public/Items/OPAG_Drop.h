#pragma once

#include "CoreMinimal.h"
#include "Interactables/OPAG_InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "OPAG_Drop.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class EDropType : uint8 {
	Ammo,
	Gold,
	Bolts,
	Potion
};

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Drop : public AActor, public IOPAG_InteractableInterface
{
	GENERATED_BODY()
	
public:	
	AOPAG_Drop();

	UPROPERTY(Category = "Drop Stuff", EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* DropMesh;
	
	UPROPERTY(Category = "Drop Stuff", EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* Sphere;

	UPROPERTY(Category = "Drop", EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* HighlightMesh;

	UPROPERTY(Category = "Drop", EditDefaultsOnly, BlueprintReadWrite)
	float DisablePhysicAfterSeconds;

	UPROPERTY(Category = "Drop", EditDefaultsOnly, BlueprintReadWrite)
	float HighlightIncreaseSizeFactor;

	float CurrentHighlightIncreaseSizeFactor;

	UPROPERTY(Category = "Drop", EditDefaultsOnly, BlueprintReadWrite)
		UTimelineComponent* DropTimeline;

	UPROPERTY(Category = "Drop", EditDefaultsOnly, BlueprintReadWrite)
		class UCurveVector* CurveDropVector;

	UPROPERTY(Category = "Drop", EditDefaultsOnly, BlueprintReadWrite)
		float DropCurveXFactor = 200;

	UPROPERTY(Category = "Drop", EditDefaultsOnly, BlueprintReadWrite)
		float DropCurveZFactor = 100;

	FOnTimelineVector OnDropTimelineProgress;

	UPROPERTY()
		FVector DropTimelinePropertyName;

	UPROPERTY()
		FVector RandomDropDir;

	UPROPERTY()
		FVector SpawnLocation;

public:
	UPROPERTY(Category = "Drop", EditAnywhere, BlueprintReadWrite)
	int Amount = 1;
	
	UPROPERTY(Category = "Drop", EditDefaultsOnly, BlueprintReadWrite)
	EDropType DropType;

	UFUNCTION()
	void DropTimelineProgress(FVector Value);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void ApplyForce_Implementation(const FVector Force) override;
	void FloatingDrop();
	void StartDropTimeline();

};
