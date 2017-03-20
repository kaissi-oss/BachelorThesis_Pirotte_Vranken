//==============================================================================
/*
Software License Agreement (BSD License)
Copyright (c) 2003-2016, CHAI3D.
(www.chai3d.org)

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following
disclaimer in the documentation and/or other materials provided
with the distribution.

* Neither the name of CHAI3D nor the names of its contributors may
be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

\author    Niels Pirotte
\author    Casper Vranken
\version   1.0.0
*/
//==============================================================================

//+++++++++++++UHAS IMPLEMENTED+++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
#ifndef VoxelH
#define VoxelH
//------------------------------------------------------------------------------
#include "math/CMaths.h"
#include "math/CVector3d.h"
#include "collisions/Triangle.h"
//------------------------------------------------------------------------------
#include <vector>
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace chai3d {
	//------------------------------------------------------------------------------

	//==============================================================================
	/*!
	\file       Voxel.h

	\brief
	Implements a voxel point in the world used in combination with the voxalizer to
	implement the algorithm described in the voxalizer class.
	*/
	//==============================================================================

	//==============================================================================
	/*!
	\class      Voxel
	\ingroup    collisions

	*/
	//==============================================================================
	class Voxel
	{
		//--------------------------------------------------------------------------
		// CONSTRUCTOR & DESTRUCTOR:
		//--------------------------------------------------------------------------

	public:

		//! Constructor of Voxel
		Voxel();

		//! Constructor of Voxel
		Voxel(const double &x, const double &y, const double &z);

		//! Destructor of Voxel
		~Voxel();


		//--------------------------------------------------------------------------
		// METHODS:
		//--------------------------------------------------------------------------

	public:
		//getters
		inline cVector3d* getPos() { return pos; };
		inline float getMinDist() { return minDist; };

		//setters
		void setPos(const double &x, const double &y, const double &z);
		void setMinDist(float dist);
		inline void setTriangle(Triangle* setT) { t = setT; }

	public:
		//--------------------------------------------------------------------------
		// MEMBERS:
		//--------------------------------------------------------------------------
		cVector3d* pos;
		float minDist;
		Triangle* t;

	public:
		inline bool operator <(const Voxel* voxel1) {
			return this->minDist > voxel1->minDist;
		}
	};
	//------------------------------------------------------------------------------
}	  // namespace chai3d
	  //------------------------------------------------------------------------------

	  //------------------------------------------------------------------------------
#endif
	  //------------------------------------------------------------------------------