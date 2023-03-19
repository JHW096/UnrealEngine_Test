// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyCharacter.h"

AMyGameModeBase::AMyGameModeBase()
{
	//UCLASS()에 등록된 Class, UCLASS에 등록된 Class는 StaticClass함수 포함.//Static객체를 받아 온다.
	DefaultPawnClass = AMyCharacter::StaticClass();
}
