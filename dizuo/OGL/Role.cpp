#include	"Role.h"
#include	"Logger.h"


Role::Role()
: m_fTimSec(0)
, m_fLastTimSec(0)
, m_bStop(false)
{
	Active();
	m_Type = ROLE;

	direction = 45.0f;
	
	m_vVelocity.setValue(0.0, 0.0, 0.0);
	//m_vAcceleration.setValue(0.0, 0.0, 0.0);
}

bool Role::Init()
{
	m_vVelocity.setValue(0.0, 0.0, 15.0);

	bool success1 = m_Role.LoadModel( "Data/Models/Ogros.md2" );
	bool success2 = m_Role.LoadSkin( "Data/Models/igdosh.pcx" );
	m_Role.SetAnim( RUN );			
	m_Role.ScaleModel( 0.10 );

	bool success3 = m_Weapon.LoadModel( "Data/Models/Weapon.md2" );
	bool success4 = m_Weapon.LoadSkin( "Data/Models/Weapon.pcx" );
	m_Weapon.SetAnim( RUN );
	m_Weapon.ScaleModel( 0.10 );

	return success1 && success2 && success3 && success4;
}

void Role::Draw()
{
	float cosYaw = (float)cos( gtl::DegToRad(direction) );
	float sinYaw = (float)sin( gtl::DegToRad(direction) ); 

	// m_fTimSec在递增, 每次去两次时间差值.
	float speed = m_vVelocity.z() * ( m_fTimSec - m_fLastTimSec );
	m_fLastTimSec = m_fTimSec;

	if(m_bStop != true) {
		direction += 0.3;

		m_vPos.x() += float(cosYaw)*speed;
		m_vPos.z() += float(sinYaw)*speed;
	}

	if ((direction >= 360.0f) || (direction <= -360.0f))
		direction = 0.0f;
	
	m_Role.DrawModel(m_fTimSec);
	m_Weapon.DrawModel(m_fTimSec);
}