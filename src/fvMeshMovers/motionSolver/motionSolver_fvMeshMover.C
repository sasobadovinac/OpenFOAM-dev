/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2021-2024 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "motionSolver_fvMeshMover.H"
#include "motionSolver.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace fvMeshMovers
{
    defineTypeNameAndDebug(motionSolver, 0);
    addToRunTimeSelectionTable(fvMeshMover, motionSolver, fvMesh);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fvMeshMovers::motionSolver::motionSolver
(
    fvMesh& mesh,
    const dictionary& dict
)
:
    fvMeshMover(mesh),
    motionPtr_(Foam::motionSolver::New("motionSolver", mesh, dict)),
    velocityMotionCorrection_(mesh, dict)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::fvMeshMovers::motionSolver::~motionSolver()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::motionSolver& Foam::fvMeshMovers::motionSolver::motion() const
{
    return motionPtr_();
}


bool Foam::fvMeshMovers::motionSolver::solidBody() const
{
    return motion().solidBody();
}


bool Foam::fvMeshMovers::motionSolver::update()
{
    mesh().movePoints(motionPtr_->newPoints());
    velocityMotionCorrection_.update();

    return true;
}


void Foam::fvMeshMovers::motionSolver::topoChange(const polyTopoChangeMap& map)
{
    motionPtr_->topoChange(map);
}


void Foam::fvMeshMovers::motionSolver::mapMesh(const polyMeshMap& map)
{
    motionPtr_->mapMesh(map);
}


void Foam::fvMeshMovers::motionSolver::distribute
(
    const polyDistributionMap& map
)
{
    motionPtr_->distribute(map);
}


bool Foam::fvMeshMovers::motionSolver::write(const bool write) const
{
    if (write)
    {
        return motionPtr_->write();
    }
    else
    {
        return true;
    }
}


// ************************************************************************* //
