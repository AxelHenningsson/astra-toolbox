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

#ifdef ASTRA_CUDA

#include "astra/CudaDartMaskAlgorithm3D.h"

#include "astra/cuda/3d/darthelper3d.h"
#include "astra/cuda/3d/dims3d.h"

#include "astra/AstraObjectManager.h"
#include "astra/VolumeGeometry3D.h"

#include "astra/Logging.h"

using namespace std;

namespace astra {

//----------------------------------------------------------------------------------------
// Constructor
CCudaDartMaskAlgorithm3D::CCudaDartMaskAlgorithm3D() 
{
	m_bIsInitialized = false;
}

//----------------------------------------------------------------------------------------
// Destructor
CCudaDartMaskAlgorithm3D::~CCudaDartMaskAlgorithm3D() 
{

}

//---------------------------------------------------------------------------------------
// Initialize - Config
bool CCudaDartMaskAlgorithm3D::initialize(const Config& _cfg)
{
	ConfigReader<CAlgorithm> CR("CudaDartMaskAlgorithm3D", this, _cfg);

	bool ok = true;
	int id = -1;

	ok &= CR.getRequiredID("SegmentationDataId", id);
	m_pSegmentation = dynamic_cast<CFloat32VolumeData3D*>(CData3DManager::getSingleton().get(id));

	ok &= CR.getRequiredID("MaskDataId", id);
	m_pMask = dynamic_cast<CFloat32VolumeData3D*>(CData3DManager::getSingleton().get(id));

	if (CR.hasOption("GPUIndex"))
		ok &= CR.getOptionInt("GPUIndex", m_iGPUIndex, -1);
	else
		ok &= CR.getOptionInt("GPUindex", m_iGPUIndex, -1);

	ok &= CR.getOptionUInt("Connectivity", m_iConn, 26);
	ok &= CR.getOptionUInt("Threshold", m_iThreshold, 1);
	ok &= CR.getOptionUInt("Radius", m_iRadius, 1);

	if (!ok)
		return false;

	_check();

	if (!m_bIsInitialized)
		return false;

	return true;
}

//---------------------------------------------------------------------------------------
// Initialize - C++
//bool CCudaDartMaskAlgorithm3D::initialize(CFloat32VolumeData2D* _pSegmentation, int _iConn)
//{
//	return false;
//}

//----------------------------------------------------------------------------------------
// Iterate
bool CCudaDartMaskAlgorithm3D::run(int _iNrIterations)
{
	// check initialized
	ASTRA_ASSERT(m_bIsInitialized);

	const CVolumeGeometry3D& volgeom = m_pSegmentation->getGeometry();
	astraCUDA3d::SDimensions3D dims;
	dims.iVolX = volgeom.getGridColCount();
	dims.iVolY = volgeom.getGridRowCount();
	dims.iVolZ = volgeom.getGridSliceCount();

	astraCUDA3d::setGPUIndex(m_iGPUIndex);
	astraCUDA3d::dartMasking(m_pMask->getFloat32Memory(), m_pSegmentation->getFloat32Memory(), m_iConn, m_iRadius, m_iThreshold, dims);

	return true;
}

//----------------------------------------------------------------------------------------
// Check
bool CCudaDartMaskAlgorithm3D::_check() 
{
	ASTRA_CONFIG_CHECK(m_pMask->isFloat32Memory(), "CudaDartMask3D", "Mask data object must be float32/memory");
	ASTRA_CONFIG_CHECK(m_pSegmentation->isFloat32Memory(), "CudaDartMask3D", "Segmentation data object must be float32/memory");

	// connectivity: 6 or 26
	ASTRA_CONFIG_CHECK(m_iConn == 6 || m_iConn == 26, "CudaDartMask3D", "Connectivity must be 6 or 26");

	// gpuindex >= 0 


	// success
	m_bIsInitialized = true;
	return true;
}

} // namespace astra

#endif // ASTRA_CUDA
