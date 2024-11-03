// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	RootComponent = ProjectileMesh;

	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>("UParticleSystemComponent");
	TrailParticle->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovement->InitialSpeed = 2000;
	ProjectileMovement->MaxSpeed = 5000;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	if (LaunchSound)
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, RootComponent->GetComponentLocation());
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto MyOwner = GetOwner();

	if(MyOwner == nullptr)
	{
		DestroyHandler();
		return;
	}

	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		if(HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
	}

	DestroyHandler();
}

void AProjectile::DestroyHandler()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation(), GetActorRotation());
	if (LaunchSound)
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, RootComponent->GetComponentLocation());
	Destroy();
	
}