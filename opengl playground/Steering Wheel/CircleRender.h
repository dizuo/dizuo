#ifndef CIRCLERENDER_H
#define CIRCLERENDER_H

#ifndef M_PI
#define M_PI 3.1415926
#endif

class CircleRender
{
public:
	static void render_circle_contour(float cx, float cy, float r, int num_segments)
	{
		glBegin(GL_LINE_STRIP);
		for (int i=0; i<=num_segments; i++)
		{
			glVertex2f( cx+r*cos( (2*M_PI*i)/num_segments ), 
				cy+r*sin( (2*M_PI*i)/num_segments ) );
		}
		glEnd();
	}

	static void render_circle_area(float cx, float cy, float cz=0.0f, float r=1.0f, int num_segments=32)
	{
		float vertex[4]; 

		const float delta_angle = 2.0*M_PI/num_segments;
		glBegin(GL_TRIANGLE_FAN);

		vertex[0] = cx;
		vertex[1] = cy;
		vertex[2] = cz;
		vertex[3] = 1.0;        
		glVertex4fv(vertex);

		//draw the vertex on the contour of the circle
		for(int i = 0; i < num_segments ; i++)
		{
			vertex[0] = std::cos(delta_angle*i) * r + cx;
			vertex[1] = std::sin(delta_angle*i) * r + cy;
			vertex[2] = cz;
			vertex[3] = 1.0;
			glVertex4fv(vertex);
		}

		vertex[0] = 1.0 * r + cx;
		vertex[1] = 0.0 * r + cy;
		vertex[2] = cz;
		vertex[3] = 1.0;
		glVertex4fv(vertex);
		glEnd();
	}

	/*
	 * 内部半径	外部半径	 开始角度		结束角度
	 * 开始角度 < 结束角度
	 * [315, 405]	:	right
	 * [-45, 45]	:	right
	 * [315, 45]	:	wrong
	 */
	static void render_circular_sector_area(float inner_r, float outer_r, 
											float beg_angle=0.0f, float end_angle=90.0f, 
											int segment=32)
	{
		// beg_angle位于第四象限
		// end_angle位于第一象限
		while(end_angle < beg_angle)
			end_angle += 360;

		// angle --> rad
		const float beg_rad = beg_angle*M_PI/180.f;
		const float end_rad = end_angle*M_PI/180.f;

		const float delta_rad = (end_rad-beg_rad)/segment;

		float inner_vert[2] = {0.0f};
		float outer_vert[2] = {0.0f};
		float inner_next_vert[2] = {0.0f};

		inner_vert[0] = std::cos(beg_rad) * inner_r;
		inner_vert[1] = std::sin(beg_rad) * inner_r;	

		glBegin(GL_TRIANGLE_STRIP);
		glVertex2fv(inner_vert);

		for (int i=0; i<segment; i++)
		{
			outer_vert[0] = std::cos(delta_rad*i + beg_rad) * outer_r;
			outer_vert[1] = std::sin(delta_rad*i + beg_rad) * outer_r;	

			int next = i+1;
			inner_next_vert[0] = std::cos(delta_rad*next + beg_rad) * inner_r;
			inner_next_vert[1] = std::sin(delta_rad*next + beg_rad) * inner_r;
			
			glVertex2fv(outer_vert);
			glVertex2fv(inner_next_vert);
		}
		
		outer_vert[0] = std::cos(end_rad) * outer_r;
		outer_vert[1] = std::sin(end_rad) * outer_r;	
		glVertex2fv(outer_vert);

		glEnd();
	}

};

#endif