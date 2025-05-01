/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2019 OpenCFD Ltd.
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

#include "windkesselBC.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "mappedPatchBase.H"
#include "surfaceFields.H"
#include"backwardDdtScheme.H"
#include "word.H"
// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Default initializer constructor 
Foam::windkesselBC::
windkesselBC
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(p, iF),  // Call base class constructor

    // Initialize with default values 
    Rp_(1),
    Rd_(1), 
    C_(1), 
    Pd_(0),
    rho_(1060), 

    Pooo_(0),  // Initialize pressures to zero
    Poo_(0), 
    Po_(0), 
    Pn_(0), 

    Qooo_(0),  // Initialize flow rates to zero 
    Qoo_(0), 
    Qo_(0), 
    Qn_(0),

    currTimeIndex_(-1)

{}

// Mapping Constructor: copy and map existing field onto a new patch  
Foam::windkesselBC::
windkesselBC
(
    const windkesselBC& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchScalarField(ptf, p, iF, mapper), // Base class 

    // Copy all parameters defined in default constructor 
    Rp_(ptf.Rp_),
    Rd_(ptf.Rd_), 
    C_(ptf.C_), 
    Pd_(ptf.Pd_),
    rho_(ptf.rho_), 

    Pooo_(ptf.Pooo_),  
    Poo_(ptf.Poo_), 
    Po_(ptf.Po_), 
    Pn_(ptf.Pn_), 

    Qooo_(ptf.Qooo_),  
    Qoo_(ptf.Qoo_), 
    Qo_(ptf.Qo_), 
    Qn_(ptf.Qn_), 
 
    currTimeIndex_(ptf.currTimeIndex_)
{}

// Dictionary constructor: initialize from dictionary
Foam::windkesselBC::
windkesselBC
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchScalarField(p, iF, dict),

    Rp_(dict.lookupOrDefault<scalar>("Rp", 1)), 
    Rd_(dict.lookupOrDefault<scalar>("Rd", 1)), 
    C_(dict.lookupOrDefault<scalar>("C", 1)), 
    Pd_(dict.lookupOrDefault<scalar>("Pd", 0)), 
    rho_(dict.lookupOrDefault<scalar>("rho", 1060)),  // sjekk om det er riktig å sette denne til 1060 kg/m^3
    Pooo_(dict.lookupOrDefault<scalar>("Pooo", 0)), 
    Poo_(dict.lookupOrDefault<scalar>("Poo", 0)), 
    Po_(dict.lookupOrDefault<scalar>("Po", 0)), 
    Pn_(dict.lookupOrDefault<scalar>("Pn", 0)), 
    Qooo_(dict.lookupOrDefault<scalar>("Qooo", 0)), 
    Qoo_(dict.lookupOrDefault<scalar>("Qoo", 0)), 
    Qo_(dict.lookupOrDefault<scalar>("Qo", 0)), 
    Qn_(dict.lookupOrDefault<scalar>("Qn", 0)), 

    currTimeIndex_(-1)
{
    Info << "Reading boundary condition from: " << dict << endl;
    Info << "Read values -> Rp = " << Rp_ << ", Rd = " << Rd_ << ", C = " << C_ << endl;

}

// Copy constructor: Create duplicate field 
Foam::windkesselBC::
windkesselBC
(
    const windkesselBC& tppsf
)
:

    fixedValueFvPatchScalarField(tppsf),
    Rp_(tppsf.Rp_),
    Rd_(tppsf.Rd_), 
    C_(tppsf.C_), 
    Pd_(tppsf.Pd_),
    rho_(tppsf.rho_), 

    Pooo_(tppsf.Pooo_),  
    Poo_(tppsf.Poo_), 
    Po_(tppsf.Po_), 
    Pn_(tppsf.Pn_), 

    Qooo_(tppsf.Qooo_),  
    Qoo_(tppsf.Qoo_), 
    Qo_(tppsf.Qo_), 
    Qn_(tppsf.Qn_), 

    currTimeIndex_(tppsf.currTimeIndex_)
{}

// Copy constructor with new internalField
Foam::windkesselBC::
windkesselBC
(
    const windkesselBC& tppsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(tppsf, iF),
    Rp_(tppsf.Rp_),
    Rd_(tppsf.Rd_), 
    C_(tppsf.C_), 
    Pd_(tppsf.Pd_),
    rho_(tppsf.rho_), 

    Pooo_(tppsf.Pooo_),  
    Poo_(tppsf.Poo_), 
    Po_(tppsf.Po_), 
    Pn_(tppsf.Pn_), 

    Qooo_(tppsf.Qooo_),  
    Qoo_(tppsf.Qoo_), 
    Qo_(tppsf.Qo_), 
    Qn_(tppsf.Qn_), 

    currTimeIndex_(tppsf.currTimeIndex_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

/*
//Mapping function for mesh refinement

void Foam::windkesselBC::autoMap
(
    const fvPatchFieldMapper& mapper
)
{
    fixedValueFvPatchScalarField::autoMap(mapper);

}


//Mapping function for mesh coarsening
// for parallel simulations 

void Foam::windkesselBC::rmap
(
    const fvPatchScalarField& ptf,
    const labelList& addr
)
{
    // Call base class function
    fixedValueFvPatchScalarField::rmap(ptf, addr);

    // Cast to Windkessel boundary condition type
    const windkesselBC& wkptf = refCast<const windkesselBC>(ptf);

    // Directly assign pressure history variables 
    Pooo_ = wkptf.Pooo_;
    Poo_ = wkptf.Poo_;
    Po_ = wkptf.Po_;
    Pn_ = wkptf.Pn_;

    // Directly assign flow rate history variables 
    Qooo_ = wkptf.Qooo_;
    Qoo_ = wkptf.Qoo_;
    Qo_ = wkptf.Qo_;
    Qn_ = wkptf.Qn_;

    // Assign scalar parameters (Rp_, Rd_, C_) directly
    Rp_ = wkptf.Rp_;
    Rd_ = wkptf.Rd_;
    C_ = wkptf.C_;
}
*/



void Foam::windkesselBC::updateCoeffs()
{
    //- Check if coefficients have already been updated for this time step
    if (updated())
    {
        return;
    }


    // Retrieve the time step size from database 
    const scalar dt(db().time().deltaTValue());

    // Retrieve the flux field (phi) from database 

    const surfaceScalarField& phi = db().lookupObject<surfaceScalarField>("phi");

    // Retrieve boundary pressure field from the database 
    const fvPatchField<scalar>& p(db().lookupObject<volScalarField>("p").boundaryField()[this->patch().index()]);


    // Calculate the total area of the patch 
    scalar area = gSum(patch().magSf()); // Sum all face areas (patch().magSf) on the boundary patch

    if (area < SMALL) {
        FatalErrorIn("windkesselBC::updateCoeffs") << "Patch area is zero or too small." << abort(FatalError);
    } 

    if (db().time().timeIndex() != currTimeIndex_)
    {
        currTimeIndex_ = db().time().timeIndex();
        
        // Update pressure history variables 
        Pooo_ = Poo_;
        Poo_ = Po_;
        Po_ = Pn_;
        Pn_=gSum(p * patch().magSf()) / area; // Compute mean pressure over the patch 

        // Update flow rate history variables 
        Qooo_ = Qoo_;
        Qoo_ = Qo_;
        Qo_ = Qn_;
        Qn_= gSum(phi.boundaryField()[patch().index()]); // Compute total flux
    }
    
    // Calculate new pressure using backward differencing scheme 
    if
    (
        db().time().timeIndex() > 1 // Perform calculations only if the time index is greater than 1
    )
    {

        Pn_= Rp_ * Rd_ * C_ * (( 3*Qn_ - 4*Qo_ + Qoo_)/(2*dt))
        + (Rp_ +Rd_)*Qn_ 
        + Pd_ 
        - (Rd_ * C_)* ((Poo_ - 4*Po_)/(2*dt));

        Pn_ /=(1 + (3 * Rd_ * C_)/(2 * dt)) + SMALL  ;  // SMALL: OF macro, preventing division of zero
    }

    // Apply implicit pressure update to the boundary field 
    operator== (Pn_ / (rho_ + SMALL) ); //Fjernet  rho_ + SMALL

    // Call base class function to finalize update 
    fixedValueFvPatchScalarField::updateCoeffs();
}



void Foam::windkesselBC::write(Ostream& os) const
{
    fvPatchField<scalar>::write(os);

    Info << "Writing boundary condition values to log:" << nl
         << "Pn = " << Pn_ << nl
         << "Qn = " << Qn_ << nl
         << "Rp = " << Rp_ << nl
         << "Rd = " << Rd_ << nl
         << "C = " << C_ << nl
         << endl;

    os.writeEntry("Pn", Pn_);
    os.writeEntry("Qn", Qn_);
    os.writeEntry("Rp", Rp_);
    os.writeEntry("Rd", Rd_);
    os.writeEntry("C", C_);
     //- Write flow history parameters

    os.writeEntry("Qooo", Qooo_); // Third past flow rate

    os.writeEntry("Qoo", Qoo_); // Second past flow rate

    os.writeEntry("Qo", Qo_); // Previous flow rate

    os.writeEntry("Qn", Qn_); // Current flow rate



    //- Write pressure history parameters

    os.writeEntry("Pooo",Pooo_); // Third past pressure

    os.writeEntry("Poo",Poo_);

    os.writeEntry("Po",Po_);

    os.writeEntry("Pn",Pn_);



    //- Write the boundary field value entry to the output stream

    writeEntry("value", os);
    fvPatchField<scalar>::writeValueEntry(os);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        windkesselBC
    );
}

// ************************************************************************* //
