/*
-----------------------------------------------------------------------
Copyright: 2010-2022, imec Vision Lab, University of Antwerp
           2014-2022, CWI, Amsterdam

Contact: astra@astra-toolbox.com
Website: http://www.astra-toolbox.com/

This file is part of the ASTRA Toolbox.


The ASTRA Toolbox is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The ASTRA Toolbox is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with the ASTRA Toolbox. If not, see <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------
*/

#include "astra/ProjectionGeometry3DFactory.h"

#include "astra/ParallelProjectionGeometry3D.h"
#include "astra/ParallelVecProjectionGeometry3D.h"
#include "astra/ConeProjectionGeometry3D.h"
#include "astra/ConeVecProjectionGeometry3D.h"
#include "astra/CylConeVecProjectionGeometry3D.h"

#include "astra/Logging.h"

namespace astra
{

_AstraExport std::unique_ptr<CProjectionGeometry3D> constructProjectionGeometry3D(const std::string &type)
{
	CProjectionGeometry3D* pProjGeometry = 0;
	if (type == "parallel3d") {
		pProjGeometry = new CParallelProjectionGeometry3D();
	} else if (type == "parallel3d_vec") {
		pProjGeometry = new CParallelVecProjectionGeometry3D();
	} else if (type == "cone") {
		pProjGeometry = new CConeProjectionGeometry3D();
	} else if (type == "cone_vec") {
		pProjGeometry = new CConeVecProjectionGeometry3D();
	} else if (type == "cyl_cone_vec") {
		pProjGeometry = new CCylConeVecProjectionGeometry3D();
	} else {
		// invalid geometry type
	}

	return std::unique_ptr<CProjectionGeometry3D>(pProjGeometry);
}

}
