#ifndef TWISTAPPLYER_H_
#define TWISTAPPLYER_H_

#include "Model.h"

class TwistApplyer
{
public:
	TwistApplyer() 
		: pTwistModel(0)
		, height(0.0f)
		, angleDegMax(90.0f)
	{}

	~TwistApplyer() 
	{
		if (pTwistModel)
		{
			delete pTwistModel;
			pTwistModel = 0;
		}
	}
	
	//according model's vertice normal to twist model
	void operator()(CModel& dstModel)
	{
		if (!pTwistModel)
		{
			pTwistModel = new CModel(dstModel);
		}

		Real angleDeg = angleDegMax * sinf(time);
		Real angleRad = angleDeg * 3.14159f / 180.0f;
		
		for (int i=0; i<dstModel.GetVertNum(); i++)
		{
			CVertex vert = dstModel.GetVertex(i);

			Real factor = (height*0.1 + vert.v[1])/height * angleRad ;
			
			_DoTwist( factor, pTwistModel->GetVertex(i), dstModel.GetVertex(i) );
		}
	}

	void SetTime(Real t) { time = t; }

	// twist object's height
	void SetHeight(Real h) { height = h; }

	// twist Max angle : 90.0f;
	void SetMaxAngleInDeg(Real angle) { angleDegMax = angle; }
private:	
	void _DoTwist(Real t, const CVertex& srcV, CVertex& dstV)
	{
			Real sint = sinf(t);
			Real cost = cosf(t);

			dstV.v[0] = srcV.v[0] * cost - srcV.v[2] * sint;
			dstV.v[2] = srcV.v[0] * sint + srcV.v[2] * cost;
			
			dstV.n[0] = srcV.n[0] * cost - srcV.n[2] * sint;
			dstV.n[2] = srcV.n[0] * sint + srcV.n[2] * cost;
			//new_pos.x = pos.x*ct - pos.z*st;
			//new_pos.z = pos.x*st + pos.z*ct;
	}

	CModel*	pTwistModel;

	Real	time;
	Real	height;
	Real	angleDegMax;
};

//////////////////////////////////////////////////////////////////////////
//vec4 DoTwist( vec4 pos, float t )
//{
//	float st = sin(t);
//	float ct = cos(t);
//	vec4 new_pos;
//
//	new_pos.x = pos.x*ct - pos.z*st;
//	new_pos.z = pos.x*st + pos.z*ct;
//
//	new_pos.y = pos.y;
//	new_pos.w = pos.w;
//
//	return( new_pos );
//}
//////////////////////////////////////////////////////////////////////////
// float angle_deg = angle_deg_max*sin(time);
// float angle_rad = angle_deg * 3.14159 / 180.0;
// 
// vec4 twistedPosition = DoTwist( gl_Vertex, (height*0.1 + gl_Vertex.y)/height * angle_rad );
// vec4 twistedNormal = DoTwist( vec4(gl_Normal, 0.0), (height*0.1 + gl_Vertex.y)/height * angle_rad );


#endif