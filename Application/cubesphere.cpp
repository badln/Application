#include "Mesh.h"

Mesh MeshGenerator::CubeSphere(int subdivisions) {
	Debug::Warning("The CubeSphere mesh isn't currently implemented. Mesh generator defaulted to UVSphere.");
	return MeshGenerator::UVSphere();  //Not implemented
}