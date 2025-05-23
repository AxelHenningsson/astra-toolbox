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

#include "astra/CudaProjector2D.h"

#include "astra/Logging.h"

namespace astra
{

//----------------------------------------------------------------------------------------
// Default constructor
CCudaProjector2D::CCudaProjector2D()
{
	_clear();
}

//----------------------------------------------------------------------------------------
// Destructor
CCudaProjector2D::~CCudaProjector2D()
{
	if (m_bIsInitialized) clear();
}

//----------------------------------------------------------------------------------------
// Clear for constructors
void CCudaProjector2D::_clear()
{
	m_pProjectionGeometry = NULL;
	m_pVolumeGeometry = NULL;
	m_bIsInitialized = false;

	m_projectionKernel = ker2d_default;
	m_iVoxelSuperSampling = 1;
	m_iDetectorSuperSampling = 1;
	m_iGPUIndex = -1;
}

//----------------------------------------------------------------------------------------
// Clear
void CCudaProjector2D::clear()
{
	ASTRA_DELETE(m_pProjectionGeometry);
	ASTRA_DELETE(m_pVolumeGeometry);
	m_bIsInitialized = false;
}

//----------------------------------------------------------------------------------------
// Check
bool CCudaProjector2D::_check()
{
	// projection geometry
	ASTRA_CONFIG_CHECK(m_pProjectionGeometry, "CudaProjector2D", "ProjectionGeometry2D not initialized.");
	ASTRA_CONFIG_CHECK(m_pProjectionGeometry->isInitialized(), "CudaProjector2D", "ProjectionGeometry2D not initialized.");

	// volume geometry
	ASTRA_CONFIG_CHECK(m_pVolumeGeometry, "CudaProjector2D", "VolumeGeometry2D not initialized.");
	ASTRA_CONFIG_CHECK(m_pVolumeGeometry->isInitialized(), "CudaProjector2D", "VolumeGeometry2D not initialized.");

	return true;
}

//---------------------------------------------------------------------------------------
// Initialize, use a Config object
bool CCudaProjector2D::initialize(const Config& _cfg)
{
	ConfigReader<CProjector2D> CR("CudaProjector2D", this, _cfg);

	// if already initialized, clear first
	if (m_bIsInitialized) {
		clear();
	}

	// initialization of parent class
	if (!CProjector2D::initialize(_cfg)) {
		return false;
	}

	// TODO: Check the projection geometry is a supported type

	std::string sProjKernel;
	CR.getString("ProjectionKernel", sProjKernel, "default");
	if (sProjKernel == "default") {
		m_projectionKernel = ker2d_default;
	} else {
		ASTRA_ERROR("Unknown ProjectionKernel \"%s\"", sProjKernel.c_str());
		return false;
	}

	bool ok = true;

	ok &= CR.getOptionInt("VoxelSuperSampling", m_iVoxelSuperSampling, 1);
	ok &= CR.getOptionInt("DetectorSuperSampling", m_iDetectorSuperSampling, 1);

	if (CR.hasOption("GPUIndex"))
		ok &= CR.getOptionInt("GPUIndex", m_iGPUIndex, -1);
	else
		ok &= CR.getOptionInt("GPUindex", m_iGPUIndex, -1);
	if (!ok)
		return false;

	m_bIsInitialized = _check();
	return m_bIsInitialized;
}


std::string CCudaProjector2D::description() const
{
	return CCudaProjector2D::type;
}

} // end namespace
