// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStatComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GamePlayStatics.h"

// Sets default values for this component's properties
UMyStatComponent::UMyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UMyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevel(Level);
}


// Called every frame
void UMyStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMyStatComponent::SetLevel(int32 NewLevel)
{
	//SingleTon °³³ä
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetStatData(NewLevel);
		if (StatData)
		{
			Level = StatData->Level;
			Hp = StatData->MaxHP;
			Attack = StatData->Attack;
		}
	}
}

void UMyStatComponent::OnAttacked(float DamageAmount)
{
	Hp -= DamageAmount;

	if (Hp < 0)
	{
		Hp = 0;
	}

	UE_LOG(LogTemp, Warning, TEXT("OnAttacked %d"), Hp);
}

