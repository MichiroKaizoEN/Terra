// Copyright 2023 MichiroKaizoEN All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FastNoise/VoxelFastNoise.h"
#include "VoxelGenerators/VoxelGenerator.h"
#include "VoxelGenerators/VoxelGeneratorHelpers.h"
#include "TerraWorldGenerator.generated.h"

/**
 * 
 */

class UCurveFloat;

UCLASS(Blueprintable)
class TERRA_API UTerraWorldGenerator : public UVoxelGenerator
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		float NoiseHeight = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		float NoiseScale = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		float Frequency = 0.001f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		int32 Seed = 1337;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		float Octaves = 7.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		UCurveFloat* OceanCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		UCurveFloat* IslandCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		UCurveFloat* LandCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		UCurveFloat* MountainCurve;
	//~ Begin UVoxelGenerator Interface
	virtual TVoxelSharedRef<FVoxelGeneratorInstance> GetInstance() override;
	//~ End UVoxelGenerator Interface
};

class FTerraGeneratorInstance : public TVoxelGeneratorInstanceHelper<FTerraGeneratorInstance, UTerraWorldGenerator>
{
public:
	using Super = TVoxelGeneratorInstanceHelper<FTerraGeneratorInstance, UTerraWorldGenerator>;

	explicit FTerraGeneratorInstance(const UTerraWorldGenerator& MyGenerator);

	//~ Begin FVoxelGeneratorInstance Interface
	virtual void Init(const FVoxelGeneratorInit& InitStruct) override;

	v_flt GetValueImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const;
	FVoxelMaterial GetMaterialImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const;

	TVoxelRange<v_flt> GetValueRangeImpl(const FVoxelIntBox& Bounds, int32 LOD, const FVoxelItemStack& Items) const;

	virtual FVector GetUpVector(v_flt X, v_flt Y, v_flt Z) const override final;
	//~ End FVoxelGeneratorInstance Interface

private:
	const float NoiseHeight;
	const float NoiseScale;
	const float Frequency;
	const int32 Seed;
	const int32 Octaves;
	UCurveFloat* OceanCurve;
	UCurveFloat* IslandCurve;
	UCurveFloat* LandCurve;
	UCurveFloat* MountainCurve;
	FVoxelFastNoise Noise;
};