#include "XCBossInfoSlot.h"
#include "../util/ImageLoader.h"
#include "../util/ShaderReader.h"
#include "../XCShape/XCDefaultShape.h"
#include <GL3/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
using namespace xc_ogl;
using namespace xc_se;
void xc_se::XCBossInfoSlot::ShaderInit()
{
	ShaderReader BossSE;	
	BossSE.load_from_file("Shader/se/BossSE.vert",GL_VERTEX_SHADER);
	BossSE.load_from_file("Shader/se/BossSE.frag",GL_FRAGMENT_SHADER);
	program = BossSE.get_program();
}

void xc_se::XCBossInfoSlot::TextureInit()
{
}

void xc_se::XCBossInfoSlot::BufferInit()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(covered_plane_vertex), covered_plane_vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

}

void xc_se::XCBossInfoSlot::SetInfo(float * hp, float * mhp, int rtime, int sccount)
{
	BossHP = hp;
	BossMaxHP=mhp;
	RemainingTime = rtime;
	SpellCardCount = sccount;
}

void xc_se::XCBossInfoSlot::SpecialEffectInit()
{
	ShaderInit();
	TextureInit();
	BufferInit();
}

bool xc_se::XCBossInfoSlot::SpecialEffectRender()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	auto transfom_mat = glGetUniformLocation(program, "transform_mat");
	auto time = glGetUniformLocation(program, "time");
	glm::mat4 transform_mat;
	transform_mat = glm::translate(transform_mat, glm::vec3(0.0f,0.0f,0.0f));
	transform_mat = glm::scale(transform_mat, glm::vec3(0.05f,0.01f,0.0f));
	glUniformMatrix4fv(transfom_mat, 1, GL_FALSE, glm::value_ptr(transform_mat));
	glUniform1f(time, glfwGetTime());
//	glDrawArrays(GL_TRIANGLES, 0, sizeof(covered_plane_vertex)/2*sizeof(float));
	std::cout << "Rate:" << (*BossHP) / (*BossMaxHP) << std::endl;
	return false;
}

void xc_se::XCBossInfoSlot::SpecialEffectRelease()
{
}