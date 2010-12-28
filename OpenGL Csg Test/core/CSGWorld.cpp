
// Author: Greg Santucci, 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "CSGWorld.h"

#include "BooleanModeller.h"
#include "Solid.h"
#include "Color.h"
#include "IntSet.h"
#include "FaceSet.h"
#include "VertexSet.h"
#include "ColorSet.h"

#include "Color.h"

#include <stdlib.h>
#include <time.h>

CSGWorld * g_pCSGWorld = 0;

CSGWorld::CSGWorld()
{
	srand(::time(0));

	g_pCSGWorld = this;

	m_nPaletteIndex = 0;

	m_Palette.push_back(new Color(255,  0,    0  ));
	m_Palette.push_back(new Color(0,    255,  0  ));
	m_Palette.push_back(new Color(0,    0,    255));

	m_Palette.push_back(new Color(255,  255,  0  ));
	m_Palette.push_back(new Color(0,    255,  255));
	m_Palette.push_back(new Color(255,  0,    255));

	m_Palette.push_back(new Color(255,  127,  127));
	m_Palette.push_back(new Color(127,  255,  127));
	m_Palette.push_back(new Color(127,  127,  255));

	m_Palette.push_back(new Color(255,  255,  127));
	m_Palette.push_back(new Color(127,  255,  255));
	m_Palette.push_back(new Color(255,  127,  255));
}

CSGWorld::~CSGWorld()
{
	for(int i = 0; i < m_Solids.size(); i++)
	{
		Solid * pSolid = m_Solids[i];
		delete pSolid;
	}

	for(int i = 0; i < m_Palette.size(); i++)
	{
		Color * pColor = m_Palette[i];
		delete pColor;
	}
}

void CSGWorld::LoadCube()
{
	Solid * pSolid = new Solid("box.txt", *m_Palette[m_nPaletteIndex], 0,0,0);

	m_Solids.push_back(pSolid);
	m_RenderSwitchboard.push_back(true);
	m_nPaletteIndex = (m_nPaletteIndex + 1) % m_Palette.size();
}

void CSGWorld::LoadSphere()
{
	Solid * pSolid = new Solid("sphere.txt", *m_Palette[m_nPaletteIndex], 0,0,0);

	m_Solids.push_back(pSolid);
	m_RenderSwitchboard.push_back(true);
	m_nPaletteIndex = (m_nPaletteIndex + 1) % m_Palette.size();
}

void CSGWorld::LoadCone()
{
	Solid * pSolid = new Solid("cone.txt", *m_Palette[m_nPaletteIndex], 0,0,0);

	m_Solids.push_back(pSolid);
	m_RenderSwitchboard.push_back(true);
	m_nPaletteIndex = (m_nPaletteIndex + 1) % m_Palette.size();
}

void CSGWorld::LoadCylinder()
{
	Solid * pSolid = new Solid("cylinder.txt", *m_Palette[m_nPaletteIndex], 0,0,0);

	m_Solids.push_back(pSolid);
	m_RenderSwitchboard.push_back(true);
	m_nPaletteIndex = (m_nPaletteIndex + 1) % m_Palette.size();
}

int CSGWorld::GetNumSolids()
{
	return m_Solids.size();
}

Solid * CSGWorld::GetSolid(int i)
{
	if(i < 0) return 0;
	if(i >= m_Solids.size()) return 0;

	return m_Solids[i];
}

void CSGWorld::Difference(int i, int j)
{
	Solid * pSolid1 = GetSolid(i);
	Solid * pSolid2 = GetSolid(j);

	if(pSolid1 == 0) return;
	if(pSolid2 == 0) return;

	BooleanModeller * pModeller = new BooleanModeller(pSolid1, pSolid2);

	Solid * pResult = pModeller->getDifference();

	delete pModeller;

	printf("Num vertices  %10d\n", pResult->vertices.GetSize());
	printf("Num indices   %10d\n", pResult->indices.GetSize());
	printf("Num colors    %10d\n", pResult->colors.GetSize());

	m_Solids.push_back(pResult);
	m_RenderSwitchboard.push_back(true);
	// Don't change palette index
	m_RenderSwitchboard[i] = false;
	m_RenderSwitchboard[j] = false;
}

void CSGWorld::Union(int i, int j)
{
	Solid * pSolid1 = GetSolid(i);
	Solid * pSolid2 = GetSolid(j);

	if(pSolid1 == 0) return;
	if(pSolid2 == 0) return;

	BooleanModeller * pModeller = new BooleanModeller(pSolid1, pSolid2);

	Solid * pResult = pModeller->getUnion();

	delete pModeller;

	printf("Num vertices  %10d\n", pResult->vertices.GetSize());
	printf("Num indices   %10d\n", pResult->indices.GetSize());
	printf("Num colors    %10d\n", pResult->colors.GetSize());

	m_Solids.push_back(pResult);
	m_RenderSwitchboard.push_back(true);
	// Don't change palette index
	m_RenderSwitchboard[i] = false;
	m_RenderSwitchboard[j] = false;
}

void CSGWorld::Intersection(int i, int j)
{
	Solid * pSolid1 = GetSolid(i);
	Solid * pSolid2 = GetSolid(j);

	if(pSolid1 == 0) return;
	if(pSolid2 == 0) return;

	BooleanModeller * pModeller = new BooleanModeller(pSolid1, pSolid2);

	Solid * pResult = pModeller->getIntersection();

	delete pModeller;

	printf("Num vertices  %10d\n", pResult->vertices.GetSize());
	printf("Num indices   %10d\n", pResult->indices.GetSize());
	printf("Num colors    %10d\n", pResult->colors.GetSize());

	m_Solids.push_back(pResult);
	m_RenderSwitchboard.push_back(true);
	// Don't change palette index
	m_RenderSwitchboard[i] = false;
	m_RenderSwitchboard[j] = false;
}

void CSGWorld::Render()
{
	for(int i = 0; i < m_Solids.size(); i++)
	{
		Solid * pSolid = m_Solids[i];

		if(m_RenderSwitchboard[i])
		{
			pSolid->Render();
		}
	}
}

//void CSGWorld::Build_Ex1()
//{
//	pSolid1 = new Solid("box.txt", Color(0,0,255), 0,0,0);// 0, 150, -120);
//	pSolid2 = new Solid("cone.txt", Color(255,0,0), 0,0,0);// -120, 150, 0);
//
//	pSolid1->Translate(Vector(0.2, -0.2, 0));
//	pSolid2->Scale(Vector(0.4f, 0.3f, 2.8f));
//
//	pModeller = new BooleanModeller(pSolid2, pSolid1);
//
//	pUnionResult = pModeller->getUnion();
//	pIntersectionResult = pModeller->getIntersection();
//	pDifferenceResult = pModeller->getDifference();
//
//	printf("              %10s %10s %10s\n", "Union", "Intersection", "Difference");
//	printf("Num vertices  %10d %10d %10d\n", pUnionResult->vertices.GetSize(), pIntersectionResult->vertices.GetSize(), pDifferenceResult->vertices.GetSize());
//	printf("Num indices   %10d %10d %10d\n", pUnionResult->indices.GetSize(), pIntersectionResult->indices.GetSize(), pDifferenceResult->indices.GetSize());
//	printf("Num colors    %10d %10d %10d\n", pUnionResult->colors.GetSize(), pIntersectionResult->colors.GetSize(), pDifferenceResult->colors.GetSize());
//}
//
//void CSGWorld::Build_Ex2()
//{
//	pSolid1 = new Solid("box.txt", Color(0,0,255), 0,0,0);// 0, 150, -120);
//	pSolid2 = new Solid("cone.txt", Color(255,0,0), 0,0,0);// -120, 150, 0);
//
//	pSolid1->Translate(Vector(0.2, -0.2, 0));
//	pSolid2->Scale(Vector(0.4f, 1.3f, 1.8f));
//
//	pMidModeller = new BooleanModeller(pSolid2, pSolid1);
//
//	Solid * pMidResult = pMidModeller->getDifference();
//
//	pSphere = new Solid("sphere.txt", Color(0,255,0), 0,0,0);//150, -120, 0);
//
//	pSphere->Scale(Vector(1.0f, 1.0f, 0.6f));
//	pSphere->Translate(Vector(0.5, 0, 0));
//
//	pModeller = new BooleanModeller(pMidResult, pSphere);
//
//	pUnionResult = pModeller->getUnion();
//	pIntersectionResult = pModeller->getIntersection();
//	pDifferenceResult = pModeller->getDifference();
//
//	printf("              %10s %10s %10s\n", "Union", "Intersection", "Difference");
//	printf("Num vertices  %10d %10d %10d\n", pUnionResult->vertices.GetSize(), pIntersectionResult->vertices.GetSize(), pDifferenceResult->vertices.GetSize());
//	printf("Num indices   %10d %10d %10d\n", pUnionResult->indices.GetSize(), pIntersectionResult->indices.GetSize(), pDifferenceResult->indices.GetSize());
//	printf("Num colors    %10d %10d %10d\n", pUnionResult->colors.GetSize(), pIntersectionResult->colors.GetSize(), pDifferenceResult->colors.GetSize());
//}
//
//void CSGWorld::Render()
//{
//	//pUnionResult->Render();
//	//pDifferenceResult->Render();
//	pIntersectionResult->Render();
//
//	extern QtAdaptor * g_pImagineQtAdaptor;
//	g_pImagineQtAdaptor->RenderText(0,0,0,"hello");
//}
//
