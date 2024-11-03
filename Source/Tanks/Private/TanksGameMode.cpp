// Fill out your copyright notice in the Description page of Project Settings.


#include "TanksGameMode.h"

#include "Tank.h"
#include "TanksPlayerController.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"

void ATanksGameMode::ActorDied(AActor* DeadActor)
{
	if(DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if(PlayerController)
		{
			PlayerController->SetPlayerEnabledState(false);
		}

		GameOver(false);
	}
	else if(ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
		TargetTowers--;

		if(TargetTowers == 0)
		{
			GameOver(true);
		}
	}
}

void ATanksGameMode::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
}

void ATanksGameMode::HandleGameStart()
{
	TargetTowers = GetTowerCount();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerController = Cast<ATanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if(PlayerController)
	{
		PlayerController->SetPlayerEnabledState(false);

		FTimerHandle   TimerHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(
			PlayerController,
			&ATanksPlayerController::SetPlayerEnabledState,
			true);

		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, StartDelay, false);
	}
}

int32 ATanksGameMode::GetTowerCount()
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), AllActors);
	return AllActors.Num();
}
