/*
_______________________________________________________________________
__________________________ G E O M E T R Y ____________________________
|
| THIS FILE IS PART OF THE GEOMETRY TEMPLATE LIBRARY.
| USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     
| GOVERNED BY A BSD-STYLE SOURCE LICENSE.
| PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       
_______________________________________________________________________
_______________________________________________________________________
//							Modify		   							 //
//	Add function:	MakeProjection( fovy, aspectRation, zNear, zFar) //	
//					MakeView(Vec3 eye, Vec3 position, Vec3 up)		 //
//------------------------Dizuo@126.com 15/5/2008--------------------//
*/
#ifndef MATRIX4_H
#define MATRIX4_H

// #include <gtl/gtl.hpp>
// #include <gtl/vec3.hpp>
// #include <gtl/quat.hpp>
#include "gtl.hpp"
#include "vec3.hpp"
#include "quat.hpp"

namespace gtl
{
    // forward declaration	
    template<typename Type> class Quat;

    /*!
    \class Matrix4 Matrix4.hpp geometry/Matrix4.hpp
    \brief Row-major 4x4 matrix class.
    \ingroup base

    This class is used by many other classes.

    \sa Matrix3  
    */
    template<typename Type>
    class Matrix4
    {
    public:
        //! The default constructor. The matrix will be identity.
        Matrix4()
        {
            makeIdentity();
        }

        //! Constructs a matrix instance with the given initial elements.
        Matrix4(const Type a11, const Type a12, const Type a13, const Type a14,
            const Type a21, const Type a22, const Type a23, const Type a24,
            const Type a31, const Type a32, const Type a33, const Type a34,
            const Type a41, const Type a42, const Type a43, const Type a44)
        {
            m_data[0][0] = a11; m_data[0][1] = a12; m_data[0][2] = a13; m_data[0][3] = a14;
            m_data[1][0] = a21; m_data[1][1] = a22; m_data[1][2] = a23; m_data[1][3] = a24;
            m_data[2][0] = a31; m_data[2][1] = a32; m_data[2][2] = a33; m_data[2][3] = a34;
            m_data[3][0] = a41; m_data[3][1] = a42; m_data[3][2] = a43; m_data[3][3] = a44;
        }

		//coll priority
		//Matrix4(const Type oglModelViewMat[16])
		Matrix4(const Type oglModelViewMat[16] )
		{
			for (int i=0; i<4; i++)
			{
				float *pRow = m_data[i];

				for (int j=0; j<4; j++)
				{
					pRow[j] = oglModelViewMat[i*4 + j];
				}
			}
		}

        //! Constructs a matrix instance with the initial elements from the \a matrix argument.
        Matrix4(const Matrix4<Type> & a_matrix)
        {
            *this = a_matrix;
        }

        //! Default destructor does nothing.
        virtual ~Matrix4(){}

        //! Set the matrix to be the identity matrix. \sa isIdentity().
        void makeIdentity()
        {
            m_data[0][0]=m_data[1][1]=m_data[2][2]=m_data[3][3] = (Type)1.0;
            m_data[0][1]=m_data[0][2]=m_data[0][3]=
                m_data[1][0]=m_data[1][2]=m_data[1][3]=
                m_data[2][0]=m_data[2][1]=m_data[2][3]=
                m_data[3][0]=m_data[3][1]=m_data[3][2] = (Type)0.0;
        }

        //! Check if matrix is identity. 
        bool isIdentity() const
        {
            return ((m_data[0][0] == 1.0) && (m_data[0][1] == (Type)0.0) && (m_data[0][2] == (Type)0.0) && (m_data[0][3] == (Type)0.0) &&
                (m_data[1][0] == 0.0) && (m_data[1][1] == (Type)1.0) && (m_data[1][2] == (Type)0.0) && (m_data[1][3] == (Type)0.0) &&
                (m_data[2][0] == 0.0) && (m_data[2][1] == (Type)0.0) && (m_data[2][2] == (Type)1.0) && (m_data[2][3] == (Type)0.0) &&
                (m_data[3][0] == 0.0) && (m_data[3][1] == (Type)0.0) && (m_data[3][2] == (Type)0.0) && (m_data[3][3] == (Type)1.0));
        }

        //! Inverse the current matrix.	
        Matrix4<Type> & invert()
        {
            // inversion with Cramer's Rule.
            Type tmp[12];
            Type dst[16];

            transpose();

            //calculate pairs for first 8 elements (cofactor)
            tmp[0]  = m_data[2][2] * m_data[3][3];
            tmp[1]  = m_data[3][2] * m_data[2][3];
            tmp[2]  = m_data[1][2] * m_data[3][3];
            tmp[3]  = m_data[3][2] * m_data[1][3];
            tmp[4]  = m_data[1][2] * m_data[2][3];
            tmp[5]  = m_data[2][2] * m_data[1][3];
            tmp[6]  = m_data[0][2] * m_data[3][3];
            tmp[7]  = m_data[3][2] * m_data[0][3];
            tmp[8]  = m_data[0][2] * m_data[2][3];
            tmp[9]  = m_data[2][2] * m_data[0][3];
            tmp[10] = m_data[0][2] * m_data[1][3];
            tmp[11] = m_data[1][2] * m_data[0][3];

            //calculate first 8 elements (cofactor)
            dst[0]  = tmp[0]*m_data[1][1] + tmp[3]*m_data[2][1] + tmp[4]*m_data[3][1];
            dst[0] -= tmp[1]*m_data[1][1] + tmp[2]*m_data[2][1] + tmp[5]*m_data[3][1];
            dst[1]  = tmp[1]*m_data[0][1] + tmp[6]*m_data[2][1] + tmp[9]*m_data[3][1];
            dst[1] -= tmp[0]*m_data[0][1] + tmp[7]*m_data[2][1] + tmp[8]*m_data[3][1];
            dst[2]  = tmp[2]*m_data[0][1] + tmp[7]*m_data[1][1] + tmp[10]*m_data[3][1];
            dst[2] -= tmp[3]*m_data[0][1] + tmp[6]*m_data[1][1] + tmp[11]*m_data[3][1];
            dst[3]  = tmp[5]*m_data[0][1] + tmp[8]*m_data[1][1] + tmp[11]*m_data[2][1];
            dst[3] -= tmp[4]*m_data[0][1] + tmp[9]*m_data[1][1] + tmp[10]*m_data[2][1];
            dst[4]  = tmp[1]*m_data[1][0] + tmp[2]*m_data[2][0] + tmp[5]*m_data[3][0];
            dst[4] -= tmp[0]*m_data[1][0] + tmp[3]*m_data[2][0] + tmp[4]*m_data[3][0];
            dst[5]  = tmp[0]*m_data[0][0] + tmp[7]*m_data[2][0] + tmp[8]*m_data[3][0];
            dst[5] -= tmp[1]*m_data[0][0] + tmp[6]*m_data[2][0] + tmp[9]*m_data[3][0];
            dst[6]  = tmp[3]*m_data[0][0] + tmp[6]*m_data[1][0] + tmp[11]*m_data[3][0];
            dst[6] -= tmp[2]*m_data[0][0] + tmp[7]*m_data[1][0] + tmp[10]*m_data[3][0];
            dst[7]  = tmp[4]*m_data[0][0] + tmp[9]*m_data[1][0] + tmp[10]*m_data[2][0];
            dst[7] -= tmp[5]*m_data[0][0] + tmp[8]*m_data[1][0] + tmp[11]*m_data[2][0];

            //calculate pairs for second 8 elements (cofactors)
            tmp[0]  = m_data[2][0]*m_data[3][1];
            tmp[1]  = m_data[3][0]*m_data[2][1];
            tmp[2]  = m_data[1][0]*m_data[3][1];
            tmp[3]  = m_data[3][0]*m_data[1][1];
            tmp[4]  = m_data[1][0]*m_data[2][1];
            tmp[5]  = m_data[2][0]*m_data[1][1];
            tmp[6]  = m_data[0][0]*m_data[3][1];
            tmp[7]  = m_data[3][0]*m_data[0][1];
            tmp[8]  = m_data[0][0]*m_data[2][1];
            tmp[9]  = m_data[2][0]*m_data[0][1];
            tmp[10] = m_data[0][0]*m_data[1][1];
            tmp[11] = m_data[1][0]*m_data[0][1];

            //calculate second 8 elements (cofactors)
            dst[8]   = tmp[0]*m_data[1][3] + tmp[3]*m_data[2][3] + tmp[4]*m_data[3][3];
            dst[8]  -= tmp[1]*m_data[1][3] + tmp[2]*m_data[2][3] + tmp[5]*m_data[3][3];
            dst[9]   = tmp[1]*m_data[0][3] + tmp[6]*m_data[2][3] + tmp[9]*m_data[3][3];
            dst[9]  -= tmp[0]*m_data[0][3] + tmp[7]*m_data[2][3] + tmp[8]*m_data[3][3];
            dst[10]  = tmp[2]*m_data[0][3] + tmp[7]*m_data[1][3] + tmp[10]*m_data[3][3];
            dst[10] -= tmp[3]*m_data[0][3] + tmp[6]*m_data[1][3] + tmp[11]*m_data[3][3];
            dst[11]  = tmp[5]*m_data[0][3] + tmp[8]*m_data[1][3] + tmp[11]*m_data[2][3];
            dst[11] -= tmp[4]*m_data[0][3] + tmp[9]*m_data[1][3] + tmp[10]*m_data[2][3];
            dst[12]  = tmp[2]*m_data[2][2] + tmp[5]*m_data[3][2] + tmp[1]*m_data[1][2];
            dst[12] -= tmp[4]*m_data[3][2] + tmp[0]*m_data[1][2] + tmp[3]*m_data[2][2];
            dst[13]  = tmp[8]*m_data[3][2] + tmp[0]*m_data[0][2] + tmp[7]*m_data[2][2];
            dst[13] -= tmp[6]*m_data[2][2] + tmp[9]*m_data[3][2] + tmp[1]*m_data[0][2];
            dst[14]  = tmp[6]*m_data[1][2] + tmp[11]*m_data[3][2] + tmp[3]*m_data[0][2];
            dst[14] -= tmp[10]*m_data[3][2] + tmp[2]*m_data[0][2] + tmp[7]*m_data[1][2];
            dst[15]  = tmp[10]*m_data[2][2] + tmp[4]*m_data[0][2] + tmp[9]*m_data[1][2];
            dst[15] -= tmp[8]*m_data[1][2] + tmp[11]*m_data[2][2] + tmp[5]*m_data[0][2];

            //calculate determinant
            Type det = (Type)1.0 / (m_data[0][0]*dst[0] + m_data[1][0]*dst[1] + m_data[2][0]*dst[2] + m_data[3][0]*dst[3]);

            //calculate matrix inverse
            m_data[0][0] = dst[0] * det; m_data[0][1] = dst[4] * det; m_data[0][2] = dst[8] * det; m_data[0][3] = dst[12] * det;
            m_data[1][0] = dst[1] * det; m_data[1][1] = dst[5] * det; m_data[1][2] = dst[9] * det; m_data[1][3] = dst[13] * det;
            m_data[2][0] = dst[2] * det; m_data[2][1] = dst[6] * det; m_data[2][2] = dst[10] * det; m_data[2][3] = dst[14] * det;
            m_data[3][0] = dst[3] * det; m_data[3][1] = dst[7] * det; m_data[3][2] = dst[11] * det; m_data[3][3] = dst[15] * det;

            return *this;
        }


        //! Return a new matrix which is the inverse matrix of this.	
        Matrix4<Type> inverse() const
        {
            Matrix4<Type> mat(*this);

            return mat.invert();
        }

        //! Transpose the current matrix.
        Matrix4<Type> & transpose()
        {
            *this = Matrix4<Type>(m_data[0][0], m_data[1][0], m_data[2][0], m_data[3][0],
                m_data[0][1], m_data[1][1], m_data[2][1], m_data[3][1],
                m_data[0][2], m_data[1][2], m_data[2][2], m_data[3][2],
                m_data[0][3], m_data[1][3], m_data[2][3], m_data[3][3]);
            return *this;
        }

        //! Returns the determinant of the 3x3 submatrix specified by the row and column indices.
        Type det3(int r1, int r2, int r3, int c1, int c2, int c3) const
        {
            // More or less directly from "Advanced Engineering Mathematics"
            // (E. Kreyszig), 6th edition.
            Type a11 = m_data[r1][c1];
            Type a12 = m_data[r1][c2];
            Type a13 = m_data[r1][c3];
            Type a21 = m_data[r2][c1];
            Type a22 = m_data[r2][c2];
            Type a23 = m_data[r2][c3];
            Type a31 = m_data[r3][c1];
            Type a32 = m_data[r3][c2];
            Type a33 = m_data[r3][c3];

            Type M11 = a22 * a33 - a32 * a23;
            Type M21 = -(a12 * a33 - a32 * a13);
            Type M31 = a12 * a23 - a22 * a13;

            return (a11 * M11 + a21 * M21 + a31 * M31);
        }

        //! Returns the determinant of the upper left 3x3 submatrix.
        Type det3() const
        {
            return this->det3(0, 1, 2, 0, 1, 2);
        }

        //! Returns the determinant of the matrix.
        Type det4() const
        {
            Type det = 0.0;
            det += m_data[0][0] * det3(1, 2, 3, 1, 2, 3);
            det -= m_data[1][0] * det3(0, 2, 3, 1, 2, 3);
            det += m_data[2][0] * det3(0, 1, 3, 1, 2, 3);
            det -= m_data[3][0] * det3(0, 1, 2, 1, 2, 3);
            return det;
        }

        //! Check if the \a a_matrix matrix is equal to this one, within the given tolerance value.
        bool equals(const Matrix4<Type> & a_matrix, Type a_tolerance=1E-2) const
        {
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    if (std::abs(m_data[i][j] - a_matrix.m_data[i][j]) > a_tolerance)
                        return false;
                }
            }
            return true;
        }

        //! Return pointer to the matrix' 4x4 array.
		Type* operator* ()
		{ 
			return &(m_data[0][0]); 
		}
/*      operator Type * ()
        { 
            return &(m_data[0][0]); 
        }*/
		

        //! Returns pointer to the 4 element array representing a matrix row. \a i should be within [0, 3].
        Type * operator [](int i)
        { 
            return m_data[i]; 
        }

        //! Returns pointer to the 4 element array representing a matrix row. \a i should be within [0, 3].
        const Type * operator [](int i) const
        { 
            return m_data[i]; 
        }

        //! Assignment operator. Copies the elements from \a a_matrix to the matrix.
        Matrix4<Type> & operator =(const Matrix4<Type> & m)
        {
            m_data[0][0] = m.m_data[0][0];
            m_data[0][1] = m.m_data[0][1];
            m_data[0][2] = m.m_data[0][2];
            m_data[0][3] = m.m_data[0][3];
            m_data[1][0] = m.m_data[1][0];
            m_data[1][1] = m.m_data[1][1];
            m_data[1][2] = m.m_data[1][2];
            m_data[1][3] = m.m_data[1][3];
            m_data[2][0] = m.m_data[2][0];
            m_data[2][1] = m.m_data[2][1];
            m_data[2][2] = m.m_data[2][2];
            m_data[2][3] = m.m_data[2][3];
            m_data[3][0] = m.m_data[3][0];
            m_data[3][1] = m.m_data[3][1];
            m_data[3][2] = m.m_data[3][2];
            m_data[3][3] = m.m_data[3][3];

            return *this;
        }

        Matrix4<Type> & operator =(const Quat<Type> & a_quat)
        {
            setRotate(a_quat);

            return *this;
        }

        //! Right-multiply with the \a a_matrix matrix. \sa multRight().
        Matrix4<Type> & operator *=(const Matrix4<Type> & a_matrix)
        {
            return multRight(a_matrix);
        }

        //! Multiplies matrix \a m1 with matrix \a m2 and returns the resultant matrix.
        friend Matrix4<Type> operator *(const Matrix4<Type> & m1, const Matrix4<Type> & m2)
        { 
            Matrix4<Type> m = m1; 
            m *= m2; 
            return m; 
        }

        /*! Compare matrices to see if they are equal. For two matrices to be equal,
        all their individual elements must be equal.

        \sa equals().
        */
        friend bool operator ==(const Matrix4<Type> & m1, const Matrix4<Type> & m2)
        {
            return (
                m1.m_data[0][0] == m2.m_data[0][0] &&
                m1.m_data[0][1] == m2.m_data[0][1] &&
                m1.m_data[0][2] == m2.m_data[0][2] &&
                m1.m_data[0][3] == m2.m_data[0][3] &&

                m1.m_data[1][0] == m2.m_data[1][0] &&
                m1.m_data[1][1] == m2.m_data[1][1] &&
                m1.m_data[1][2] == m2.m_data[1][2] &&
                m1.m_data[1][3] == m2.m_data[1][3] &&

                m1.m_data[2][0] == m2.m_data[2][0] &&
                m1.m_data[2][1] == m2.m_data[2][1] &&
                m1.m_data[2][2] == m2.m_data[2][2] &&
                m1.m_data[2][3] == m2.m_data[2][3] &&

                m1.m_data[3][0] == m2.m_data[3][0] &&
                m1.m_data[3][1] == m2.m_data[3][1] &&
                m1.m_data[3][2] == m2.m_data[3][2] &&
                m1.m_data[3][3] == m2.m_data[3][3]
            );
        }


        friend bool operator !=(const Matrix4<Type> & m1, const Matrix4<Type> & m2)
        { 
            return !(m1 == m2);
        }

        void setRotate(const Quat<Type> & a_quat)
        {
            *this = a_quat.getMatrix();
        }

        //! Set matrix to be a pure scaling matrix. (no translation or rotation components)
        void setScale(const Type s)
        {
            setScale(Vec3<Type>(s,s,s));
        }

        //! Set matrix to be a pure scaling matrix. (no translation or rotation components)
        void setScale(const Vec3<Type> & s)
        {
            makeIdentity();
            m_data[0][0] = s[0];
            m_data[1][1] = s[1];
            m_data[2][2] = s[2];
        }

        //! Make this matrix into a pure translation matrix. (no scale or rotation components)
        void setTranslate(const Vec3<Type> & t)
        {
            makeIdentity();
            m_data[3][0] = t[0];
            m_data[3][1] = t[1];
            m_data[3][2] = t[2];
        }

        //! Let this matrix be right-multiplied by m. Returns reference to self.
        Matrix4<Type> & multRight(const Matrix4<Type> & m)
        {
            // Trivial cases
            if(m.isIdentity()) return *this;
            else if(isIdentity()) return (*this = m);

            Matrix4<Type> tmp;

            tmp[0][0] = m_data[0][0]*m.m_data[0][0] + m_data[0][1]*m.m_data[1][0] + m_data[0][2]*m.m_data[2][0] + m_data[0][3]*m.m_data[3][0];
            tmp[0][1] = m_data[0][0]*m.m_data[0][1] + m_data[0][1]*m.m_data[1][1] + m_data[0][2]*m.m_data[2][1] + m_data[0][3]*m.m_data[3][1];
            tmp[0][2] = m_data[0][0]*m.m_data[0][2] + m_data[0][1]*m.m_data[1][2] + m_data[0][2]*m.m_data[2][2] + m_data[0][3]*m.m_data[3][2];
            tmp[0][3] = m_data[0][0]*m.m_data[0][3] + m_data[0][1]*m.m_data[1][3] + m_data[0][2]*m.m_data[2][3] + m_data[0][3]*m.m_data[3][3];
            tmp[1][0] = m_data[1][0]*m.m_data[0][0] + m_data[1][1]*m.m_data[1][0] + m_data[1][2]*m.m_data[2][0] + m_data[1][3]*m.m_data[3][0];
            tmp[1][1] = m_data[1][0]*m.m_data[0][1] + m_data[1][1]*m.m_data[1][1] + m_data[1][2]*m.m_data[2][1] + m_data[1][3]*m.m_data[3][1];
            tmp[1][2] = m_data[1][0]*m.m_data[0][2] + m_data[1][1]*m.m_data[1][2] + m_data[1][2]*m.m_data[2][2] + m_data[1][3]*m.m_data[3][2];
            tmp[1][3] = m_data[1][0]*m.m_data[0][3] + m_data[1][1]*m.m_data[1][3] + m_data[1][2]*m.m_data[2][3] + m_data[1][3]*m.m_data[3][3];
            tmp[2][0] = m_data[2][0]*m.m_data[0][0] + m_data[2][1]*m.m_data[1][0] + m_data[2][2]*m.m_data[2][0] + m_data[2][3]*m.m_data[3][0];
            tmp[2][1] = m_data[2][0]*m.m_data[0][1] + m_data[2][1]*m.m_data[1][1] + m_data[2][2]*m.m_data[2][1] + m_data[2][3]*m.m_data[3][1];
            tmp[2][2] = m_data[2][0]*m.m_data[0][2] + m_data[2][1]*m.m_data[1][2] + m_data[2][2]*m.m_data[2][2] + m_data[2][3]*m.m_data[3][2];
            tmp[2][3] = m_data[2][0]*m.m_data[0][3] + m_data[2][1]*m.m_data[1][3] + m_data[2][2]*m.m_data[2][3] + m_data[2][3]*m.m_data[3][3];
            tmp[3][0] = m_data[3][0]*m.m_data[0][0] + m_data[3][1]*m.m_data[1][0] + m_data[3][2]*m.m_data[2][0] + m_data[3][3]*m.m_data[3][0];
            tmp[3][1] = m_data[3][0]*m.m_data[0][1] + m_data[3][1]*m.m_data[1][1] + m_data[3][2]*m.m_data[2][1] + m_data[3][3]*m.m_data[3][1];
            tmp[3][2] = m_data[3][0]*m.m_data[0][2] + m_data[3][1]*m.m_data[1][2] + m_data[3][2]*m.m_data[2][2] + m_data[3][3]*m.m_data[3][2];
            tmp[3][3] = m_data[3][0]*m.m_data[0][3] + m_data[3][1]*m.m_data[1][3] + m_data[3][2]*m.m_data[2][3] + m_data[3][3]*m.m_data[3][3];

            return (*this = tmp);
        }

        //! Let this matrix be left-multiplied by m. Returns reference to self.
        Matrix4<Type> & multLeft(const Matrix4<Type> & m)
        {
            // Trivial cases
            if(m.isIdentity()) return *this;
            else if(isIdentity())	return (*this = m);

            Matrix4<Type> tmp;

            tmp[0][0] = m.m_data[0][0]*m_data[0][0] + m.m_data[0][1]*m_data[1][0] + m.m_data[0][2]*m_data[2][0] + m.m_data[0][3]*m_data[3][0];
            tmp[0][1] = m.m_data[0][0]*m_data[0][1] + m.m_data[0][1]*m_data[1][1] + m.m_data[0][2]*m_data[2][1] + m.m_data[0][3]*m_data[3][1];
            tmp[0][2] = m.m_data[0][0]*m_data[0][2] + m.m_data[0][1]*m_data[1][2] + m.m_data[0][2]*m_data[2][2] + m.m_data[0][3]*m_data[3][2];
            tmp[0][3] = m.m_data[0][0]*m_data[0][3] + m.m_data[0][1]*m_data[1][3] + m.m_data[0][2]*m_data[2][3] + m.m_data[0][3]*m_data[3][3];
            tmp[1][0] = m.m_data[1][0]*m_data[0][0] + m.m_data[1][1]*m_data[1][0] + m.m_data[1][2]*m_data[2][0] + m.m_data[1][3]*m_data[3][0];
            tmp[1][1] = m.m_data[1][0]*m_data[0][1] + m.m_data[1][1]*m_data[1][1] + m.m_data[1][2]*m_data[2][1] + m.m_data[1][3]*m_data[3][1];
            tmp[1][2] = m.m_data[1][0]*m_data[0][2] + m.m_data[1][1]*m_data[1][2] + m.m_data[1][2]*m_data[2][2] + m.m_data[1][3]*m_data[3][2];
            tmp[1][3] = m.m_data[1][0]*m_data[0][3] + m.m_data[1][1]*m_data[1][3] + m.m_data[1][2]*m_data[2][3] + m.m_data[1][3]*m_data[3][3];
            tmp[2][0] = m.m_data[2][0]*m_data[0][0] + m.m_data[2][1]*m_data[1][0] + m.m_data[2][2]*m_data[2][0] + m.m_data[2][3]*m_data[3][0];
            tmp[2][1] = m.m_data[2][0]*m_data[0][1] + m.m_data[2][1]*m_data[1][1] + m.m_data[2][2]*m_data[2][1] + m.m_data[2][3]*m_data[3][1];
            tmp[2][2] = m.m_data[2][0]*m_data[0][2] + m.m_data[2][1]*m_data[1][2] + m.m_data[2][2]*m_data[2][2] + m.m_data[2][3]*m_data[3][2];
            tmp[2][3] = m.m_data[2][0]*m_data[0][3] + m.m_data[2][1]*m_data[1][3] + m.m_data[2][2]*m_data[2][3] + m.m_data[2][3]*m_data[3][3];
            tmp[3][0] = m.m_data[3][0]*m_data[0][0] + m.m_data[3][1]*m_data[1][0] + m.m_data[3][2]*m_data[2][0] + m.m_data[3][3]*m_data[3][0];
            tmp[3][1] = m.m_data[3][0]*m_data[0][1] + m.m_data[3][1]*m_data[1][1] + m.m_data[3][2]*m_data[2][1] + m.m_data[3][3]*m_data[3][1];
            tmp[3][2] = m.m_data[3][0]*m_data[0][2] + m.m_data[3][1]*m_data[1][2] + m.m_data[3][2]*m_data[2][2] + m.m_data[3][3]*m_data[3][2];
            tmp[3][3] = m.m_data[3][0]*m_data[0][3] + m.m_data[3][1]*m_data[1][3] + m.m_data[3][2]*m_data[2][3] + m.m_data[3][3]*m_data[3][3];

            return (*this = tmp);
        }

        //! Multiplies matrix by given column vector, giving vector result. 
        void multMatrixVec(const Vec3<Type> & src, Vec3<Type> & dst) const
        {
            Type x = m_data[0][0]*src[0] + m_data[0][1]*src[1] + m_data[0][2]*src[2] + m_data[0][3];
            Type y = m_data[1][0]*src[0] + m_data[1][1]*src[1] + m_data[1][2]*src[2] + m_data[1][3];
            Type z = m_data[2][0]*src[0] + m_data[2][1]*src[1] + m_data[2][2]*src[2] + m_data[2][3];
            Type w = m_data[3][0]*src[0] + m_data[3][1]*src[1] + m_data[3][2]*src[2] + m_data[3][3];

            dst.setValue(x/w, y/w, z/w);
        }

        //! Multiplies given row vector by matrix, giving vector result. 
        void multVecMatrix(const Vec3<Type> & src, Vec3<Type> & dst) const
        {
            Type x = src[0]*m_data[0][0] + src[1]*m_data[1][0] + src[2]*m_data[2][0] + m_data[3][0];
            Type y = src[0]*m_data[0][1] + src[1]*m_data[1][1] + src[2]*m_data[2][1] + m_data[3][1];
            Type z = src[0]*m_data[0][2] + src[1]*m_data[1][2] + src[2]*m_data[2][2] + m_data[3][2];
            Type w = src[0]*m_data[0][3] + src[1]*m_data[1][3] + src[2]*m_data[2][3] + m_data[3][3];

            dst.setValue(x/w, y/w, z/w);
        }

        /*!Multiplies given row vector by matrix, giving vector result. 
        src is assumed to be a direction vector, so translation part of matrix is ignored. 
        Note: if you wish to transform surface points and normals by a matrix, call multVecMatrix() for the points and call multDirMatrix() on the inverse transpose of the matrix for the normals.
        */
        void multDirMatrix(const Vec3<Type> & src, Vec3<Type> & dst) const
        {
            dst.setValue(src[0]*m_data[0][0] + src[1]*m_data[1][0] + src[2]*m_data[2][0],
                src[0]*m_data[0][1] + src[1]*m_data[1][1] + src[2]*m_data[2][1],
                src[0]*m_data[0][2] + src[1]*m_data[1][2] + src[2]*m_data[2][2]);
        }

        friend std::ostream & operator<<(std::ostream & os, const Matrix4<Type> & mat)
        { 
            for(unsigned int i=0; i<4; i++){
                os << mat[i][0] <<'\t'<< mat[i][1] <<'\t'<< mat[i][2] <<'\t'<< mat[i][3] <<std::endl;
            }
            return os;
        }
 
		// the following are the codes which i add.
		// Pay attention to that all the transform is the same to the opengl's function
		Matrix4<Type> MakeProjection(const Type& fovy, const Type& aspectRation, const Type& near, const Type& far)
		{
		    Type c  = static_cast< Type > ( 1.0 ) / std::tan( DegToRad(fovy) * 0.5 ); 
			return Matrix4<Type> ( 
				static_cast< Type >(c / aspectRation), static_cast< Type >(0), static_cast< Type >(0), static_cast< Type >(0),     
				static_cast< Type >(0), static_cast< Type >(c), static_cast< Type >(0), static_cast< Type >(0),           
				static_cast< Type >(0), static_cast< Type >(0), static_cast< Type >(((far + near)/(near-far))), static_cast< Type >(-1),                                   
				static_cast< Type >(0), static_cast< Type >(0), static_cast< Type >((2*(far*near)/(near-far))), static_cast< Type >(0)); 
		}
		
		//eye 为眼睛位置，position为看的目标位置，up为照相机的正方向
		Matrix4<Type> MakeView( const gtl::Vec3<Type>& eyes, const gtl::Vec3<Type>& position, const gtl::Vec3<Type>& up)
		{ 
			gtl::Vec3<Type> f = position - eyes;	f.normalize();
			gtl::Vec3<Type> s = f.cross(up);		s.normalize();
			gtl::Vec3<Type> u = s.cross(f); 

			return Matrix4<Type>(
				s.x(),	s.y(),	s.z(),  0.0, 
				u.x(),	u.y(),	u.z(),  0.0,
				-f.x(),	-f.y(),	-f.z(), 0.0,
				-eyes.x(), -eyes.y(), -eyes.z(), 1 ); 
		}
    private:
        Type m_data[4][4];
    };

    typedef Matrix4<int>    Matrix4i;
    typedef Matrix4<float>  Matrix4f;
    typedef Matrix4<double> Matrix4d;
} // namespace gtl

#endif
