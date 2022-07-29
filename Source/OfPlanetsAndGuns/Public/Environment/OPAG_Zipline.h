// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interactables/OPAG_InteractableInterface.h"
#include "OPAG_ZiplineSupport.h"
#include "../../../../../../../../../../Program Files/Epic Games/UE_4.27/Engine/Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "../Characters/Player/OPAG_Player.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OPAG_Zipline.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Zipline : public AActor, public IOPAG_InteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOPAG_Zipline();

	UPROPERTY(BlueprintReadOnly, Category = "Zipline")
		USceneComponent* Root;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Zipline | Supports")
		AOPAG_ZiplineSupport *  PointA;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Zipline | Supports")
		AOPAG_ZiplineSupport* PointB;
	
	UPROPERTY(BlueprintReadOnly , Category = "Zipline")
		UCableComponent* Cable;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Interact();
	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool CanInteract();
	virtual bool CanInteract_Implementation() override;

private: 
	UPROPERTY(VisibleinstanceOnly, BlueprintReadOnly, Category = "Zipline" , meta = (AllowPrivateAccess = "true"))
		uint8 bCanPlayerInteract : 1;

	UPROPERTY(VisibleinstanceOnly, BlueprintReadOnly, Category = "Zipline | Supports" , meta = (AllowPrivateAccess = "true"))
		AOPAG_ZiplineSupport* StartingPoint;

	UPROPERTY(VisibleinstanceOnly, BlueprintReadOnly, Category = "Zipline | Supports", meta = (AllowPrivateAccess = "true"))
		AOPAG_ZiplineSupport* EndingPoint;

	UPROPERTY(VisibleinstanceOnly, BlueprintReadOnly, Category = "Zipline | Supports" , meta = (AllowPrivateAccess = "true"))
		AOPAG_Player* PlayerRef;

	UFUNCTION(BlueprintCallable)
		void SetUpZipline();

	UFUNCTION(BlueprintCallable)
		void OverlapReset();

	UFUNCTION()
		void OverlapPointABegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OverlapPointBBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
