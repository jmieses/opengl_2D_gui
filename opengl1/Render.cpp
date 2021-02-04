#include "Render.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>

static std::vector<float> lcl_control_points {
        -0.9f, -0.5f, 0.0f,  
        -0.0f, -0.5f, 0.0f,  
        -0.45f, 0.5f, 0.0f,  
         0.45f, 0.5f, 0.0f,
         0.25f, 0.25f, 0.0f
    };

Render::Render(){

		show_decasteljau = false;
		show_bspline = false;
		show_nurbs = true;
		update_control_points = true;
		
		this->m_rdr_obj_ctrl_pts.shader.Set_Shader("control_points.vs", "control_points.fs");
		this->m_rdr_obj_ctrl_pts.layout.Push<float>(3);
		this->m_rdr_obj_ctrl_pts.vertex_array.AddBuffer(this->m_rdr_obj_ctrl_pts.vertex_buffer, 
														this->m_rdr_obj_ctrl_pts.layout);
		glEnableVertexAttribArray(0);
		this->m_rdr_obj_decasteljau.shader.Set_Shader("decasteljau.vs", "decasteljau.fs");
		this->m_rdr_obj_decasteljau.layout.Push<float>(3);
		this->m_rdr_obj_decasteljau.vertex_array.AddBuffer(this->m_rdr_obj_decasteljau.vertex_buffer, 
														this->m_rdr_obj_decasteljau.layout);
		glEnableVertexAttribArray(0);
		this->m_rdr_obj_bspline.shader.Set_Shader("bspline.vs", "bspline.fs");
		this->m_rdr_obj_bspline.layout.Push<float>(3);
		this->m_rdr_obj_bspline.vertex_array.AddBuffer(this->m_rdr_obj_bspline.vertex_buffer, 
														this->m_rdr_obj_bspline.layout);
		glEnableVertexAttribArray(0);
		this->m_rdr_obj_nurbs.shader.Set_Shader("nurbs.vs", "nurbs.fs");
		this->m_rdr_obj_nurbs.layout.Push<float>(3);
		this->m_rdr_obj_nurbs.vertex_array.AddBuffer(this->m_rdr_obj_nurbs.vertex_buffer, 
														this->m_rdr_obj_nurbs.layout);
		glEnableVertexAttribArray(0);
}

void Render::Dynamic_Draw(){

	glPointSize(5);
	Draw(this->m_rdr_obj_ctrl_pts, lcl_control_points);
	glPointSize(2);

	if (show_decasteljau)
		Draw(this->m_rdr_obj_decasteljau, this->deCasteljau(lcl_control_points));

	if (show_bspline)
		Draw(this->m_rdr_obj_bspline, this->Bspline(lcl_control_points));

	if (show_nurbs)
		Draw(this->m_rdr_obj_nurbs, this->NURBS(lcl_control_points));

	if (update_control_points)
		Update_Vertices(lcl_control_points);
}

void Render::Draw(RenderObject& rdr_obj, const std::vector<float>& data){
	rdr_obj.shader.Use();
	rdr_obj.vertex_buffer.BindDynamic(data);
	rdr_obj.vertex_array.Bind();

	glDrawArrays(GL_POINTS, 0, data.size() / 3);
}

void Render::Update_Vertices(std::vector<float>& data) {

    double current_time = glfwGetTime();
    static double old_time = current_time;
    static constexpr float TIME_LAPSE = 5.0;
  
    if (current_time != 0.0 && (current_time - old_time) > TIME_LAPSE) {

        for (int i = 0; i < data.size(); i++) {
            float sample;
            Normal_Distribution(&sample);
            data[i] = sample;
        }
        old_time = current_time;
    }
}

void Render::Normal_Distribution(float * sample) {

    static std::random_device rd;
    static std::mt19937 gen(rd());  // Mersenne twister PRNG, initialized with seed from previous random device instance
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);

    static const float mean = distribution(generator);
    static const float std_dev = distribution(generator);
                                            
    static std::normal_distribution<float> normal_distribution(mean, std_dev);  // instance of class std::normal_distribution with specific mean and stddev

    float x = normal_distribution(gen);
    *sample = x / (1 + std::abs(x)); // *sample in range [-1, 1] using sigmoid function
}