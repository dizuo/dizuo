// template<unsigned int sequence>
// struct TypeBase
// {
// };
// 
// typedef TypeBase<0x00000001> TOBJ_TYPE_PERSON;
// typedef TypeBase<0x00000002> TOBJ_TYPE_DOG;
// typedef TypeBase<0x00000003> TOBJ_TYPE_CAT;
// 
// typedef TypeBase<0x00000004> TOBJ_ID_ME;
// typedef TypeBase<0x00000005> TOBJ_ID_LP;
// 
// typedef TypeBase<0x00000006> TRET_SUCCESS;
// typedef TypeBase<0x00000007> TRET_FAIL;
// typedef TypeBase<0x00000008> TRET_PENDING;
// typedef TypeBase<0x00000009> TRET_UNKONW;

template<typename Type >
struct TypeBase
{
	TypeBase( const Type& type ) : m_Type( type )                {}
	bool    operator==( const TypeBase<Type>& otherOne ) const    { return (this->m_Type == otherOne.m_Type); }
	Type    m_Type;
};

typedef TypeBase< unsigned int > TObjectType;
const TObjectType    TOBJ_TYPE_PERSON        = 0x00000001;
const TObjectType    TOBJ_TYPE_DOG            = 0x00000002;
const TObjectType    TOBJ_TYPE_CAT            = 0x00000003;

typedef TypeBase< unsigned int > TObjectIdentity;
const TObjectIdentity TOBJ_ID_ME            = 0x00000001;
const TObjectIdentity TOBJ_ID_LP            = 0x00000002;

typedef TypeBase< unsigned int > TResultType;
const TResultType    TRET_SUCCESS            = 0x00000001;
const TResultType    TRET_FAIL            = 0x00000002;
const TResultType    TRET_PENDING            = 0x00000003;
const TResultType    TRET_UNKNOWN            = 0xFFFFFFFF;

struct Man_Tag{static TObjectType                ObjectType;};
TObjectType Man_Tag:: ObjectType = TOBJ_TYPE_PERSON;
struct Dog_Tag{static TObjectType                ObjectType;};
TObjectType Dog_Tag:: ObjectType = TOBJ_TYPE_DOG;
struct Cat_Tag{static TObjectType                ObjectType;};
TObjectType Cat_Tag:: ObjectType = TOBJ_TYPE_CAT;
//------------------------------------------------------------------------
//**************************** Class declaration *************************
//------------------------------------------------------------------------
template< typename Type >
class IObject
{
public:

	// Property functions
	virtual TObjectIdentity        GetIdentity    () const        = 0;
	virtual TObjectType        GetType        () const        = 0;

	// Logic functions
	virtual TResultType        Init        ()            = 0;
	virtual TResultType        Update        ()            = 0;
	virtual TResultType        Paint        ()            = 0;
	virtual TResultType        UnInit        ()            = 0;
};

template< typename Type >
class TObject : public IObject< Type >
{
public:

	TObject( TObjectIdentity id ) : m_Identity( id ), m_Type( Type::ObjectType )        {}
	virtual ~TObject()                        {}

	virtual    TObjectIdentity        GetIdentity    () const    { return m_Identity;    } 
	virtual TObjectType        GetType        () const    { return m_Type;    }

	virtual TResultType        Init        ()        { return TRET_SUCCESS;    }
	virtual TResultType        Update        ()        { return TRET_SUCCESS;    }
	virtual TResultType        Paint        ()        { return TRET_SUCCESS;    }
	virtual TResultType        UnInit        ()        { return TRET_SUCCESS;    }

protected:

	TObjectType                m_Type;
	TObjectIdentity                m_Identity;
};

typedef TObject< Man_Tag >    TPerson;

int main()
{
	TPerson me( TOBJ_ID_ME );
	TPerson lp( TOBJ_ID_LP );
}