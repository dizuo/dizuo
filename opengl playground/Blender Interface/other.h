#define UI_RB_ALPHA 16

/* plain antialiased unfilled rectangle */
void uiRoundRect(float minx, float miny, float maxx, float maxy, float rad)
{
	float color[4];

	if(roundboxtype & UI_RB_ALPHA) {
		glGetFloatv(GL_CURRENT_COLOR, color);
		color[3]= 0.5;
		glColor4fv(color);
		glEnable( GL_BLEND );
	}

	/* set antialias line */
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_BLEND );

	gl_round_box(GL_LINE_LOOP, minx, miny, maxx, maxy, rad);

	glDisable( GL_BLEND );
	glDisable( GL_LINE_SMOOTH );
}

void gl_round_box(int mode, float minx, float miny, float maxx, float maxy, float rad)
{
	float vec[7][2]= {{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
	{0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}};
	int a;

	/* mult */
	for(a=0; a<7; a++) {
		vec[a][0]*= rad; vec[a][1]*= rad;
	}

	glBegin(mode);

	/* start with corner right-bottom */
	if(roundboxtype & 4) {
		glVertex2f(maxx-rad, miny);
		for(a=0; a<7; a++) {
			glVertex2f(maxx-rad+vec[a][0], miny+vec[a][1]);
		}
		glVertex2f(maxx, miny+rad);
	}
	else glVertex2f(maxx, miny);

	/* corner right-top */
	if(roundboxtype & 2) {
		glVertex2f(maxx, maxy-rad);
		for(a=0; a<7; a++) {
			glVertex2f(maxx-vec[a][1], maxy-rad+vec[a][0]);
		}
		glVertex2f(maxx-rad, maxy);
	}
	else glVertex2f(maxx, maxy);

	/* corner left-top */
	if(roundboxtype & 1) {
		glVertex2f(minx+rad, maxy);
		for(a=0; a<7; a++) {
			glVertex2f(minx+rad-vec[a][0], maxy-vec[a][1]);
		}
		glVertex2f(minx, maxy-rad);
	}
	else glVertex2f(minx, maxy);

	/* corner left-bottom */
	if(roundboxtype & 8) {
		glVertex2f(minx, miny+rad);
		for(a=0; a<7; a++) {
			glVertex2f(minx+vec[a][1], miny+rad-vec[a][0]);
		}
		glVertex2f(minx+rad, miny);
	}
	else glVertex2f(minx, miny);

	glEnd();
}

void ui_shadowbox(float minx, float miny, float maxx, float maxy, float shadsize, unsigned char alpha)
{
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);

	/* right quad */
	glBegin(GL_POLYGON);
	glColor4ub(0, 0, 0, alpha);
	glVertex2f(maxx, miny);
	glVertex2f(maxx, maxy-0.3*shadsize);
	glColor4ub(0, 0, 0, 0);
	glVertex2f(maxx+shadsize, maxy-0.75*shadsize);
	glVertex2f(maxx+shadsize, miny);
	glEnd();

	/* corner shape */
	glBegin(GL_POLYGON);
	glColor4ub(0, 0, 0, alpha);
	glVertex2f(maxx, miny);
	glColor4ub(0, 0, 0, 0);
	glVertex2f(maxx+shadsize, miny);
	glVertex2f(maxx+0.7*shadsize, miny-0.7*shadsize);
	glVertex2f(maxx, miny-shadsize);
	glEnd();

	/* bottom quad */		
	glBegin(GL_POLYGON);
	glColor4ub(0, 0, 0, alpha);
	glVertex2f(minx+0.3*shadsize, miny);
	glVertex2f(maxx, miny);
	glColor4ub(0, 0, 0, 0);
	glVertex2f(maxx, miny-shadsize);
	glVertex2f(minx+0.5*shadsize, miny-shadsize);
	glEnd();

	glDisable(GL_BLEND);
	glShadeModel(GL_FLAT);
}
