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

	//RootComponent�� �ְ�, �������� �����Ѵ�.
	//�θ� ������ �ϴ��� ��� �����ش�.
	//SpringArm���� ���� Capsule�� �θ��̱� ������ �� ģ������ ���δ�.
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

	/*�� �����Ӹ��� Ȯ�� �ϴ� ���� �ƴ�, Montage�� ������ �� �� ���� üũ�Ͽ� ���ɻ� ������ �ִ�. */
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

	//Jump�Լ��� ACharacter�� ����� �Լ�
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
	/*�� ƽ���� ȣ��Ǵ� �Լ��� �ƴ϶� Character�ʿ��� �����ϴ� ���� ���ƺ��δ� ��� �ǰ��� ����!*/
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

	//�ǰ� ����
	//Params�� ���� ����.
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

		//��κ��� ���� �ǰݴ��ϴ� �����ʿ��� �ǰ� �Լ��� ó���ϴ°� ����ϴ�.
		//��Ʈ ������, �����ڿ� ���� ��׷� �Ǻ� �� ������ �׷��ϴ�.
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

