///
/// PlanetData.h
/// Seed of Andromeda
///
/// Created by Benjamin Arnold on 5 Feb 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Structs for planetary data
///

#pragma once

#ifndef PlanetData_h__
#define PlanetData_h__

#include <Vorb/VorbPreDecl.inl>
#include <Vorb/graphics/Texture.h>
#include <Vorb/io/Keg.h>

#include "Biome.h"

DECL_VG(class GLProgram);

struct LiquidColorKegProperties {
    nString colorPath = "";
    nString texturePath = "";
    ColorRGB8 tint = ColorRGB8(255, 255, 255);
    float depthScale = 1000.0f;
    float freezeTemp = -1.0f;
};
KEG_TYPE_DECL(LiquidColorKegProperties);


struct TerrainColorKegProperties {
    nString colorPath = "";
    nString texturePath = "";
    ColorRGB8 tint = ColorRGB8(255, 255, 255);
};
KEG_TYPE_DECL(TerrainColorKegProperties);

enum class TerrainFunction {
    NOISE,
    RIDGED_NOISE
};
KEG_ENUM_DECL(TerrainFunction);

struct TerrainFuncKegProperties {
    TerrainFunction func;
    int octaves = 1;
    float persistence = 1.0f;
    float frequency = 1.0f;
    float low = -1.0f;
    float high = 1.0f;
};
KEG_TYPE_DECL(TerrainFuncKegProperties);

struct TerrainFuncs {
    std::vector<TerrainFuncKegProperties> funcs;
    float baseHeight = 0.0f;
};

struct PlanetGenData {
    vg::Texture terrainColorMap = 0;
    vg::Texture liquidColorMap = 0;
    vg::Texture terrainTexture = 0;
    vg::Texture liquidTexture = 0;
    ColorRGB8 liquidTint = ColorRGB8(255, 255, 255);
    ColorRGB8 terrainTint = ColorRGB8(255, 255, 255);
    float liquidDepthScale = 1000.0f;
    float liquidFreezeTemp = -1.0f;
    float tempLatitudeFalloff = 0.0f;
    float humLatitudeFalloff = 0.0f;
    VGTexture biomeArrayTexture = 0;
    VGTexture baseBiomeLookupTexture = 0;
    std::vector<Biome> biomes;
    std::vector<BlockLayer> blockLayers;
    ui32 liquidBlock = 0;
    ui32 surfaceBlock = 0;
    vg::GLProgram* program = nullptr;
    f64 radius = 0.0;

    TerrainFuncs baseTerrainFuncs;
    TerrainFuncs tempTerrainFuncs;
    TerrainFuncs humTerrainFuncs;

    std::map <nString, ui32> blockColorMapLookupTable; ///< For looking up the index for the block color maps
    std::vector <ColorRGB8*> blockColorMaps; ///< Storage for the block color maps
};

#endif // PlanetData_h__