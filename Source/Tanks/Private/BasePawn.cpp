// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasePawn::RotateTurret(FVector p_targetToLookAt)
{
	FVector lookAt = p_targetToLookAt - TurretMesh->GetComponentLocation();
	FRotator lookAtRotation = FRotator(0.0f,lookAt.Rotation().Yaw,0.0f);

	TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(),
		lookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 25.0f));
}

void ABasePawn::Fire()
{
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	Projectile -> SetOwner(this);
}

void ABasePawn::HandleDestruction() const
{
	UGameplayStatics::PlaySoundAtLocation(this, DestroySound, RootComponent->GetComponentLocation());
	UGameplayStatics::SpawnEmitterAtLocation(this, DestroyParticles, GetActorLocation(), GetActorRotation());
	if(DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
}