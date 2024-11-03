// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API ATanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor);
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);
private:
	class ATank* Tank;
	class ATanksPlayerController* PlayerController;

	float StartDelay = 3.0f;
	int32 TargetTowers = 0;

	void HandleGameStart();
	int32 GetTowerCount();
};
