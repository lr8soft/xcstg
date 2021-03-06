#pragma once
#ifndef _default_shape_
#define _default_shape_
#define FLAT_COVERED_PLANE 0xffa
#define DEFAULT_CUBE_WITH_COLOR 0xffb
/***!!!It should be render as "GL_TRIANGLES" mode.!!!***/
static float covered_plane_vertex_with_texture[] = {
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f, 0.0f,
	-1.0,-1.0f, 0.0f, 0.0f,

	-1.0,-1.0f, 0.0f, 0.0f,
	-1.0, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};
/*GL_TRIANGLES sizeof(float)*24*/
float* GetCoveredPlaneWithTexVertex(float width_rate, float height_rate);
/***!!!It should be render as "GL_TRIANGLES" mode.!!!***/
static float covered_plane_vertex[] = {
	1.0f, 1.0f,
	1.0f,-1.0f,
	-1.0,-1.0f,
	-1.0,-1.0f,
	-1.0, 1.0f,
	1.0f, 1.0f,
};
/*GL_TRIANGLES sizeof(float)*12 */
float* GetCoveredPlaneVertex(float width_rate,float height_rate);
/***!!!It should be render as "GL_TRIANGLES" mode.!!!***/
static float player_render_texture_vertex[] = {
	0.8f, 1.0f,
	0.8f,-1.0f,
	-0.8f,-1.0f,
	-0.8f,-1.0f,
	-0.8f, 1.0f,
	0.8f, 1.0f
};
/***!!!It should be render as "GL_TRIANGLES" mode.!!!***/
static float player_decision_render_vertex[] = {
	0.025f, 0.025f,
	0.025f,-0.025f,
   -0.025f,-0.025f,
   -0.025f,-0.025f,
   -0.025f, 0.025f,
	0.025f, 0.025f
};
/***!!!It should be render as "GL_TRIANGLES" mode.!!!***/
static float default_cube_color[] =
{
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
/*共column列，row行；取第x列 第y行（以该子纹理右上角坐标为准）*/
float* GetSpecificTexture(int column, int row, int x, int y);
/*共column列，row行；取第x列 第y行（以该子纹理右上角坐标为准）*/
float* GetSpecificTexWithWidthAndHeight(float width_rate, float height_rate,int column, int row, int x, int y);
float* GetPointSpriteVertex(float size);
#endif