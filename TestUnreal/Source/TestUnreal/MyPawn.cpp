// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));

	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT
	("StaticMesh'/Game/StarterContent/Props/SM_Couch.SM_Couch'"));

	if (SM.Succeeded())
	{
		Mesh->SetStaticMesh(SM.Object);
	}
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Axis:축(JoyStick), Action:Button
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyPawn::LeftRight);
}

//Setting, 입력파트의 Binding부분, 축매핑에서 함수의 이름과 동일하게 설정해야하며,
//맵핑 값을 지정한다. 지정된 값이 Value가 된다.
//Unity의 경우 Transform을 가져와 사용하였는데, 단 점은 유동적이지 않고, 상황마다 값을 변경시켜주어야 한다.
//Unreal은 FPS기반에서 나왔기 때문에 영향을 많이 받았다.
void AMyPawn::UpDown(float Value)
{
	if (Value == 0.0f)
	{
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("UpDown %f"), Value);

	AddMovementInput(GetActorForwardVector(), Value);

}

void AMyPawn::LeftRight(float Value)
{
	if (Value == 0.0f)
	{
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("LeftRight %f"), Value);
	AddMovementInput(GetActorRightVector(), Value);
}

