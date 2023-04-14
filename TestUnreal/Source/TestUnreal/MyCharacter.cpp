// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/Capsulecomponent.h"
#include "MyAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "MyWeapon.h"
#include "MyStatComponent.h"

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

	//add StatComponent
	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));
	
	/*FName WeaponSocket(TEXT("hand_l_socket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(
			TEXT("StaticMesh'/Game/ParagonGreystone/FX/Meshes/Heroes/Greystone/SM_Greystone_Blade_01.SM_Greystone_Blade_01'"));
		if (SW.Succeeded())
		{
			Weapon->SetStaticMesh(SW.Object);
		}

		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}*/

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FName WeaponSocket(TEXT("hand_l_socket"));

	auto CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	
	if (CurrentWeapon)
	{
		/*CurrentWeapon->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			WeaponSocket
		);*/
	}

}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	/*매 프레임마다 확인 하는 것이 아닌, Montage가 끝났을 때 한 번만 체크하여 성능상 이점이 있다. */
	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	//Null check
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
		AnimInstance->OnAttackHit.AddUObject(this, &AMyCharacter::AttackCheck);
	}


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

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->OnAttacked(DamageAmount);
	
	return DamageAmount;
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

void AMyCharacter::AttackCheck()
{
	FHitResult HitResult;

	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.0f;
	float AttackRadius = 50.0f;

	//피격 판정
	//Params는 문서 참고.
	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() *  AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2, //Attack Channel
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;

	if (bResult)
	{
		DrawColor = FColor::Green;
	}
	else
	{
		DrawColor = FColor::Red;
	}

	DrawDebugCapsule(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		Rotation,
		DrawColor,
		false,
		2.0f
	);

	if (bResult && HitResult.Actor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName());

		//대부분의 경우는 피격당하는 입장쪽에서 피격 함수를 처리하는게 깔끔하다.
		//도트 데미지, 공격자에 대한 어그로 판별 등 때문에 그러하다.
		/*HitResult.Actor->TakeDamage(
			
		);*/
		FDamageEvent DamageEvnet;
		HitResult.Actor->TakeDamage(Stat->GetAttack(), DamageEvnet, GetController(), this);
	}
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

