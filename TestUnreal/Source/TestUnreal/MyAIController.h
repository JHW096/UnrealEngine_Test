// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREAL_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AMyAIController();

	//빙의할 때 호출되는 함수
	virtual void OnPossess(APawn* InPawn) override;
	
	//빙의가 해제될 때 함수
	virtual void OnUnPossess() override;

private:

	void RandomMove();

private:

	FTimerHandle TimerHandle;
};
