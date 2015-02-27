/**
 * @file mymathplot.cpp
 * @brief The own created AddData method of the mathplot library.
 * @author Andreas Ziegler
 */

// Includes
#include <wx/log.h>
#include <mathplot.h>

/**
 * @brief Adds a new x data point to the x-vector.
 * @param x X value
 * @param xs Reference to the vector containing the x values.
 */
void mpFXYVector::AddXData(double x, std::vector<double> &xs){
		// Check if the data vectora are of the same size
	/*
		if (xs.size() != ys.size()) {
				wxLogError(_("wxMathPlot error: X and Y vector are not of the same length!"));
				return;
		}
		*/

		//Delete first point if you need a filo buffer (i dont need it)
		//xs.erase(xs.begin());
		//xy.erase(xy.begin());

		//After a certain number of points implement a FIFO buffer
		//As plotting too many points can cause missing data
		if (x > 300){
				xs.erase(xs.begin());
		}

		//Add new Data points at the end
		xs.push_back(x);


		// Copy the data:
		m_xs = xs;

		// Update internal variables for the bounding box.
		if (xs.size()>0){
				m_minX  = xs[0];
				m_maxX  = xs[0];

				std::vector<double>::const_iterator  it;

				for (it=xs.begin();it!=xs.end();it++){
						if (*it<m_minX) m_minX=*it;
						if (*it>m_maxX) m_maxX=*it;
				}
				m_minX-=0.5f;
				m_maxX+=0.5f;
		}
		else{
				m_minX  = -1;
				m_maxX  = 1;
		}
}

/**
 * @brief Adds a new y data point to the y-vector.
 * @param y Y value
 * @param ys Reference to the vector containing the y values.
 */
void mpFXYVector::AddYData(double y, std::vector<double> &ys){
		// Check if the data vectora are of the same size
	/*
		if (xs.size() != ys.size()) {
				wxLogError(_("wxMathPlot error: X and Y vector are not of the same length!"));
				return;
		}
		*/

		//Delete first point if you need a filo buffer (i dont need it)
		//xs.erase(xs.begin());
		//xy.erase(xy.begin());

		//After a certain number of points implement a FIFO buffer
		//As plotting too many points can cause missing data
		if (y > 300){
				ys.erase(ys.begin());
		}

		//Add new Data points at the end
		ys.push_back(y);


		// Copy the data:
		m_ys = ys;

		// Update internal variables for the bounding box.
		if (ys.size()>0){
				m_minY  = ys[0];
				m_maxY  = ys[0];

				std::vector<double>::const_iterator  it;

				for (it=ys.begin();it!=ys.end();it++){
						if (*it<m_minY) m_minY=*it;
						if (*it>m_maxY) m_maxY=*it;
				}
				m_minY-=0.5f;
				m_maxY+=0.5f;
		}
		else{
				m_minY  = -1;
				m_maxY  = 1;
		}

}
