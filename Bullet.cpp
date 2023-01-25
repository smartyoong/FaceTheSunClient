// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FaceTheSunCharacter.h"
#include "CommonEnemy.h"

ABullet::ABullet()
{
    GetCollisionComp()->OnComponentHit.AddDynamic(this,&ABullet::OnHit);
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if ((OtherActor != nullptr) && (OtherActor != this))
	{
		// Only add impulse and destroy projectile if we hit a physics
		auto EnemyActor = Cast<ACommonEnemy>(OtherActor);
		if (EnemyActor)
		{
			if ((OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
			{
				OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
			}
			UGameplayStatics::ApplyDamage(EnemyActor, Damage, GetWorld()->GetFirstPlayerController(), this, UDamageType::StaticClass());
		}
		Destroy();
	}
}