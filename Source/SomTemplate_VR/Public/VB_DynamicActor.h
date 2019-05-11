// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VirtualReality/TP_PickupCube.h"
#include "VB_DynamicActor.generated.h"

/**
 * 
 */
UCLASS()
class SOMTEMPLATE_VR_API AVB_DynamicActor : public ATP_PickupCube
{
	GENERATED_BODY()
public:
	//Set default
	AVB_DynamicActor();
	//Pick up interface
	virtual void Pickup_Implementation(class USceneComponent* AttachTo);
	//Set whether the object is pickable or not
	void setPickable(bool newPickable);
	bool getPickable();

protected:
	//Define whether this object can be picked up
	UPROPERTY(EditInstanceOnly, Category = "Behavior")
	bool m_isPickable;

};
