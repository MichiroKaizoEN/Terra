// // Copyright MichiroKaizoEN All Rights Reserved.


#include "TerraItemBase.h"

// Sets default values
ATerraItemBase::ATerraItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATerraItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATerraItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

