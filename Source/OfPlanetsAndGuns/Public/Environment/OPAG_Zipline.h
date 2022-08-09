// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_ZiplineSupport.h"
#include "CableComponent.h"
#include "Characters/Player/OPAG_Player.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "OPAG_Zipline.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Zipline : public AActor , public IOPAG_InteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOPAG_Zipline();

	UPROPERTY(BlueprintReadOnly, Category = "Zipline")
		USceneComponent* Root;

	UPROPERTY(EditAnyWhere, BLueprintReadWrite, Category = "ZiplineSupport | Point A")
		USceneComponent* PointA;

	UPROPERTY(EditDefaultsOnly, BLueprintReadWrite, Category = "ZiplineSupport | Point A")
		UStaticMeshComponent* SupportMeshA;

	UPROPERTY(EditDefaultsOnly, BLueprintReadWrite, Category = "ZiplineSupport | Point A")
		UBoxComponent* InteractableAreaA;

	UPROPERTY(EditDefaultsOnly, BLueprintReadWrite, Category = "ZiplineSupport | Point A")
		USceneComponent* CableAttachPointA;

	UPROPERTY(EditDefaultsOnly, BLueprintReadWrite, Category = "ZiplineSupport | Point A")
		USceneComponent* PlayerAttachPointA;

	UPROPERTY(EditAnyWhere, BLueprintReadWrite, Category = "ZiplineSupport | Point B")
		USceneComponent* PointB;

	UPROPERTY(EditDefaultsOnly, BLueprintReadWrite, Category = "ZiplineSupport | Point B")
		UStaticMeshComponent* SupportMeshB;

	UPROPERTY(EditDefaultsOnly, BLueprintReadWrite, Category = "ZiplineSupport | Point B")
		UBoxComponent* InteractableAreaB;

	UPROPERTY(EditDefaultsOnly, BLueprintReadWrite, Category = "ZiplineSupport | Point B")
		USceneComponent* CableAttachPointB;

	UPROPERTY(EditDefaultsOnly, BLueprintReadWrite, Category = "ZiplineSupport | Point B")
		USceneComponent* PlayerAttachPointB;
	
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
		FVector StartingPoint;

	UPROPERTY(VisibleinstanceOnly, BlueprintReadOnly, Category = "Zipline", meta = (AllowPrivateAccess = "true"))
		FVector EndingPoint;

	UPROPERTY(VisibleinstanceOnly, BlueprintReadOnly, Category = "Zipline" , meta = (AllowPrivateAccess = "true"))
		AOPAG_Player* PlayerRef;

	UPROPERTY(VisibleinstanceOnly, BlueprintReadOnly, Category = "Zipline", meta = (AllowPrivateAccess = "true"))
		uint8 bCanPlayerInteract : 1;

	UFUNCTION(BlueprintCallable)
		void SetUpZipline();

	UFUNCTION()
		void OverlapPointABegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OverlapPointBBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OverlapReset(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
