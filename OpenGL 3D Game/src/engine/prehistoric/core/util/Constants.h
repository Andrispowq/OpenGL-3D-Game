#pragma once

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720
#define DEFAULT_TITLE "OpenGL Game Engine by András Kmeczó, 2019"
#define DEFAULT_FULLSCREEN false

#define FS_WINDOW_WIDTH 1920
#define FS_WINDOW_HEIGHT 1080

#define NM_WINDOW_WIDTH 1280
#define NM_WINDOW_HEIGHT 720

#define WINDOW_FULLSCREEN false

#define WINDOW_WIDTH WINDOW_FULLSCREEN ? FS_WINDOW_WIDTH : NM_WINDOW_WIDTH
#define WINDOW_HEIGHT WINDOW_FULLSCREEN ? FS_WINDOW_HEIGHT : NM_WINDOW_HEIGHT
#define WINDOW_TITLE "Game"

//Material texture names
#define ALBEDO_MAP "albedoMap"
#define NORMAL_MAP "normalMap"
#define DISPLACEMENT_MAP "displacementMAp"
#define METALLIC_MAP "metallicMap"
#define ROUGHNESS_MAP "roughnessMap"
#define OCCLUSION_MAP "occlusionMap"
#define EMISSION_MAP "emissionMap"
#define ALPHA_MAP "alphaMap"

//Specific textures' postfixes to be able to identify which texture contains which data
#define ALBEDO_MAP_POSTFIX "_DIF"
#define NORMAL_MAP_POSTFIX "_NRM"
#define DISPLACEMENT_MAP_POSTFIX "_DISP"
#define METALLIC_MAP_POSTFIX "_MET"
#define ROUGHNESS_MAP_POSTFIX "_RGH"
#define OCCLUSION_MAP_POSTFIX "_OCC"
#define EMISSION_MAP_POSTFIX "_EMI"
#define ALPHA_MAP_POSTFIX "_A"

//Material constant uniform names
#define COLOUR "colour"
#define USES_NORMAL_MAP "usesNormalMap"
#define HEIGHT_SCALE "heightScale"
#define METALLIC "metallic"
#define ROUGHNESS "roughness"
#define OCCLUSION "occlusion"
#define EMISSION "emission"
#define ALPHA "alpha"