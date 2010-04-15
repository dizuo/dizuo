#include "CLoft.h"


CLoft::CLoft(){}
CLoft::~CLoft(){}

bool CLoft::Setup()
{
	//////////////////////////////////////////////////////////////////////////
	// create the model's basic informations
	if ( !path.GetSize() || !shape.GetSize() || path.GetSize() < 3)
	{
		return false;
	}
	size_t pathSize = path.GetSize();
	size_t shapeSize = shape.GetSize();

	verNum = pathSize * (shapeSize + 1);
	indiceNum = 2*(pathSize-1)*shapeSize*3;

	pVertex = new CVertex[verNum];
	pTriIndices = new int[indiceNum];

	//path的每一个结点对应一个shape
	//pLoftShapes[0]为重心位于原点的shape
	CCurve* pLoftShapes = new CCurve[pathSize+1];	
	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//#########################################################################
	//////////////////////////////////////////////////////////////////////////
	RVec3 pathNorm = path.GetNormal();
	RVec3 shapeNorm = shape.GetNormal();	

	//assert: pathNorm.dot(shapeNorm) = 0
	if ( pathNorm.dot(shapeNorm) )
	{
		//
		return false;
	}

	//compute the shape's center of gravity
	RVec3 shapeCenter = shape.GetGravCenter();

	//求得重心位于原点的shape
	gtl::Matrix4f tranMat;
	tranMat.setTranslate( -shapeCenter );
	_ProcessCurve(shape, tranMat, pLoftShapes[0]);

	//########################################################################
	for (int p=0; p<pathSize; p++)
	{
		gtl::Matrix4f mat1;

		if (p==0)
		{
			_GetShapeMatrix(shapeNorm, path.GetSlice(0), path[0], mat1);
			_ProcessCurve(pLoftShapes[0], mat1, pLoftShapes[p+1]);
		} else if (p<pathSize-1)
		{	
			RVec3 cutPlaneNorm;		//角分面法线
			RVec3 rayDir(path[p]-path[p-1]); 	rayDir.normalize();

			_ComputePlaneEquation(path[p-1], path[p+1], path[p], pathNorm, cutPlaneNorm);

			for (int i=0; i<shapeSize; i++)
			{
				RVec3 rayOrig(pLoftShapes[p][i]);
				RVec3 temVec;
				_GetRayAndPlaneIntersetPnt(rayOrig, rayDir, cutPlaneNorm, path[p], temVec);

				pLoftShapes[p+1].AddPnt(temVec);
			}

			pLoftShapes[p+1].ComputeNormal();
			pLoftShapes[p+1].ComputeGravCenter();
		} else	//last section.
		{
			_GetShapeMatrix(shapeNorm, path[p]-path[p-1], path[p], mat1);	
			_ProcessCurve(pLoftShapes[0], mat1, pLoftShapes[p+1]);
		}

		//############transform#######
		//pLoftShape[p+1] ==> pVertex;
		Real texS = static_cast<Real>(p) / static_cast<Real>(pathSize-1);

		for (int i=0; i<shapeSize; i++)
		{
			RVec3 v = pLoftShapes[ p+1 ].GetSlice(i);	//get the path's first slice
			RVec3 n = v.cross( pLoftShapes[ p+1 ].GetNormal() );	//compute the normal
			n.normalize();
			
			//compute the texture coordinates
			Real texT = static_cast<Real>(i) / static_cast<Real>(shapeSize);

			int offset = shapeSize + 1;
			if (i==0)
			{
				pVertex[ p*offset + i + shapeSize ] = CVertex(pLoftShapes[ p+1 ][i].x(), pLoftShapes[ p+1 ][i].y(), pLoftShapes[ p+1 ][i].z(),
					texS, 1.0f,
					n.x(), n.y(), n.z());
			}
			pVertex[ p*offset + i ] = CVertex(pLoftShapes[ p+1 ][i].x(), pLoftShapes[ p+1 ][i].y(), pLoftShapes[ p+1 ][i].z(),
				texS, texT,
				n.x(), n.y(), n.z());
		}
	}

	//delete the pLoftShapes...

	//4/2/2010 RYF
	//setup bbox;
	SetupBBox();
	//########################################################################
	_SetupIndices();

	return true;
}

void CLoft::_SetupIndices()
{
	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//########################################################################
	//////////////////////////////////////////////////////////////////////////
	//shape的顶点数=shape的线段数
	int shapeStep = shape.GetSize();

	for (int p=0; p<path.GetSize()-1; p++)	//i only control the loop times
	{
		for (int i=0; i<shape.GetSize(); i++)
		{
			// triangle 0
			pTriIndices[ ((p*shapeStep+i)*2)*3+0 ]	= p*(shapeStep+1)+i;
			pTriIndices[ ((p*shapeStep+i)*2)*3+1 ]	= (p+1)*(shapeStep+1)+i;
			pTriIndices[ ((p*shapeStep+i)*2)*3+2 ]	= p*(shapeStep+1)+i+1;

			// triangle 1
			pTriIndices[ ((p*shapeStep+i)*2+1)*3+0 ]= p*(shapeStep+1)+i+1;
			pTriIndices[ ((p*shapeStep+i)*2+1)*3+1 ]= (p+1)*(shapeStep+1)+i;
			pTriIndices[ ((p*shapeStep+i)*2+1)*3+2 ]= (p+1)*(shapeStep+1)+i+1;
		}
	}// two loops
}

//使用mat变换srcShape的顶点结果存储在dstCurve
void CLoft::_ProcessCurve(const CCurve &srcCurve, 
						  const gtl::Matrix4f& mat,
						  CCurve& dstCurve)
{
	for (int i=0; i<srcCurve.GetSize(); i++)
	{
		RVec3 newVec;
		mat.multVecMatrix(srcCurve[i], newVec);
		dstCurve.AddPnt(newVec);
	}

	dstCurve.ComputeNormal();
	dstCurve.ComputeGravCenter();
}

//////////////////////////////////////////////////////////////////////////
//shape已经位于原点
//1.旋转shape到p0p1垂直的平面上
//2.将新的shape移到这个位置：shape的重心和path[0]重合
//3.最终相乘顺序：vec = vec * tranMat1 * rotMat * tranMat2
//计算第一段和最后一段
//////////////////////////////////////////////////////////////////////////
void CLoft::_GetShapeMatrix(const RVec3& shapeNorm,
							const RVec3& slice, const RVec3& pathPnt,
							gtl::Matrix4f& finalMat)
{
	RVec3 rot_to = slice;

	gtl::Quatf	rotQuat(shapeNorm, rot_to);
	gtl::Matrix4f rotMat = rotQuat.getMatrix();

	gtl::Matrix4f tranMat;
	tranMat.setTranslate( pathPnt );

	finalMat = rotMat * tranMat;
}

//////////////////////////////////////////////////////////////////////////
//	centerPnt为角的顶点，同时也是目标平面上一点
//	planeNorm为平面的法线 
//	根据:centerPnt & planeNorm即可表示角分面
//////////////////////////////////////////////////////////////////////////
void CLoft::_ComputePlaneEquation(const RVec3 &leftPnt, const RVec3 &rightPnt, //input
								  const RVec3 &centerPnt,	//input
								  const RVec3 &pathNorm,	//input
								  RVec3 &planeNorm)			//output
{
	RVec3 ray1 = leftPnt - centerPnt;	ray1.normalize();
	RVec3 ray2 = rightPnt - centerPnt;	ray2.normalize();

	RVec3 ray = ray1 + ray2;			ray.normalize();

	planeNorm = ray.cross(pathNorm);	planeNorm.normalize();
}

//////////////////////////////////////////////////////////////////////////
// 计算射线和平面的交点
// rayDir & planeNorm 必须为单位向量
//////////////////////////////////////////////////////////////////////////
void CLoft::_GetRayAndPlaneIntersetPnt(const RVec3& rayOrig, const RVec3& rayDir, 
								const RVec3& planeNorm, const RVec3& planePnt, 
								RVec3& point)
{
	Real v1 = rayOrig.dot(planeNorm);
	Real v2 = rayDir.dot(planeNorm);
	Real v3 = planePnt.dot(planeNorm);

	if (!v2)
		return;

	Real tFactor = (v3 - v1) / v2;
	point = rayOrig + tFactor*rayDir;
}

