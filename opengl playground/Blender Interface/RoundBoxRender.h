#ifndef ROUNDBOXRENDER_H
#define ROUNDBOXRENDER_H

// 7/8/2010 RYF
// QQ: 546952710
// Email: dizuo@126.com

struct RoundBoxRender
{
	// 产生一个渐变的效果：
	//		shadetop叠加top
	//		shadedown叠加到down 可以为负
	//		rad为圆角的半径

	/* linear horizontal shade within button or in outline */
	/* view2d scrollers use it */
	static void gl_round_box_shade(int mode, float minx, float miny, float maxx, float maxy, float rad, float shadetop, float shadedown)
	{
		float vec[7][2]= {{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
		{0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}};
		float div= maxy-miny;
		float coltop[3], coldown[3], color[4];
		int a;

		/* mult */
		for(a=0; a<7; a++) {
			vec[a][0]*= rad; vec[a][1]*= rad;
		}
		/* get current color, needs to be outside of glBegin/End */
		glGetFloatv(GL_CURRENT_COLOR, color);

		/* 'shade' defines strength of shading */	
		coltop[0]= color[0]+shadetop; if(coltop[0]>1.0) coltop[0]= 1.0;
		coltop[1]= color[1]+shadetop; if(coltop[1]>1.0) coltop[1]= 1.0;
		coltop[2]= color[2]+shadetop; if(coltop[2]>1.0) coltop[2]= 1.0;
		coldown[0]= color[0]+shadedown; if(coldown[0]<0.0) coldown[0]= 0.0;
		coldown[1]= color[1]+shadedown; if(coldown[1]<0.0) coldown[1]= 0.0;
		coldown[2]= color[2]+shadedown; if(coldown[2]<0.0) coldown[2]= 0.0;

		glShadeModel(GL_SMOOTH);
		glBegin(mode);

		/* start with corner right-bottom */
		if(roundboxtype & 4) {

			round_box_shade_col(coltop, coldown, 0.0);
			glVertex2f(maxx-rad, miny);

			for(a=0; a<7; a++) {
				round_box_shade_col(coltop, coldown, vec[a][1]/div);
				glVertex2f(maxx-rad+vec[a][0], miny+vec[a][1]);
			}

			round_box_shade_col(coltop, coldown, rad/div);
			glVertex2f(maxx, miny+rad);
		}
		else {
			round_box_shade_col(coltop, coldown, 0.0);
			glVertex2f(maxx, miny);
		}

		/* corner right-top */
		if(roundboxtype & 2) {

			round_box_shade_col(coltop, coldown, (div-rad)/div);
			glVertex2f(maxx, maxy-rad);

			for(a=0; a<7; a++) {
				round_box_shade_col(coltop, coldown, (div-rad+vec[a][1])/div);
				glVertex2f(maxx-vec[a][1], maxy-rad+vec[a][0]);
			}
			round_box_shade_col(coltop, coldown, 1.0);
			glVertex2f(maxx-rad, maxy);
		}
		else {
			round_box_shade_col(coltop, coldown, 1.0);
			glVertex2f(maxx, maxy);
		}

		/* corner left-top */
		if(roundboxtype & 1) {

			round_box_shade_col(coltop, coldown, 1.0);
			glVertex2f(minx+rad, maxy);

			for(a=0; a<7; a++) {
				round_box_shade_col(coltop, coldown, (div-vec[a][1])/div);
				glVertex2f(minx+rad-vec[a][0], maxy-vec[a][1]);
			}

			round_box_shade_col(coltop, coldown, (div-rad)/div);
			glVertex2f(minx, maxy-rad);
		}
		else {
			round_box_shade_col(coltop, coldown, 1.0);
			glVertex2f(minx, maxy);
		}

		/* corner left-bottom */
		if(roundboxtype & 8) {

			round_box_shade_col(coltop, coldown, rad/div);
			glVertex2f(minx, miny+rad);

			for(a=0; a<7; a++) {
				round_box_shade_col(coltop, coldown, (rad-vec[a][1])/div);
				glVertex2f(minx+vec[a][1], miny+rad-vec[a][0]);
			}

			round_box_shade_col(coltop, coldown, 0.0);
			glVertex2f(minx+rad, miny);
		}
		else {
			round_box_shade_col(coltop, coldown, 0.0);
			glVertex2f(minx, miny);
		}

		glEnd();
		glShadeModel(GL_FLAT);
	}

	static void setType(int nType){ 
		roundboxtype = nType % 16;
	}

protected:
	static void round_box_shade_col(float *col1, float *col2, float fac)
	{
		float col[3];

		col[0]= (fac*col1[0] + (1.0-fac)*col2[0]);
		col[1]= (fac*col1[1] + (1.0-fac)*col2[1]);
		col[2]= (fac*col1[2] + (1.0-fac)*col2[2]);

		glColor3fv(col);
	}

	static int roundboxtype;
};

int RoundBoxRender::roundboxtype = 3;

#endif