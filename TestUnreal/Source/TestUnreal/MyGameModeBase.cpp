// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyPawn.h"

AMyGameModeBase::AMyGameModeBase()
{
	//UCLASS()�� ��ϵ� Class, UCLASS�� ��ϵ� Class�� StaticClass�Լ� ����.//Static��ü�� �޾� �´�.
	DefaultPawnClass = AMyPawn::StaticClass();
}
