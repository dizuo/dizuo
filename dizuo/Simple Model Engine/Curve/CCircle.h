#ifndef CIRCLE_H_
#define CIRCLE_H_

// 3D circle
class CCircle 
{
public:
	CCircle(Real cx, Real cy, Real cz, Real r, int seg=20, AXIS_PLANE type_=XOZ)
	{
		center.setValue(cx, cy, cz);
		radius = r;
		type = type_;
		numSegment = seg;
	}

	CCircle(const RVec3& center_, Real r, int seg=20, AXIS_PLANE type_=XOZ )
	{
		center = center_;
		radius = r;
		type = type_;
		numSegment = seg;
	}

	bool Setup()
	{
		if (type==XOY)
		{
			for (int i=0; i<numSegment; i++)
			{
				RVec3 cV(center);
				Real angleRad = (2*M_PI*i)/numSegment;
				Real cosValue = cosf( angleRad );
				Real sinValue = sinf( angleRad );

				cV.x() = center.x() + radius * cosValue;
				cV.y() = center.y() + radius * sinValue;

				profile.push_back( cV );
			}
		}
		else if(type==XOZ)
		{
			for (int i=0; i<numSegment; i++)
			{
				RVec3 cV(center);
				Real angleRad = (2*M_PI*i)/numSegment;
				Real cosValue = cosf( angleRad );
				Real sinValue = sinf( angleRad );
				
				cV.x() = center.x() + radius * cosValue;
				cV.z() = center.z() + radius * sinValue;

				profile.push_back( cV );
			}
		}
		else
		{
			for (int i=0; i<numSegment; i++)
			{
				RVec3 cV(center);
				Real angleRad = (2*M_PI*i)/numSegment;
				Real cosValue = cosf( angleRad );
				Real sinValue = sinf( angleRad );

				cV.z() = center.z() + radius * cosValue;
				cV.y() = center.y() + radius * sinValue;
		
				profile.push_back( cV );
			}
		}

		return true;
	}

	//virtual void Render()
	//{
	//	glBegin(GL_LINE_STRIP);
	//	for (int i=0; i<profile.size(); i++)
	//	{
	//		glVertex3fv( profile[i].getValue() );
	//	}
	//	glEnd();
	//}

	//CArc GetArc() {};
	const RVec3Array& GetProfile() const { return profile; }
private:
	RVec3		center;
	Real		radius;
	RVec3Array	profile;

	AXIS_PLANE	type;
	int			numSegment;
};
// 跟CCurve有重复变量
#endif