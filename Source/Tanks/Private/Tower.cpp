// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector TankLocation;
	if(IsInFireRange(TankLocation))
	{
		RotateTurret(TankLocation);
	}
}

void ATower::CheckFireCondition()
{
	FVector TankLocation;
	if(IsInFireRange(TankLocation))
	{
		Fire();
	}
}

bool ATower::IsInFireRange(FVector& TankLocation) const
{
	if(!Tank) return false;
	
	TankLocation = Tank->GetActorLocation();
	float   Distance = FVector::Dist(GetActorLocation(), TankLocation);

	return Distance <= FireRange;
}
