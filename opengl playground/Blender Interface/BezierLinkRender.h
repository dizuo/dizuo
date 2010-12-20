#ifndef BEZIERLINK_H
#define BEZIERLINK_H

typedef struct rcti 
{
	int xmin, xmax;
	int ymin, ymax;
} rcti;

struct BezierLinkRender
{
	static const int LINK_RESOL = 24;

	static void ui_draw_link_bezier(rcti *rect)
	{
		float coord_array[LINK_RESOL+1][2];

		if(ui_link_bezier_points(rect, coord_array, LINK_RESOL)) {
			float dist;
			int i;

			/* we can reuse the dist variable here to increment the GL curve eval amount*/
			dist = 1.0f/(float)LINK_RESOL;

			glEnable(GL_BLEND);
			glEnable(GL_LINE_SMOOTH);

			glBegin(GL_LINE_STRIP);
			for(i=0; i<=LINK_RESOL; i++) {
				glVertex2fv(coord_array[i]);
			}
			glEnd();

			glDisable(GL_BLEND);
			glDisable(GL_LINE_SMOOTH);

		}
	}

protected:

	static int ui_link_bezier_points(rcti *rect, float coord_array[][2], int resol)
	{
		float dist, vec[4][2];

		vec[0][0]= rect->xmin;
		vec[0][1]= rect->ymin;
		vec[3][0]= rect->xmax;
		vec[3][1]= rect->ymax;

		dist= 0.5f*abs(vec[0][0] - vec[3][0]);

		vec[1][0]= vec[0][0]+dist;
		vec[1][1]= vec[0][1];

		vec[2][0]= vec[3][0]-dist;
		vec[2][1]= vec[3][1];

		forward_diff_bezier(vec[0][0], vec[1][0], vec[2][0], vec[3][0], coord_array[0], resol, sizeof(float)*2);
		forward_diff_bezier(vec[0][1], vec[1][1], vec[2][1], vec[3][1], coord_array[0]+1, resol, sizeof(float)*2);

		return 1;
	}

	/* forward differencing method for bezier curve */
	static void forward_diff_bezier(float q0, float q1, float q2, float q3, float *p, int it, int stride)
	{
		float rt0,rt1,rt2,rt3,f;
		int a;

		f= (float)it;
		rt0= q0;
		rt1= 3.0f*(q1-q0)/f;
		f*= f;
		rt2= 3.0f*(q0-2.0f*q1+q2)/f;
		f*= it;
		rt3= (q3-q0+3.0f*(q1-q2))/f;

		q0= rt0;
		q1= rt1+rt2+rt3;
		q2= 2*rt2+6*rt3;
		q3= 6*rt3;

		for(a=0; a<=it; a++) {
			*p= q0;
			p = (float *)(((char *)p)+stride);
			q0+= q1;
			q1+= q2;
			q2+= q3;
		}
	}
};

#endif