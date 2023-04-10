// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"

// Sets default values
AMyWeapon::AMyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	

	//Create Weapon StaticMesh
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));


	static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT(
		"StaticMesh'/Game/ParagonGreystone/FX/Meshes/Heroes/Greystone/SM_Greystone_Blade_01.SM_Greystone_Blade_01'")
	);

	if (SW.Succeeded())
	{
		Weapon->SetStaticMesh(SW.Object);
	}

	//RootComponent를 Weapon으로 설정하여 TriggerBox가 Weapon을 따라다니게 끔 설정
	Weapon->SetupAttachment(RootComponent);
	Trigger->SetupAttachment(Weapon);


	//Weapon Collision
	Weapon->SetCollisionProfileName(TEXT("MyCollectible"));
	Trigger->SetCollisionProfileName(TEXT("MyCollectible"));
	//Trigger Collision Size
	Trigger->SetBoxExtent(FVector(30.0f, 30.0f, 30.0f));
	
}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnCharacterOverlap);


}

void AMyWeapon::OnCharacterOverlap(UPrimitiveComponent* Overlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapped"));

	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter)
	{
		FName WeaponSocket(TEXT("hand_l_socket"));

		AttachToComponent(
			MyCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			WeaponSocket
		);
	}
}

// Called every frame
/*oid AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

