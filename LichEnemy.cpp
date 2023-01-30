// Fill out your copyright notice in the Description page of Project Settings.


#include "LichEnemy.h"
#include "LichSkillActor.h"
#include "FaceTheSunCharacter.h"
#include "Kismet/GameplayStatics.h"

void ALichEnemy::MultiLichAttack_Implementation()
{
    if(SkillAttackAnim)
        GetMesh()->GetAnimInstance()->Montage_Play(SkillAttackAnim);
}

void ALichEnemy::MultiLichAttackReal_Implementation()
{
    GetWorld()->SpawnActor<ALichSkillActor>(LichSkillClass, SkillLocation, SkillRotator);
}