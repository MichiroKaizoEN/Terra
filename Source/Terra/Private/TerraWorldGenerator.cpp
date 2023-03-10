// Copyright 2023 MichiroKaizoEN All Rights Reserved.


#include "TerraWorldGenerator.h"
#include "FastNoise/VoxelFastNoise.inl"
#include "VoxelMaterialBuilder.h"
#include "VoxelGraph/Public/NodeFunctions/VoxelMathNodeFunctions.h"

TVoxelSharedRef<FVoxelGeneratorInstance> UTerraWorldGenerator::GetInstance()
{
	return MakeVoxelShared<FTerraGeneratorInstance>(*this);
}

///////////////////////////////////////////////////////////////////////////////

FTerraGeneratorInstance::FTerraGeneratorInstance(const UTerraWorldGenerator& MyGenerator)
	: Super(&MyGenerator)
	, NoiseHeight(MyGenerator.NoiseHeight)
	, NoiseScale(MyGenerator.NoiseScale)
	, Frequency(MyGenerator.Frequency)
	, Seed(MyGenerator.Seed)
	, Octaves(MyGenerator.Octaves)
	, OceanCurve(MyGenerator.OceanCurve)
	, IslandCurve(MyGenerator.IslandCurve)
	, LandCurve(MyGenerator.LandCurve)
	, MountainCurve(MyGenerator.MountainCurve)
{
}

void FTerraGeneratorInstance::Init(const FVoxelGeneratorInit& InitStruct)
{
	Noise.SetSeed(Seed);
	//OceanCurve->bIsEventCurve = false;
	//IslandCurve->bIsEventCurve = false;
	//LandCurve->bIsEventCurve = false;
	//MountainCurve->bIsEventCurve = false;
}

v_flt FTerraGeneratorInstance::GetValueImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const
{
	// Z - (Noise * Noise scale + Base Height)
	v_flt OceanMask;
	v_flt IslandMask;
	v_flt LandMask;
	v_flt MountainMask;
	// const float Height = Noise.GetPerlin_2D(X, Y, Frequency) * NoiseHeight;
	const float Height = Noise.GetPerlinFractal_2D(X, Y, Frequency, Octaves);
	const float Biome = Noise.GetCubicFractal_2D(X, Y, .0003f, Octaves);
	
	
	TVoxelStaticArray<v_flt, 6> ArrayIn;
	TVoxelStaticArray<v_flt, 4> BiomeMask;
	ArrayIn[0] = v_flt(-10.f);
	ArrayIn[1] = v_flt(3.f);
	ArrayIn[2] = v_flt(15.f);
	ArrayIn[3] = v_flt(5.f);
	ArrayIn[4] = v_flt(35.f);
	ArrayIn[5] = v_flt(10.f);
	FVoxelMathNodeFunctions::HeightSplit(Biome * 100,ArrayIn,BiomeMask);
	// The voxel value is clamped between -1 and 1. That can result in a bad gradient/normal. To solve that we divide it
	OceanMask = BiomeMask[0];
	IslandMask = BiomeMask[1];
	LandMask = BiomeMask[2];
	MountainMask = BiomeMask[3];
	
	
	float Ocean = FMath::Lerp(0.f, OceanCurve->GetFloatValue(Height), OceanMask);
	float Islands = FMath::Lerp(Ocean, IslandCurve->GetFloatValue(Height), IslandMask);
	float Land = FMath::Lerp(Islands, LandCurve->GetFloatValue(Height), LandMask);
	float Mountains = FMath::Lerp(Land,  MountainCurve->GetFloatValue(Height), MountainMask);

	float Value = Z - (Mountains * NoiseScale + NoiseHeight);
	
	//Value /= 5;
	
	return Value;
}

FVoxelMaterial FTerraGeneratorInstance::GetMaterialImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const
{
	FVoxelMaterialBuilder Builder;
	
	// RGB
	//Builder.SetMaterialConfig(EVoxelMaterialConfig::RGB);
	//Builder.SetColor(FColor::Red);

	//// Single index
	//Builder.SetMaterialConfig(EVoxelMaterialConfig::SingleIndex);
	//Builder.SetSingleIndex(0); 

	v_flt OceanMask;
	v_flt IslandMask;
	v_flt LandMask;
	v_flt MountainMask;

	const float Biome = Noise.GetCubicFractal_2D(X, Y, .0003f, Octaves);


	TVoxelStaticArray<v_flt, 6> ArrayIn;
	TVoxelStaticArray<v_flt, 4> BiomeMask;
	ArrayIn[0] = v_flt(-10.f);
	ArrayIn[1] = v_flt(0.f);
	ArrayIn[2] = v_flt(10.f);
	ArrayIn[3] = v_flt(0.f);
	ArrayIn[4] = v_flt(40.f);
	ArrayIn[5] = v_flt(0.f);
	FVoxelMathNodeFunctions::HeightSplit(Biome * 100, ArrayIn, BiomeMask);
	// The voxel value is clamped between -1 and 1. That can result in a bad gradient/normal. To solve that we divide it
	OceanMask = BiomeMask[0];
	IslandMask = BiomeMask[1];
	LandMask = BiomeMask[2];
	MountainMask = BiomeMask[3];

	//// Multi index
	Builder.SetMaterialConfig(EVoxelMaterialConfig::MultiIndex);
	Builder.AddMultiIndex(0, IslandMask);
	Builder.AddMultiIndex(1, LandMask);
	Builder.AddMultiIndex(2, LandMask);
	Builder.AddMultiIndex(3, LandMask);
	Builder.AddMultiIndex(4, MountainMask);
	Builder.AddMultiIndex(5, OceanMask);
	Builder.AddMultiIndex(6, LandMask);
	Builder.AddMultiIndex(7, LandMask);
	Builder.AddMultiIndex(8, LandMask);
	
	return Builder.Build();
}

TVoxelRange<v_flt> FTerraGeneratorInstance::GetValueRangeImpl(const FVoxelIntBox& Bounds, int32 LOD, const FVoxelItemStack& Items) const
{
	// Return the values that GetValueImpl can return in Bounds
	// Used to skip chunks where the value does not change
	// Be careful, if wrong your world will have holes!
	// By default return infinite range to be safe
	return TVoxelRange<v_flt>::Infinite();

	// Example for the GetValueImpl above

	// Noise is between -1 and 1
	const TVoxelRange<v_flt> Height = TVoxelRange<v_flt>(-1, 1) * NoiseHeight;

	// Z can go from min to max
	TVoxelRange<v_flt> Value = TVoxelRange<v_flt>(Bounds.Min.Z, Bounds.Max.Z) - Height;

	Value /= 5;

	return Value;
}

FVector FTerraGeneratorInstance::GetUpVector(v_flt X, v_flt Y, v_flt Z) const
{
	// Used by spawners
	return FVector::UpVector;
}

