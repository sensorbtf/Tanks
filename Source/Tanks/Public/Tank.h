// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"


UCLASS()
class TANKS_API ATank : public ABasePawn {
	GENERATED_BODY()

public:
	ATank();
	virtual void       SetupPlayerInputComponent(UInputComponent* p_playerInputComponent) override;
	void               HandleDestruction();
	APlayerController* GetTankPlayerController() const { return TankPlayerController; }

protected:
	// Sets default values for this pawn's properties
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SprintArm;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	float Speed = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Parameters")
	float TurningSpeed = 200.0f;

	void               Move(float p_value);
	void               Turn(float p_value);
	void               Tick(float DeltaTime);
	APlayerController* TankPlayerController;
};
