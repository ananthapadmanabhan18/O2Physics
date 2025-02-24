// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// \file LFF1Tables.h
///
/// \author Sourav Kundu <sourav.kundu@cern.ch>

#ifndef PWGLF_DATAMODEL_REDUCEDF1PROTONTABLES_H_
#define PWGLF_DATAMODEL_REDUCEDF1PROTONTABLES_H_

#include <cmath>

#include "Common/DataModel/Centrality.h"
#include "Common/DataModel/Multiplicity.h"
#include "Common/DataModel/PIDResponse.h"
#include "Common/Core/RecoDecay.h"
#include "Common/DataModel/TrackSelectionTables.h"
#include "Framework/AnalysisDataModel.h"
#include "Framework/ASoA.h"

namespace o2::aod
{
namespace redf1pevent
{
DECLARE_SOA_COLUMN(Spherocity, spherocity, float); //! Spherocity of the event
} // namespace redf1pevent
DECLARE_SOA_TABLE(RedF1PEvents, "AOD", "REDF1PEVENT",
                  o2::soa::Index<>,
                  bc::GlobalBC,
                  bc::RunNumber,
                  timestamp::Timestamp,
                  collision::PosZ,
                  collision::NumContrib,
                  redf1pevent::Spherocity);
using RedF1PEvent = RedF1PEvents::iterator;

namespace f1protondaughter
{
DECLARE_SOA_INDEX_COLUMN(RedF1PEvent, redF1PEvent);
DECLARE_SOA_COLUMN(F1SignalStat, f1SignalStat, int);                       //! F1 Px
DECLARE_SOA_COLUMN(F1Px, f1Px, float);                                     //! F1 Px
DECLARE_SOA_COLUMN(F1Py, f1Py, float);                                     //! F1 Py
DECLARE_SOA_COLUMN(F1Pz, f1Pz, float);                                     //! F1 Pz
DECLARE_SOA_COLUMN(F1d1Px, f1d1Px, float);                                 //! F1 d1 Px
DECLARE_SOA_COLUMN(F1d1Py, f1d1Py, float);                                 //! F1 d1 Py
DECLARE_SOA_COLUMN(F1d1Pz, f1d1Pz, float);                                 //! F1 d1 Pz
DECLARE_SOA_COLUMN(F1d2Px, f1d2Px, float);                                 //! F1 d2 Px
DECLARE_SOA_COLUMN(F1d2Py, f1d2Py, float);                                 //! F1 d2 Py
DECLARE_SOA_COLUMN(F1d2Pz, f1d2Pz, float);                                 //! F1 d2 Pz
DECLARE_SOA_COLUMN(F1d3Px, f1d3Px, float);                                 //! F1 d3 Px
DECLARE_SOA_COLUMN(F1d3Py, f1d3Py, float);                                 //! F1 d3 Py
DECLARE_SOA_COLUMN(F1d3Pz, f1d3Pz, float);                                 //! F1 d3 Pz
DECLARE_SOA_COLUMN(F1d1TOFHit, f1d1TOFHit, int);                           //! TOF hit pion
DECLARE_SOA_COLUMN(F1d2TOFHit, f1d2TOFHit, int);                           //! TOF hit pion
DECLARE_SOA_COLUMN(F1d1TPC, f1d1TPC, float);                               //! TPC nsigma pion
DECLARE_SOA_COLUMN(F1d2TPC, f1d2TPC, float);                               //! TPC nsigma kaon
DECLARE_SOA_COLUMN(F1d2TPCPionHypo, f1d2TPCPionHypo, float);               //! TPC nsigma kaon
DECLARE_SOA_COLUMN(F1Mass, f1Mass, float);                                 //! F1 mass
DECLARE_SOA_COLUMN(F1MassKaonKshort, f1MassKaonKshort, float);             //! F1 mass kaon kshort
DECLARE_SOA_COLUMN(F1PionIndex, f1PionIndex, int64_t);                     //! F1 pion index
DECLARE_SOA_COLUMN(F1KaonIndex, f1KaonIndex, int64_t);                     //! F1 kaon index
DECLARE_SOA_COLUMN(F1KshortPositiveIndex, f1KshortPositiveIndex, int64_t); //! F1 kshort pion positive index
DECLARE_SOA_COLUMN(F1KshortNegativeIndex, f1KshortNegativeIndex, int64_t); //! F1 kshort pion negative index
DECLARE_SOA_COLUMN(ProtonCharge, protonCharge, float);                     //! Proton charge
DECLARE_SOA_COLUMN(ProtonPx, protonPx, float);                             //! Proton Px
DECLARE_SOA_COLUMN(ProtonPy, protonPy, float);                             //! Proton Py
DECLARE_SOA_COLUMN(ProtonPz, protonPz, float);                             //! Proton Pz
DECLARE_SOA_COLUMN(ProtonNsigmaTPC, protonNsigmaTPC, float);               //! Proton TPC nsigma
DECLARE_SOA_COLUMN(ProtonTOFHit, protonTOFHit, int);                       //! Proton TOF Hit
DECLARE_SOA_COLUMN(ProtonNsigmaTOF, protonNsigmaTOF, float);               //! Proton TOF nsigma
DECLARE_SOA_COLUMN(F1ProtonIndex, f1ProtonIndex, int64_t);                 //! F1 proton index
} // namespace f1protondaughter
DECLARE_SOA_TABLE(F1Tracks, "AOD", "F1TRACK",
                  o2::soa::Index<>,
                  f1protondaughter::RedF1PEventId,
                  f1protondaughter::F1SignalStat,
                  f1protondaughter::F1Px,
                  f1protondaughter::F1Py,
                  f1protondaughter::F1Pz,
                  f1protondaughter::F1d1Px,
                  f1protondaughter::F1d1Py,
                  f1protondaughter::F1d1Pz,
                  f1protondaughter::F1d2Px,
                  f1protondaughter::F1d2Py,
                  f1protondaughter::F1d2Pz,
                  f1protondaughter::F1d3Px,
                  f1protondaughter::F1d3Py,
                  f1protondaughter::F1d3Pz,
                  f1protondaughter::F1d1TOFHit,
                  f1protondaughter::F1d2TOFHit,
                  f1protondaughter::F1d1TPC,
                  f1protondaughter::F1d2TPC,
                  f1protondaughter::F1d2TPCPionHypo,
                  f1protondaughter::F1Mass,
                  f1protondaughter::F1MassKaonKshort,
                  f1protondaughter::F1PionIndex,
                  f1protondaughter::F1KaonIndex,
                  f1protondaughter::F1KshortPositiveIndex,
                  f1protondaughter::F1KshortNegativeIndex);
using F1Track = F1Tracks::iterator;

DECLARE_SOA_TABLE(ProtonTracks, "AOD", "PROTONTRACK",
                  o2::soa::Index<>,
                  f1protondaughter::RedF1PEventId,
                  f1protondaughter::ProtonCharge,
                  f1protondaughter::ProtonPx,
                  f1protondaughter::ProtonPy,
                  f1protondaughter::ProtonPz,
                  f1protondaughter::ProtonNsigmaTPC,
                  f1protondaughter::ProtonTOFHit,
                  f1protondaughter::ProtonNsigmaTOF,
                  f1protondaughter::F1ProtonIndex);
using ProtonTrack = ProtonTracks::iterator;
} // namespace o2::aod
#endif // PWGLF_DATAMODEL_REDUCEDF1PROTONTABLES_H_
