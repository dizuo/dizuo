
// Author: Greg Santucci, 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef CSGWORLD_H
#define CSGWORLD_H

#include <vector>

class BooleanModeller;
class Solid;
class Color;

class CSGWorld
{
public:
	CSGWorld();
	virtual ~CSGWorld();

	void LoadCube();
	void LoadSphere();
	void LoadCone();
	void LoadCylinder();

	int GetNumSolids();
	Solid * GetSolid(int i);

	void Difference(int i, int j);
	void Union(int i, int j);
	void Intersection(int i, int j);

	void Render();

	typedef std::vector<Solid *> SolidSet;
	typedef std::vector<bool> BoolSet;
	typedef std::vector<Color *> Palette;

	SolidSet m_Solids;
	BoolSet m_RenderSwitchboard;

	int m_nPaletteIndex;
	Palette m_Palette;
};

#endif // CSGWORLD_H
