// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/Capsulecomponent.h"
#include "MyAnimInstance.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	//RootComponent가 있고, 나머지를 조립한다.
	//부모를 누구로 하는지 계속 붙혀준다.
	//SpringArm같은 경우는 Capsule이 부모이기 때문에 그 친구한테 붙인다.
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 1250.0f;
	SpringArm->SetRelativeRotation(FRotator(-55.0f, 0.0f, 0.0f));

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f)
	);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(
		TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'")
	);

	
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	/*매 프레임마다 확인 하는 것이 아닌, Montage가 끝났을 때 한 번만 체크하여 성능상 이점이 있다. */
	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Jump함수는 ACharacter에 내장된 함수
	/*void ACharacter::Jump()
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}*/
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
	
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter::Yaw);
}

void AMyCharacter::Attack()
{
	/*매 틱마다 호출되는 함수가 아니라서 Character쪽에서 관리하는 것이 나아보인다 라는 의견이 있음!*/
	if (IsAttacking)
	{
		return;
	}

	AnimInstance->PlayAttackMontage();

	AnimInstance->JumpToSection(AttackIndex);
	AttackIndex = (AttackIndex + 1) % 3;
	
	IsAttacking = true;
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
}

void AMyCharacter::UpDown(float Value)
{
	UpDownValue = Value;
	
	AddMovementInput(GetActorForwardVector(), Value);
}

void AMyCharacter::LeftRight(float Value)
{
	LeftRightValue = Value;

	AddMovementInput(GetActorRightVector(), Value);
}

void AMyCharacter::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

