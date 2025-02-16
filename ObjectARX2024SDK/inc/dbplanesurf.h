//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2023 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
//
//
// DESCRIPTION:
//
// The AcDbPlaneSurface class is the interface class for representing
// ASM plane surfaces.  

#pragma once
#ifndef DBPLANESURF_H
#define DBPLANESURF_H

#include "dbsurf.h"

#pragma pack(push, 8)


class AcDbPlaneSurface: public AcDbSurface
{
public:
    AcDbPlaneSurface ();
    ~AcDbPlaneSurface();
    ACDB_DECLARE_MEMBERS(AcDbPlaneSurface);

    // Create plane surface from region.
    virtual Acad::ErrorStatus createFromRegion ( AcDbRegion *pRegion );

    // AcDbObject methods
    Acad::ErrorStatus dwgInFields(AcDbDwgFiler* filer) override;
    Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* filer) const override;
    Acad::ErrorStatus dxfInFields(AcDbDxfFiler* filer) override;
    Acad::ErrorStatus dxfOutFields(AcDbDxfFiler* filer) const override;
    virtual bool isDependent() const;

protected:
    
    Acad::ErrorStatus subGetClassID(CLSID* pClsid) const override;
};
#pragma pack(pop)

#endif // DBPLANESURF_H
