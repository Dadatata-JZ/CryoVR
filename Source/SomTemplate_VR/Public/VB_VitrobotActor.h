// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VB_StaticActor.h"
#include "VB_VitrobotActor.generated.h"

class UBoxComponent;
class UAudioComponent;
/**
 * 
 */
UCLASS()
class SOMTEMPLATE_VR_API AVB_VitrobotActor : public AVB_StaticActor
{
	GENERATED_BODY()

public:
	AVB_VitrobotActor();

	UFUNCTION()
	void SetInteractableByRotation(UStaticMeshComponent* SM_Mesh);

	UFUNCTION()
	void MoveWorkstationHolder(float F);

	UPROPERTY(VisibleAnyWhere, Category = "Components")
	UStaticMeshComponent* WorkstationHolder;

	UPROPERTY(VisibleAnyWhere, Category = "Components")
	UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* Plunger_Collider;

	UPROPERTY(VisibleAnyWhere, Category = "Components")
	UStaticMeshComponent* Plunger;

	UPROPERTY(VisibleAnyWhere, Category = "Components")
	UStaticMeshComponent* Screen;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* PlungerSound;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TurnOnMachine(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
	void PlungerDelay();

	virtual void Tick(float DeltaTime) override;



protected:
	

	UPROPERTY(VisibleAnyWhere, Category = "Components")
	UStaticMeshComponent* InnerHolder;

	UPROPERTY(VisibleAnyWhere, Category = "Components")
	UStaticMeshComponent* InnerHolder_Left;

	UPROPERTY(VisibleAnyWhere, Category = "Components")
	UStaticMeshComponent* InnerHolder_Right;

	
	//UPROPERTY(VisibleAnyWhere, Category = "Components")
	//UStaticMeshComponent* Door;
	
	/*UPROPERTY(VisibleAnyWhere, Category = "Components")
	UStaticMeshComponent* LEDCover;*/
	

	UPROPERTY(VisibleAnyWhere, Category = "Components")
	UStaticMeshComponent* Bottom_Cover;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* WorkstationHolder_Collider;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* Door_Collider;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* TestButton_Collider;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* PowerButton_Collider;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UMaterial* MainMaterial;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UMaterial* OptionMaterial;

	UMaterialInstanceDynamic* DynamicMaterialInst;

	UPROPERTY(EditAnywhere)
	AActor* ACT;

	class ALevelScriptActor* GetLevelScriptActor(class ULevel* OwnerLevel = NULL) const;


private:
	bool m_IsMachineOn = false;
	bool bIsInteractable;
	bool bIsButtonOn;
	bool bIsHolderGoingUp;
	bool bIsHolderTouchingBottom;
	bool bIsHolderMovingFoward;
	bool bPlungerStatus;
	bool bIsDoorOn;
	bool bIsDoorGoingOpen;

	int LevelStatus = 0;
	int Status = 1;
	int Counter = 0;
};
