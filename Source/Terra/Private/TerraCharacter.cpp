// Copyright MichiroKaizoEN All Rights Reserved.


#include "TerraCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
ATerraCharacter::ATerraCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATerraCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(TerraInputMapping, 0);
		}
	}

}

// Called every frame
void ATerraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATerraCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void ATerraCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D DirectionValue = Value.Get<FVector2D>();

	if (GetController())
	{
		FVector Forward = GetActorForwardVector();
		FVector Right = GetActorRightVector();
		AddMovementInput(Forward, DirectionValue.Y);
		AddMovementInput(Right, DirectionValue.X);
	}
}

void ATerraCharacter::Primary(const FInputActionValue& Value)
{

}

void ATerraCharacter::Secondary(const FInputActionValue& Value)
{

}

// Called to bind functionality to input
void ATerraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATerraCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATerraCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Triggered, this, &ATerraCharacter::Primary);
		EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Triggered, this, &ATerraCharacter::Secondary);
	}
}

