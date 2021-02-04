#pragma once
#ifndef RENDER_H
#define RENDER_H

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Curve.h"

#include <vector>

struct RenderObject {
	VertexArray vertex_array;
	VertexBuffer vertex_buffer;
	VertexBufferLayout layout;
	Shader shader;
};

class Render : public Curve{
public:
	Render();
	void Dynamic_Draw();
	void Add_Control_Point();
	void Remove_Control_Point();
	
	bool show_decasteljau;
	bool show_bspline;
	bool show_nurbs;
	bool update_control_points;
private:

	RenderObject m_rdr_obj_ctrl_pts;
	RenderObject m_rdr_obj_decasteljau;
	RenderObject m_rdr_obj_bspline;
	RenderObject m_rdr_obj_nurbs;

	void Draw(RenderObject&, const std::vector<float>&);
	void Update_Vertices(std::vector<float>&);
	void Normal_Distribution(float *);
};
#endif