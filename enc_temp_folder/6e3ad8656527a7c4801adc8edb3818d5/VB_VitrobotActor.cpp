// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

//Output test
//UE_LOG(LogTemp, Log, TEXT("==Step1=="));


#include "VB_VitrobotActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Sound/SoundWave.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "VirtualReality/TP_MotionController.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "VB_WorkstationActor.h"
#include "VB_PenActor.h"
#include "VB_TextActor.h"
#include "VB_LevelScriptActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"



AVB_VitrobotActor::AVB_VitrobotActor() {
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MainMesh(TEXT("StaticMesh'/Game/Test_Geometry/Test_Textures/Vitrobot.Vitrobot'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WorkstationHolder(TEXT("StaticMesh'/Game/Test_Geometry/Test_Textures/Workstation_Holder.Workstation_Holder'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_InnerHolder(TEXT("StaticMesh'/Game/Test_Geometry/Test_Textures/Holder.Holder'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_InnerHolder_L(TEXT("StaticMesh'/Game/Test_Geometry/Test_Textures/HolderArm_Left.HolderArm_Left'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_InnerHolder_R(TEXT("StaticMesh'/Game/Test_Geometry/Test_Textures/HolderArm_Right.HolderArm_Right'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Door(TEXT("StaticMesh'/Game/Test_Geometry/Test_Textures/Door.Door'"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_LEDCover(TEXT("StaticMesh'/Game/Test_Geometry/LED_Cover.LED_Cover'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Plunger(TEXT("StaticMesh'/Game/Test_Geometry/Test_Textures/Plunger.Plunger'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BottomCover(TEXT("StaticMesh'/Game/Test_Geometry/Test_Textures/BottomCover.BottomCover'"));
	static ConstructorHelpers::FObjectFinder<USoundWave> S_Plunger(TEXT("/Game/Test_Geometry/Test_Textures/Sounds/PlungerSound.PlungerSound"));
	static ConstructorHelpers::FObjectFinder<USoundWave> S_WH(TEXT("/Game/Test_Geometry/Test_Textures/Sounds/Workstation_GD.Workstation_GD"));
	static ConstructorHelpers::FObjectFinder<UMaterial> M_MainMaterial(TEXT("/Game/Test_Geometry/Test_Textures/Screen_Shot_1.Screen_Shot_1"));
	static ConstructorHelpers::FObjectFinder<UMaterial> M_OptionMaterial(TEXT("/Game/Test_Geometry/Test_Textures/Screen_Shot_2.Screen_Shot_2"));

	if (SM_MainMesh.Succeeded()) {
		meshComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		meshComp->SetStaticMesh(SM_MainMesh.Object);
		shapeComp->DestroyComponent();
	}

	//#1 WorkStation Holder and its collider
	WorkstationHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Workstation_Holder"));
	WorkstationHolder->SetupAttachment(meshComp);
	WorkstationHolder->SetGenerateOverlapEvents(true);
	WorkstationHolder->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WorkstationHolder->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WorkstationHolder->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	WorkstationHolder->SetVisibility(true);
	WorkstationHolder->SetRelativeLocation(FVector(3.73f, 0.0f, 8.33f));
	if (SM_WorkstationHolder.Succeeded()) {
		WorkstationHolder->SetStaticMesh(SM_WorkstationHolder.Object);
	}
	WorkstationHolder_Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Workstation_Holder_Collider"));
	WorkstationHolder_Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WorkstationHolder_Collider->SetupAttachment(WorkstationHolder);
	WorkstationHolder_Collider->SetRelativeLocation(FVector(-20.6f, 0.0f, 1.22f));
	Cast<UBoxComponent>(WorkstationHolder_Collider)->SetBoxExtent(FVector(8.7f, 8.7f, 1.3f));
	//WorkstationHolder->OnComponentBeginOverlap.AddDynamic(this, &AVB_VitrobotActor::OnOverlapBegin);


	//#2 InnerHolder
	InnerHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Inner_Holder"));
	InnerHolder->SetupAttachment(meshComp);
	InnerHolder->SetGenerateOverlapEvents(true);
	InnerHolder->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InnerHolder->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InnerHolder->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	InnerHolder->SetVisibility(true);
	InnerHolder->SetRelativeLocation(FVector(-17.0f, 0.0f, 62.63f));
	if (SM_InnerHolder.Succeeded()) {
		InnerHolder->SetStaticMesh(SM_InnerHolder.Object);
	}
	bIsHolderGoingUp = true;
	InnerHolder_Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Inner_Holder_L"));
	InnerHolder_Left->SetupAttachment(InnerHolder);
	InnerHolder_Left->SetRelativeLocation(FVector(0.0f, 0.0f, -7.82f));
	InnerHolder_Left->SetRelativeRotation(FRotator(0.0f, 0.0f, 5.0f));
	if (SM_InnerHolder_L.Succeeded()) {
		InnerHolder_Left->SetStaticMesh(SM_InnerHolder_L.Object);
	}
	InnerHolder_Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Inner_Holder_R"));
	InnerHolder_Right->SetupAttachment(InnerHolder);
	InnerHolder_Right->SetRelativeLocation(FVector(0.0f, 0.0f, -7.82f));
	InnerHolder_Right->SetRelativeRotation(FRotator(0.0f, 0.0f, -5.0f));
	if (SM_InnerHolder_R.Succeeded()) {
		InnerHolder_Right->SetStaticMesh(SM_InnerHolder_R.Object);
	}

	//#3 Door
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(meshComp);
	Door->SetGenerateOverlapEvents(true);
	Door->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Door->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Door->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Door->SetVisibility(true);
	Door->SetRelativeLocation(FVector(-22.010479f, -13.1936617f, 50.48f));
	Door->SetRelativeRotation(FRotator(0.0f));
	if (SM_Door.Succeeded()) {
		Door->SetStaticMesh(SM_Door.Object);
	}
	Door_Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Door_Collider"));
	Door_Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Door_Collider->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Door_Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Door_Collider->SetupAttachment(Door);
	Door_Collider->SetRelativeLocation(FVector(-2.8f, 24.70f, 0.0f));
	Cast<UBoxComponent>(Door_Collider)->SetBoxExtent(FVector(1.3f, 1.3f, 12.7f));
	Door_Collider->OnComponentBeginOverlap.AddDynamic(this, &AVB_VitrobotActor::OnOverlapBegin);

	bIsDoorOn = false;
	bIsDoorGoingOpen = true;

	//Plunger
	Plunger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plunger"));
	Plunger->SetupAttachment(meshComp);
	Plunger->SetGenerateOverlapEvents(true);
	Plunger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Plunger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Plunger->SetVisibility(true);
	Plunger->SetRelativeLocation(FVector(-17.0f, 0.0f, 67.620285f));
	Plunger->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	if (SM_Plunger.Succeeded()) {
		Plunger->SetStaticMesh(SM_Plunger.Object);
	}
	Plunger_Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Plunger_Collider"));
	Plunger_Collider->SetupAttachment(Plunger);
	Plunger_Collider->SetRelativeLocation(FVector(0.0f, 0.0f, -32.33f));
	Cast<UBoxComponent>(Plunger_Collider)->SetBoxExtent(FVector(0.8f, 0.8f, 0.8f));
	
	//LED Screen
	Screen = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Screen"));
	Screen->SetupAttachment(meshComp);
	Screen->SetVisibility(true);
	Screen->SetRelativeLocation(FVector(-23.267395f,0.0f,74.93f));
	Screen->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));
	Screen->SetRelativeScale3D(FVector(0.19f, 0.13f, 1.0f));
	Screen->OnComponentBeginOverlap.AddDynamic(this, &AVB_VitrobotActor::TouchScreen);

	//#Power button test version
	PowerButton_Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("PowerButton_Collider"));
	PowerButton_Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	PowerButton_Collider->SetupAttachment(Screen);
	PowerButton_Collider->SetRelativeLocation(FVector(-24.4023972f, 5.3110352f, 74.2524109f));
	Cast<UBoxComponent>(PowerButton_Collider)->SetBoxExtent(FVector(3.0f, 3.0f, 3.0f));
	PowerButton_Collider->SetHiddenInGame(false);
	PowerButton_Collider->OnComponentBeginOverlap.AddDynamic(this, &AVB_VitrobotActor::TurnOnMachine);

	//Test button on the LED screen
	TestButton_Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("TestButton_Collider"));
	TestButton_Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	TestButton_Collider->SetupAttachment(Screen);
	TestButton_Collider->SetRelativeLocation(FVector(-27.5f, -6.827611f, 76.4f));
	Cast<UBoxComponent>(TestButton_Collider)->SetBoxExtent(FVector(3.0f, 1.0f, 3.0f));
	TestButton_Collider->SetHiddenInGame(false);
	TestButton_Collider->OnComponentBeginOverlap.AddDynamic(this, &AVB_VitrobotActor::OnOverlapBegin);
	bIsHolderTouchingBottom = true;

	//Bottom Cover
	Bottom_Cover = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottom_Cover"));
	Bottom_Cover->SetupAttachment(meshComp);
	Bottom_Cover->SetGenerateOverlapEvents(true);
	Bottom_Cover->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Bottom_Cover->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Bottom_Cover->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Bottom_Cover->SetVisibility(true);
	Bottom_Cover->SetRelativeLocation(FVector(-13.13f, 0.0f, 37.63f));
	Bottom_Cover->SetRelativeRotation(FRotator(0.0f, -16.0f, 0.0f));
	if (SM_BottomCover.Succeeded()) {
		Bottom_Cover->SetStaticMesh(SM_BottomCover.Object);
	}


	//Sounds & Materials Setting
	USoundWave* SoundWave = S_Plunger.Object;
	PlungerSound = CreateAbstractDefaultSubobject<UAudioComponent>(TEXT("Audio_Plunger"));
	PlungerSound->SetupAttachment(meshComp);
	PlungerSound->SetAutoActivate(false);
	PlungerSound->SetSound(SoundWave);

	USoundWave* SoundWave1 = S_WH.Object;
	Holder_Sound = CreateAbstractDefaultSubobject<UAudioComponent>(TEXT("Audio_WH"));
	Holder_Sound->SetupAttachment(meshComp);
	Holder_Sound->SetAutoActivate(false);
	Holder_Sound->SetSound(SoundWave1);

	MainMaterial = CreateDefaultSubobject<UMaterial>(TEXT("MainMaterial"));
	if (M_MainMaterial.Succeeded())
	{
		MainMaterial = M_MainMaterial.Object;
	}
	OptionMaterial = CreateDefaultSubobject<UMaterial>(TEXT("OptionMaterial"));
	if (M_OptionMaterial.Succeeded())
	{
		OptionMaterial = M_OptionMaterial.Object;
	}
	DynamicMaterialInst = UMaterialInstanceDynamic::Create(OptionMaterial, meshComp);
}
		
void AVB_VitrobotActor::TurnOnMachine(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AVB_PenActor>(OtherActor))
	{	
		m_IsMachineOn = !m_IsMachineOn;	
	}
}
void AVB_VitrobotActor::TouchScreen(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{	
	AVB_LevelScriptActor* LSA = Cast<AVB_LevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (Cast<ATP_MotionController>(OtherActor) != nullptr)
	{
		//if (LSA->GetStatus() == 16 || LSA->GetStatus() == 23)
		//{
			bIsZooming = true;
		//}
	}
}
//Set if the cover is interactable by rotation vector
void AVB_VitrobotActor::SetInteractableByRotation(UStaticMeshComponent* SM_Mesh)
{
	FRotator TempRotator = SM_Mesh->GetComponentRotation();
	float TempYaw = TempRotator.Yaw;
}

//Move the workstation holder in specific speed
void AVB_VitrobotActor::MoveWorkstationHolder(float F)
{
	float addz = 1.0f;
	addz = addz * F;
	WorkstationHolder->AddWorldOffset(FVector(0.0f, 0.0f, addz));

}

//Generate the overlap function for bottom cover and workstation holder
void AVB_VitrobotActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (Cast<AVB_PenActor>(OtherActor) != nullptr)	
	{		
		
		//Set the Button ON/OFF
		if (OverlappedComp == TestButton_Collider)
		{	
			bIsButtonOn = !bIsButtonOn;
		}
		if (OverlappedComp == Door_Collider)
		{
			bIsDoorOn = !bIsDoorOn;
		}
	}
}

void AVB_VitrobotActor::PlungerDelay()
{	
	float PlungerPosition = Plunger->GetComponentLocation().Z;
	if (m_IsMachineOn)
	{
		if (PlungerPosition > 209.963f)
		{
			bPlungerStatus = true;
			m_IsMachineOn = false;
		}
		else if (PlungerPosition < 185.0f)
		{
			bPlungerStatus = false;
			m_IsMachineOn = false;
			Status = 1;
		}
		else if (PlungerPosition > 203.836685f && Status == 1)
		{
			AVB_LevelScriptActor* LSA = Cast<AVB_LevelScriptActor>(GetWorld()->GetLevelScriptActor());
			if (LSA->GetStatus() == 15)
			{
				LSA->SetStatus(16);
			}
			m_IsMachineOn = false;
			Status = 0;
		}

		if (bPlungerStatus)
		{
			Plunger->AddWorldOffset(FVector(0.0f, 0.0f, -0.07f));

		}
		else if (!bPlungerStatus)
		{
			Plunger->AddWorldOffset(FVector(0.0f, 0.0f, 0.07f));
		}
	}
}

void AVB_VitrobotActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AVB_LevelScriptActor* LSA = Cast<AVB_LevelScriptActor>(GetWorld()->GetLevelScriptActor());
	UE_LOG(LogTemp, Log, TEXT("==%f=="), WorkstationHolder->GetComponentLocation().Z);
	float PlungerPosition = Plunger->GetComponentLocation().Z;
	if(PlungerPosition > 209.0f)
	{
		Counter++;
	}
	PlungerDelay();
	if (bIsButtonOn)
	{	
		//Holder range(Component location) Z:(125.68->147.68)
		if (WorkstationHolder->GetComponentLocation().Z < 127.0f)
		{
			bIsHolderGoingUp = true;
			bIsButtonOn = false;
		}
		else if (WorkstationHolder->GetComponentLocation().Z > 145.5f)
		{
			bIsHolderGoingUp = false;
			bIsButtonOn = false;
			if (LSA != nullptr)
			{
				if (LSA->GetStatus() == 22)
				{
					LSA->SetStatus(23);
				}
			}
		}

		if (bIsHolderGoingUp)
		{
			WorkstationHolder->AddWorldOffset(FVector(0.0f, 0.0f, 0.07f));
		}
		else if (!bIsHolderGoingUp)
		{
			WorkstationHolder->AddWorldOffset(FVector(0.0f, 0.0f, -0.07f));
		}
		
	}
	if (bIsButtonOn && WorkstationHolder->GetComponentLocation().Z < 128.0f)
	{
		Holder_Sound->Play();
	}
	else if (bIsButtonOn && WorkstationHolder->GetComponentLocation().Z > 145.0f)
	{
		Holder_Sound->Stop();
	}

	/*if (bIsDoorOn) {
		if (Door->GetComponentRotation().Yaw < -0.5f) 
		{
			bIsDoorGoingOpen = true;
			bIsDoorOn = false;
		}
		else if (Door->GetComponentRotation().Yaw > 91.0f) 
		{
			bIsDoorGoingOpen = false;
			bIsDoorOn = false;
		}

		if (bIsDoorGoingOpen) {
			Door->AddWorldRotation(FRotator(0.0f, 1.0f, 0.0f));
		}
		else if (!bIsDoorGoingOpen) {
			Door->AddWorldRotation(FRotator(0.0f, -1.0f, 0.0f));
		}
	}*/


	if (Counter >= 180 && Counter <= 300)
	{
		InnerHolder_Left->AddRelativeRotation(FRotator(0.0f, 0.0f, -0.125f));
		InnerHolder_Right->AddRelativeRotation(FRotator(0.0f, 0.0f, 0.125f));
	}
	else if (Counter >= 600 && Counter <= 720)
	{
		InnerHolder_Left->AddRelativeRotation(FRotator(0.0f, 0.0f, 0.125f));
		InnerHolder_Right->AddRelativeRotation(FRotator(0.0f, 0.0f, -0.125f));
	}
	else if (Counter == 880)
	{
		PlungerSound->Play();
	}
	else if (Counter >= 920 && Counter < 1320)
	{
		Plunger->SetRelativeLocation(FVector(-17.0f, 0.0f, 73.0f));
		Counter++;
	}
	else if (Counter >= 1320 && Counter <= 1580)
	{
		Plunger->AddLocalOffset(FVector(0.0f, 0.0f, -0.07f));
		WorkstationHolder->AddLocalOffset(FVector(0.0f, 0.0f, -0.07f));
		Counter++;
	}
	
	if (Counter == 1300)
	{
		Holder_Sound->Play();
	}
	if (Counter == 1580)
	{
		Holder_Sound->Stop();
		if (LSA->GetStatus() == 23)
		{
			LSA->SetStatus(24);
		}
	}

	if (bIsZooming == true)
	{	
		Screen->AddLocalOffset(FVector(0.0f, 0.0f, 2.0f*ZoomingDirection));
		float TempScale = Screen->GetComponentScale().X;
		if (ZoomingDirection > 0)
		{
			TempScale = TempScale * 1.05;
		}
		else
		{
			TempScale = TempScale / 1.05;
		}
		Screen->SetRelativeScale3D(FVector(TempScale));
		ZoomingTime++;
	}
	if (ZoomingTime >= 10)
	{	
		ZoomingTime = 0;
		bIsZooming = false;
		ZoomingDirection = ZoomingDirection * -1;
	}
}