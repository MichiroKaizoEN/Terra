// Copyright MichiroKaizoEN All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TerraCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class TERRA_API ATerraCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATerraCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		UInputMappingContext* TerraInputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		UInputAction* PrimaryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		UInputAction* SecondaryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		UInputAction* JumpAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Primary(const FInputActionValue& Value);
	void Secondary(const FInputActionValue& Value);


private:
	UPROPERTY(VisibleAnywhere)
		float Health;

	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 100.0;
};
