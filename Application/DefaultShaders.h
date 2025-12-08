#ifndef KTY_DEFAULT_SH_H
#define KTY_DEFAULT_SH_H

#include "Shader.h"

struct DefaultShaders {
public:
	static Shader Unlit;
	static Shader Lit;
	static Shader WorldspaceDepth;
	static Shader ScreenspaceDepth;
	static Shader Screenspace;
	static void Create() {
		Unlit.CreateShader("default_vert.glsl", "unlit_fragment.glsl");
		Lit.CreateShader("default_vert.glsl", "lit_fragment.glsl");
		WorldspaceDepth.CreateShader("default_vert.glsl", "worldspace_depth_frag.glsl");
		ScreenspaceDepth.CreateShader("screenspace_vert.glsl", "screenspace_depth_frag.glsl");
		Screenspace.CreateShader("screenspace_vert.glsl", "screenspace_frag.glsl");
	}
};

#endif