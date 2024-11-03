// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	TankPlayerController = Cast<APlayerController>(GetController());
}

// Sets default values
ATank::ATank()
{
	SprintArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SprintArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Camera->SetupAttachment(SprintArm);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* p_playerInputComponent)
{
	Super::SetupPlayerInputComponent(p_playerInputComponent);

	p_playerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	p_playerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	p_playerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(TankPlayerController)
	{
		FHitResult Hit;
		TankPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		DrawDebugSphere(GetWorld(), Hit.Location, 20, 30, FColor::Red);

		RotateTurret(Hit.Location);
	}
}

void ATank::Move(float p_value)
{
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = p_value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float p_value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = p_value * TurningSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}