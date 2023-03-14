// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
													//Cross Flatform을 위해 습관을 들이자.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));

	//Component 중 하나를 RootComponent로 만든다. 규칙이다. 이유는 아직 모름..
	RootComponent = Mesh;

	//Actor를 여러개 뿌린다고 한다면, 경로 한 번만 load가 되게 끔 static을 붙이는 것도 일반적인 방법이다. 
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
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

