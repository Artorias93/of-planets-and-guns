#pragma once

#include "CoreMinimal.h"
#include "Interactables/OPAG_InteractableInterface.h"
#include "GameFramework/Actor.h"
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

public:
	UPROPERTY(Category = "Drop Stuff", EditAnywhere, BlueprintReadWrite)
	int Amount = 1;
	
	UPROPERTY(Category = "Drop Stuff", EditDefaultsOnly, BlueprintReadWrite)
	EDropType DropType;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void ApplyForce_Implementation(const FVector Force) override;

};
