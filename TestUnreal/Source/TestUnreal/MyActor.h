// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class TESTUNREAL_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	//Reflection Complie 주석기능
	//VisibleAnyWhere : Detail 설정 가능
	UPROPERTY(VisibleAnyWhere)
	//new(x), Component기 때문에 unreal에서 자동으로 관리하는 Memory. 따라서 CreateDefalutSomeObject함수 사용
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnyWhere, Category=BattleStat)
	int32 Hp;

	UPROPERTY(EditAnyWhere, Category = BattleStat)
	int32 Mp;

	UPROPERTY(EditAnyWhere, Category = BattleStat)
	float RotateSpeed = 30.0f;
};
