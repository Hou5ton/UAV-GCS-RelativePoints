#ifndef CLIPPING_ALGORITHMS_H
#define CLIPPING_ALGORITHMS_H

#include <functional>
#include "KartaInterface.h"

const int LEFT_CODE = 1;
const int RIGHT_CODE = 2;
const int BOTTOM_CODE = 4;
const int TOP_CODE = 8;

class CohenSutherland
{
public:
	// Constructor. Parameter is the boundary rectangle.	
	CohenSutherland(MRect& boundaries)
		: m_Boundaries(boundaries)		
	{
		double y_bottom = m_Boundaries.bottom;
		m_Boundaries.bottom = m_Boundaries.top;
		m_Boundaries.top = y_bottom;
	}

	void Clip(MPtArray *input, QList<MPtArray*>& output)
	{		
		int cutResult;
		MPtArray *clipped = new MPtArray();
		output.append(clipped);	
		for (int i = 0; i < input->GetSize() - 1; i++) 
		{
			MPoint a = input->GetAt(i), b =  input->GetAt(i + 1);
			cutResult = cutSection(a, b);
			if (cutResult == 0)
			{
				if (!clipped->GetSize())
					clipped->AddTail(&a);				
				else
					if (*clipped->GetTail() != a)
					{						
						clipped = new MPtArray();
						output.append(clipped);
						clipped->AddTail(&a);
					}				
					clipped->AddTail(&b);
			}
		}
		
	}

private:

	// вычисление кода точки
	int vcode(MPoint p) 
	{
		return (((p.x < m_Boundaries.left) ? LEFT_CODE : 0)  +  	/* +1 если точка левее прямоугольника */ 
			((p.x > m_Boundaries.right) ? RIGHT_CODE : 0) +  		/* +2 если точка правее прямоугольника */
			((p.y < m_Boundaries.bottom) ? BOTTOM_CODE : 0)   +  		/* +4 если точка ниже прямоугольника */  
			((p.y > m_Boundaries.top) ? TOP_CODE : 0));     		/* +8 если точка выше прямоугольника */
	}

	/* если отрезок ab не пересекает прямоугольник m_Boundaries, функция возвращает -1;
	если отрезок ab пересекает прямоугольник m_Boundaries, функция возвращает 0 и отсекает
	те части отрезка, которые находятся вне прямоугольника */
	int cutSection (MPoint &a, MPoint &b)
	{
		int code_a, code_b, code; /* код конечных точек отрезка */

		MPoint c; /* одна из точек */

		code_a = vcode(a);
		code_b = vcode(b);

		/* пока одна из точек отрезка вне прямоугольника */
		while (code_a | code_b) {
			/* если обе точки с одной стороны прямоугольника, то отрезок не пересекает прямоугольник */
			if (code_a & code_b)
				return -1;

			/* выбираем точку c с ненулевым кодом */
			if (code_a) 
			{
				code = code_a;
				c = a;
			} else 
			{
				code = code_b;
				c = b;
			}

			/* если c левее m_Boundaries, то передвигаем c на прямую x = m_Boundaries.left
			если c правее m_Boundaries, то передвигаем c на прямую x = m_Boundaries.right */
			if (code & LEFT_CODE) 
			{
				c.y += (a.y - b.y) * (m_Boundaries.left - c.x) / (a.x - b.x);
				c.x = m_Boundaries.left;
			} 
			else 
				if (code & RIGHT_CODE) 
				{
					c.y += (a.y - b.y) * (m_Boundaries.right - c.x) / (a.x - b.x);
					c.x = m_Boundaries.right;
				}
				/* если c ниже m_Boundaries, то передвигаем c на прямую y = m_Boundaries.bottom
				если c выше m_Boundaries, то передвигаем c на прямую y = m_Boundaries.top */
				else 
					if (code & BOTTOM_CODE) 
					{
						c.x += (a.x - b.x) * (m_Boundaries.bottom - c.y) / (a.y - b.y);
						c.y = m_Boundaries.bottom;
					} 
					else 
						if (code & TOP_CODE) 
						{
							c.x += (a.x - b.x) * (m_Boundaries.top - c.y) / (a.y - b.y);
							c.y = m_Boundaries.top;
						}

						/* обновляем код */
						if (code == code_a)
						{
							a.x = c.x; a.y = c.y; 
							code_a = vcode(a);
						}
						else 
						{
							b.x = c.x; b.y = c.y;
							code_b = vcode(b);
						}
		}

		/* оба кода равны 0, следовательно обе точки в прямоугольнике */
		return 0;
	}

private:
	MRect m_Boundaries;

};

// SutherlandHodgman
// Class to perform polygon clipping against an upright rectangular boundary window.
// Implementation of Sutherland-Hodgman algorithm (1974).
class SutherlandHodgman
{
public:
	// Constructor. Parameter is the boundary rectangle.
	// SutherlandHodgman expects a 'normalized' boundary rectangle, meaning
	// that boundaries.GetRight() > boundaries.GetLeft() and
	// boundaries.GetBottom() > boundaries.GetTop().
	// In other words: boundary.Width > 0 and boundaries.Height > 0.
	// If this is violated, nothing will be output.
	SutherlandHodgman(MRect& boundaries)
		: m_stageBottom(m_stageOut, boundaries.bottom)		// Initialize each stage
		, m_stageLeft(m_stageBottom, boundaries.left)		// with its next stage and
		, m_stageTop(m_stageLeft, boundaries.top)			// the boundary position.
		, m_stageRight(m_stageTop, boundaries.right)
	{
	}

	void Clip(MPtArray *input, MPtArray *clipped)
	{	
		m_stageOut.SetDestination(clipped);

		// Clip each input vertex.
		for (int i = 0; i < input->GetSize(); i++) 
			m_stageRight.HandleVertex(*input->GetAt(i));

		// Do the final step.
		m_stageRight.Finalize();
	}

private:
	// Implementation of a horizontal boundary (top or bottom).
	// Comp is a std::binary_function object, comparing its two parameters, f.i. std::less.
	template <class Comp>
	class BoundaryHor
	{
	public:
		BoundaryHor(float y): m_Y(y)					{}	
		bool IsInside(const MPoint& pnt) const	{ return Comp()(pnt.y, m_Y); }	// return true if pnt.Y is at the inside of the boundary
		MPoint Intersect(const MPoint& p0, const MPoint& p1) const			// return intersection point of line p0...p1 with boundary
		{																	// assumes p0...p1 is not strictly horizontal
			
			MPoint d = MPoint(p1.x - p0.x,p1.y - p0.y);
			float xslope = (float)d.x/d.y;

			MPoint r;
			r.y = m_Y;
			r.x = p0.x + xslope * (m_Y - p0.y);
			return r;
		}
	private:
		float m_Y;
	};

	// Implementation of a vertical boundary (left or right).
	template <class Comp>
	class BoundaryVert
	{
	public:
		BoundaryVert(float x): m_X(x)				{}
		bool IsInside(const MPoint& pnt) const	{ return Comp()(pnt.x, m_X); }
		MPoint Intersect(const MPoint& p0, const MPoint& p1) const		// assumes p0...p1 is not strictly vertical
		{
			
			MPoint d = MPoint(p1.x - p0.x,p1.y - p0.y) ;
			float yslope = (float)d.y/d.x;

			MPoint r;
			r.x = m_X;
			r.y = p0.y + yslope * (m_X - p0.x);
			return r;
		}
	private:
		float m_X;
	};

	// This template class is the workhorse of the algorithm. It handles the clipping against one boundary.
	// Boundary is either BoundaryHor or BoundaryVert, Stage is the next ClipStage, or the output stage.
	template <class Boundary, class Stage>
	class ClipStage : private Boundary
	{
	public:
		ClipStage(Stage& nextStage, float position)
			: Boundary(position)
			, m_NextStage(nextStage)
			, m_bFirst(true)
		{}
		void HandleVertex( MPoint& pntCurrent)	// Function to handle one vertex
		{
			bool bCurrentInside = Boundary::IsInside(pntCurrent);		// See if vertex is inside the boundary.

			if (m_bFirst)	// If this is the first vertex...
			{
				m_pntFirst = pntCurrent;	// ... just remember it,...

				m_bFirst = false;
			}
			else		// Common cases, not the first vertex.
			{
				if (bCurrentInside)	// If this vertex is inside...
				{
					if (! m_bPreviousInside)
					{		// ... and the previous one was outside
						MPoint pt = Boundary::Intersect(m_pntPrevious, pntCurrent);
						m_NextStage.HandleVertex(pt);
					}
													// ... first output the intersection point.

					m_NextStage.HandleVertex(pntCurrent);	// Output the current vertex.
				}
				else if (m_bPreviousInside)
					{ // If this vertex is outside, and the previous one was inside...
						MPoint pt = Boundary::Intersect(m_pntPrevious, pntCurrent);
						m_NextStage.HandleVertex(pt);
					}
											// ... output the intersection point.

				// If neither current vertex nor the previous one are inside, output nothing.
			}
			m_pntPrevious = pntCurrent;		// Be prepared for next vertex.
			m_bPreviousInside = bCurrentInside;
		}
		void Finalize()
		{
			HandleVertex(m_pntFirst);		// Close the polygon.
			m_NextStage.Finalize();			// Delegate to the next stage.
		}
	private:
		Stage& m_NextStage;			// the next stage
		bool m_bFirst;				// true if no vertices have been handled
		MPoint m_pntFirst;			// the first vertex
		MPoint m_pntPrevious;		// the previous vertex
		bool m_bPreviousInside;		// true if the previous vertex was inside the Boundary
	};

	class OutputStage
	{
	public:
		OutputStage() : m_pDest(0)	{}
		void SetDestination(MPtArray * pDest) { m_pDest = pDest; }
		void HandleVertex(MPoint& pnt)	{ 
			m_pDest->AddTail( &pnt); 
		}	// Append the vertex to the output container.
		void Finalize()	{}		// Do nothing.
	private:
		MPtArray *m_pDest;
	};

	// These typedefs define the four boundaries. In keeping up with the GDI/GDI+ interpretation of
	// rectangles, we include the left and top boundaries, but not the right and bottom boundaries.
	// In other words: a vertex on the left boundary is considered to be inside, but a vertex
	// on the right boundary is considered to be outside.
	typedef BoundaryVert<std::less<float> >				BoundaryRight;
	typedef BoundaryHor<std::greater_equal<float> >		BoundaryTop;
	typedef BoundaryVert<std::greater_equal<float> >		BoundaryLeft;
	typedef BoundaryHor<std::less<float> >				BoundaryBottom;

	// Next typedefs define the four stages. First template parameter is the boundary,
	// second template parameter is the next stage.
	typedef ClipStage<BoundaryBottom, OutputStage>	ClipBottom;
	typedef ClipStage<BoundaryLeft, ClipBottom>		ClipLeft;
	typedef ClipStage<BoundaryTop, ClipLeft>		ClipTop;
	typedef ClipStage<BoundaryRight, ClipTop>		ClipRight;

	// Our data members.
	OutputStage m_stageOut;
	ClipBottom m_stageBottom;
	ClipLeft m_stageLeft;
	ClipTop m_stageTop;
	ClipRight m_stageRight;
};

#endif //CLIPPING_ALGORITHMS_H