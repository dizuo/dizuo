#pragma once
#include <vector>

namespace PlaybackLib
{

        template <typename T>
        class CBSpline
        {
        public:
                CBSpline(void);
                virtual ~CBSpline(void);

                // Order of curve
                void SetOrder ( int order );
                int GetOrder ( );

                // Add control point
                void AddControlPoint ( T controlPoint );

                // Remove control point
                bool RemoveControlPoint ( T controlPoint );

                // Clear all control points
                void ClearControlPoints ( );

                // Remove first waypoint
                bool RemoveFirstControlPoint ( );

                // Get a vector containing the control points
                std::vector<T>* GetControlPoints ( );

                // Get a point of the curve at P ( v ), v in range 0..GetParameterMax ( )
                void GetPoint ( double v, T* pointOut );

                // Get max value of the b-spline parameter to work out the parameter range: 0..GetParameterMax ( )
                double GetParameterMax ( );

                // Get the curve parameter value at which specified ctrl point has maximum influence
                double GetBlendingFunctionMaximum ( int controlPoint );

                // Create a universal knot vector
                void CreateUniversalKnotVector ( );

                // * Create other kinds of knot vector, e.g. Uniform, Interpolating...

                // Pre-Calculate or if possible load constants for blending function maxima
                void PreProcessBlendingMaxPoints ( );

                // Debug print function
//              void DebugOutput ( );

        protected:

                // Evaluate the k'th blending function at order t at P ( v )
                double EvaluateBlendingFunction ( int k, int t, double v );

                // Look up parameter value that yeilds max point for given blending function / control point
                // Assuming Universal knot vector
                double GetMaxValueUniversal ( int i );

                // Calculate parameter value that yeilds max point for given blending function / control point
                // No assumptions about knot vector
                double GetMaxValueCustom ( int i );

                // Types of knot vector
                typedef enum KnotVectorType { None, Universal, Custom /* ...more */ };

                // Current type of knot vector
                KnotVectorType m_knotVectorType;

                // Order of curve
                int m_order;

                // Control point vector
                std::vector<T>* m_controlPoints;

                // Knot vector
                std::vector<double>* m_knots;

                // Cached max values of blending functions
                std::vector<double>* m_maxBlendingValues;

                bool m_maxValuesValid; // Are the cached values valid
        };
}

using namespace PlaybackLib;

// Create
template <typename T>
CBSpline<T>::CBSpline ( )
{
        m_order= 3;

        // List of control points
        m_controlPoints= new std::vector<T>;

        // List of knots
        m_knots= new std::vector<double>;

        // Cached max values of blending functions
        m_maxBlendingValues= new std::vector<double> ( );

        m_maxValuesValid= false; // Are the cached values valid

        // Current type of knot vector
        m_knotVectorType= None;
}

template <typename T>
CBSpline<T>::~CBSpline ( )
{
        delete m_controlPoints;
        delete m_knots;
        delete m_maxBlendingValues;
}

// Get the curve parameter value at which specified ctrl point has maximum influence
template <typename T>
double CBSpline<T>::GetBlendingFunctionMaximum ( int controlPoint )
{
        if ( !m_maxValuesValid ) PreProcessBlendingMaxPoints ( );

        return m_maxBlendingValues->at ( controlPoint );
}

// Create a universal knot vector
template <typename T>
void CBSpline<T>::CreateUniversalKnotVector (  )
{
        int n= m_controlPoints->size ()-1;
        int t= m_order;

        // Empty knot vector
        m_knots->clear ( );

        // Create universal knots: e.g. 0,0,0,1,2,3,4,4,4
        for ( int j= 0; j < n + t + 1 /* Number of knots required */; j++ )
        {
                if (j < t)
                        m_knots->push_back ( 0 );
                else if (j <= n)
                        m_knots->push_back ( j - t + 1 );
                else //if (j > n)
                        m_knots->push_back ( n - t + 2 );       
        }

        // Update type of knot vector
        m_knotVectorType= Universal;

        m_maxValuesValid= false; // Are the cached values valid
}

// Pre-Calculate or if possible load constants for blending function maxima
template <typename T>
void CBSpline<T>::PreProcessBlendingMaxPoints ( )
{
        int n= m_controlPoints->size ()-1;
        int t= m_order;

        // Check knot vector is valid
        if ( m_knots->size () != n + t + 1 ) return;

        // Switch on type of knot vector
        switch ( m_knotVectorType )
        {
        case Universal:
                for ( int i= 0; i <= n; i++ )
                {
                        m_maxBlendingValues->push_back ( GetMaxValueUniversal ( i ) );
                }
                break;
        case Custom:
                for ( int i= 0; i <= n; i++ )
                {
                        m_maxBlendingValues->push_back ( GetMaxValueCustom ( i ) );
                }
                break;
        }

        m_maxValuesValid= true; // Are the cached values valid
}

// Get max value of the b-spline parameter to work out the parameter range: 0..GetParameterMax ( )
template <typename T>
double CBSpline<T>::GetParameterMax ( )
{
        return m_controlPoints->size ()-1 - m_order + 2;
}

// Look up parameter value that yeilds max point for given blending function / control point
// Assuming Universal knot vector
template <typename T>
double CBSpline<T>::GetMaxValueUniversal ( int i )
{
        switch ( m_order )
        {
        case 1:
                break;
        case 2:
                return (double)i;
                break;
        case 3:
                if ( i == 0 ) return 0.0;
                if ( i == 1 ) return 2.0/3.0;
                if ( i >= 2 && i <= m_controlPoints->size ()-3 ) return 1.5 + (double)( i - 2 );
                if ( i == m_controlPoints->size ()-2 ) return GetParameterMax ( ) - 2.0/3.0;
                else return GetParameterMax ( );
                break;
        default:
                return GetMaxValueCustom ( i );
                break;
        }

        return 0.0;
}

// Calculate parameter value that yeilds max point for given blending function / control point
// No assumptions about knot vector
template <typename T>
double CBSpline<T>::GetMaxValueCustom ( int i )
{
        return 0;
}

// Evaluate the k'th blending at order t function at P ( v )
template <typename T>
double CBSpline<T>::EvaluateBlendingFunction ( int k, int t, double v )
{
   double value;

   if (t == 1) 
   {
      if ( ((m_knots->at ( k ) <= v) && (v < m_knots->at ( k + 1 ))) ||

                   // Special case: If v is at maximum value then make upper bound check inclusive
                   // so that point doesn't jump back to zero at end of function
                   ((v == GetParameterMax ())&&((m_knots->at ( k ) <= v) && (v <= m_knots->at ( k + 1 )))) )
          {
         value = 1;
          }
          else
          {
         value = 0;
          }
   } 
   else
   {
      // Protect against div by 0 in both terms
      if ((m_knots->at ( k + t -1 ) == m_knots->at ( k )) && (m_knots->at ( k + t ) == m_knots->at ( k + 1 )))
          {
         value = 0;
          }
      else if (m_knots->at ( k + t -1 ) == m_knots->at ( k ))  // Protect against div by 0 in first term
          {
         value = (m_knots->at ( k + t ) - v) / (m_knots->at ( k + t ) - m_knots->at ( k + 1 )) 
                 * EvaluateBlendingFunction(k+1,t-1,v);
          }
      else if (m_knots->at ( k + t ) == m_knots->at ( k + 1 )) // Protect against div by 0 in second term
          {
         value = (v - m_knots->at ( k )) / (m_knots->at ( k + t -1 ) - m_knots->at ( k ))
                 * EvaluateBlendingFunction(k,t-1,v);
          }
          else
           {
             // No zero's, use full equation
         value = (v - m_knots->at ( k )) / (m_knots->at ( k + t -1 ) - m_knots->at ( k ))
                 * EvaluateBlendingFunction(k,t-1,v) + 
                 (m_knots->at ( k + t ) - v) / (m_knots->at ( k + t ) - m_knots->at ( k + 1 ))
                                 * EvaluateBlendingFunction(k+1,t-1,v);
            }
   }

   return value;
}

// Order of curve
template <typename T>
void CBSpline<T>::SetOrder ( int order )
{
        m_order= order;
}

template <typename T>
int CBSpline<T>::GetOrder ( )
{
        return m_order;
}

// Add control point
template <typename T>
void CBSpline<T>::AddControlPoint ( T controlPoint )
{
        m_controlPoints->push_back ( controlPoint );
}

// Remove control point
template <typename T>
bool CBSpline<T>::RemoveControlPoint ( T controlPoint )
{
        for ( int i= 0; i < m_controlPoints->size (); i++ )
        {
                if ( m_controlPoints->at ( i ) == controlPoint )
                {
                        m_controlPoint.erase ( m_controlPoint.begin () + i );
                        return true;
                }
        }

        return false;
}

// Clear all control points
template <typename T>
void CBSpline<T>::ClearControlPoints ( )
{
        m_controlPoints->clear ();
}

// Remove first waypoint
template <typename T>
bool CBSpline<T>::RemoveFirstControlPoint ( )
{
        if ( m_controlPoints->size () > 0 )
        {
                m_controlPoints->erase ( m_controlPoints->begin () );
                return true;
        }
        else
        {
                return false;
        }
}

// Get a vector containing the control points
template <typename T>
std::vector<T>* CBSpline<T>::GetControlPoints ( )
{
        return m_controlPoints;
}

// Get a point of the curve at P ( v )
template <typename T>
void CBSpline<T>::GetPoint ( double v, T* pointOut )
{
        int i;
        int n= m_controlPoints->size ()-1;
        double b;
        
        // Zero
        (*pointOut)= 0;

        for ( i= 0; i <= n; i++ )
        {
                b= EvaluateBlendingFunction ( i, m_order, v );

                (*pointOut)= (*pointOut) + ( m_controlPoints->at ( i ) * b );
        }
}

// Debug print function
//template <typename T>
//void CBSpline<T>::DebugOutput ( )
//{
//      OutputDebugString ( "B-Spline Control Points:\n" );
//
//      for ( int i= 0; i < m_controlPoints->size (); i++ )
//      {
//              OutputDebugString ( m_controlPoints->at(i).ToString () );
//      }
//
//      static const double gap= 0.01;
//      double max= m_controlPoints->size ()-1 - m_order + 2;
//
//      OutputDebugString ( "B-Spline Curve:\n" );
//
//      for ( double i= 0; i <= max; i+= gap )
//      {
//              T point;
//              GetPoint ( i, &point );
//
//              OutputDebugString ( point.ToString () );
//      }
//}

