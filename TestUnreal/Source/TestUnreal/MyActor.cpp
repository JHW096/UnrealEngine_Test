// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
													//Cross Flatform�� ���� ������ ������.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));

	//Component �� �ϳ��� RootComponent�� �����. ��Ģ�̴�. ������ ���� ��..
	RootComponent = Mesh;

	//Actor�� ������ �Ѹ��ٰ� �Ѵٸ�, ��� �� ���� load�� �ǰ� �� static�� ���̴� �͵� �Ϲ����� ����̴�. 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Couch.SM_Couch'"));

	if (SM.Succeeded())
	{
		Mesh->SetStaticMesh(SM.Object);
	}
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
	//start
	//Log ī�װ�, �α� ����, ����, ����
	//You can find log - Saved file .log 
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay %d"), 3);
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Error, TEXT("Tick %f"), DeltaTime);

	AddActorLocalRotation(FRotator(RotateSpeed * DeltaTime, 0.0f, 0.0f));
}

