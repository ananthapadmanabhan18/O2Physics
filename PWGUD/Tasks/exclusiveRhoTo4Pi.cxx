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

/// \file exclusiveRhoTo4Pi.cxx
/// \brief Task for analyzing exclusive rho decays to 4 pions
/// \author Anantha Padmanabhan M Nair

#include <cstdlib>
#include <vector>
#include <cmath>
#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"
#include "Framework/AnalysisDataModel.h"
#include "Framework/ASoA.h"
#include "Framework/ASoAHelpers.h"
#include "PWGUD/DataModel/UDTables.h"
#include "PWGUD/Core/SGSelector.h"
#include "PWGUD/Core/UDHelpers.h"
#include "PWGUD/Core/SGTrackSelector.h"
#include "Common/DataModel/PIDResponse.h"
#include <TString.h>
#include "TLorentzVector.h"
#include <TMath.h>
#include "Math/Vector4D.h"
#include "Math/Vector3D.h"
#include "Math/GenVector/Boost.h"
#include "CommonConstants/PhysicsConstants.h"

using namespace std;
using namespace o2;
using namespace o2::aod;
using namespace o2::framework;
using namespace o2::framework::expressions;

namespace o2::aod
{
namespace branch
{

DECLARE_SOA_COLUMN(PosX, posX, double);
DECLARE_SOA_COLUMN(PosY, posY, double);
DECLARE_SOA_COLUMN(PosZ, posZ, double);

DECLARE_SOA_COLUMN(Fv0signal, fv0signal, double);
DECLARE_SOA_COLUMN(Ft0asignal, ft0asignal, double);
DECLARE_SOA_COLUMN(Ft0csignal, ft0csignal, double);
DECLARE_SOA_COLUMN(Fddasignal, fddasignal, double);
DECLARE_SOA_COLUMN(Fddcsignal, fddcsignal, double);

DECLARE_SOA_COLUMN(TimeFv0, timeFv0, double);
DECLARE_SOA_COLUMN(TimeFt0a, timeFt0a, double);
DECLARE_SOA_COLUMN(TimeFt0c, timeFt0c, double);
DECLARE_SOA_COLUMN(TimeFdda, timeFdda, double);
DECLARE_SOA_COLUMN(TimeFddc, timeFddc, double);

DECLARE_SOA_COLUMN(TimeZna, timeZna, double);
DECLARE_SOA_COLUMN(TimeZnc, timeZnc, double);

DECLARE_SOA_COLUMN(Dcaxy, dcaxy, std::vector<double>);
DECLARE_SOA_COLUMN(Dcaz, dcaz, std::vector<double>);

DECLARE_SOA_COLUMN(TpcNsigmaPi, tpcNsigmaPi, std::vector<double>);
DECLARE_SOA_COLUMN(TpcNsigmaKa, tpcNsigmaKa, std::vector<double>);
DECLARE_SOA_COLUMN(TpcNsigmaPr, tpcNsigmaPr, std::vector<double>);
DECLARE_SOA_COLUMN(TpcNsigmaEl, tpcNsigmaEl, std::vector<double>);
DECLARE_SOA_COLUMN(TpcNsigmaMu, tpcNsigmaMu, std::vector<double>);

DECLARE_SOA_COLUMN(TofNsigmaPi, tofNsigmaPi, std::vector<double>);
DECLARE_SOA_COLUMN(TofNsigmaKa, tofNsigmaKa, std::vector<double>);
DECLARE_SOA_COLUMN(TofNsigmaPr, tofNsigmaPr, std::vector<double>);
DECLARE_SOA_COLUMN(TofNsigmaEl, tofNsigmaEl, std::vector<double>);
DECLARE_SOA_COLUMN(TofNsigmaMu, tofNsigmaMu, std::vector<double>);

DECLARE_SOA_COLUMN(TpcChi2, tpcChi2, std::vector<double>);
DECLARE_SOA_COLUMN(TpcNClsFindable, tpcNClsFindable, std::vector<double>);
DECLARE_SOA_COLUMN(ItsChi2, itsChi2, std::vector<double>);

DECLARE_SOA_COLUMN(PionPt, pionPt, std::vector<double>);
DECLARE_SOA_COLUMN(PionEta, pionEta, std::vector<double>);
DECLARE_SOA_COLUMN(PionPhi, pionPhi, std::vector<double>);
DECLARE_SOA_COLUMN(PionRapidity, pionRapidity, std::vector<double>);

DECLARE_SOA_COLUMN(FourPionPt, fourPionPt, double);
DECLARE_SOA_COLUMN(FourPionEta, fourPionEta, double);
DECLARE_SOA_COLUMN(FourPionPhi, fourPionPhi, double);
DECLARE_SOA_COLUMN(FourPionRapidity, fourPionRapidity, double);

DECLARE_SOA_COLUMN(FourPionMass, fourPionMass, double);
DECLARE_SOA_COLUMN(FourPionPhiPair1, fourPionPhiPair1, double);
DECLARE_SOA_COLUMN(FourPionPhiPair2, fourPionPhiPair2, double);
DECLARE_SOA_COLUMN(FourPionCosThetaPair1, fourPionCosThetaPair1, double);
DECLARE_SOA_COLUMN(FourPionCosThetaPair2, fourPionCosThetaPair2, double);
} // namespace branch

DECLARE_SOA_TABLE(SignalData, "AOD", "signalData",
                  branch::PosX,
                  branch::PosY,
                  branch::PosZ,
                  branch::Fv0signal,
                  branch::Ft0asignal,
                  branch::Ft0csignal,
                  branch::Fddasignal,
                  branch::Fddcsignal,
                  branch::TimeFv0,
                  branch::TimeFt0a,
                  branch::TimeFt0c,
                  branch::TimeFdda,
                  branch::TimeFddc,
                  branch::TimeZna,
                  branch::TimeZnc,
                  branch::Dcaxy,
                  branch::Dcaz,
                  branch::TpcNsigmaPi,
                  branch::TpcNsigmaKa,
                  branch::TpcNsigmaPr,
                  branch::TpcNsigmaEl,
                  branch::TpcNsigmaMu,
                  branch::TofNsigmaPi,
                  branch::TofNsigmaKa,
                  branch::TofNsigmaPr,
                  branch::TofNsigmaEl,
                  branch::TofNsigmaMu,
                  branch::TpcChi2,
                  branch::TpcNClsFindable,
                  branch::ItsChi2,
                  branch::PionPt,
                  branch::PionEta,
                  branch::PionPhi,
                  branch::PionRapidity,
                  branch::FourPionPt,
                  branch::FourPionEta,
                  branch::FourPionPhi,
                  branch::FourPionRapidity,
                  branch::FourPionMass,
                  branch::FourPionPhiPair1,
                  branch::FourPionPhiPair2,
                  branch::FourPionCosThetaPair1,
                  branch::FourPionCosThetaPair2);

DECLARE_SOA_TABLE(BkgroundData, "AOD", "bkgroundData",
                  branch::PosX,
                  branch::PosY,
                  branch::PosZ,
                  branch::Fv0signal,
                  branch::Ft0asignal,
                  branch::Ft0csignal,
                  branch::Fddasignal,
                  branch::Fddcsignal,
                  branch::TimeFv0,
                  branch::TimeFt0a,
                  branch::TimeFt0c,
                  branch::TimeFdda,
                  branch::TimeFddc,
                  branch::TimeZna,
                  branch::TimeZnc,
                  branch::Dcaxy,
                  branch::Dcaz,
                  branch::TpcNsigmaPi,
                  branch::TpcNsigmaKa,
                  branch::TpcNsigmaPr,
                  branch::TpcNsigmaEl,
                  branch::TpcNsigmaMu,
                  branch::TofNsigmaPi,
                  branch::TofNsigmaKa,
                  branch::TofNsigmaPr,
                  branch::TofNsigmaEl,
                  branch::TofNsigmaMu,
                  branch::TpcChi2,
                  branch::TpcNClsFindable,
                  branch::ItsChi2,
                  branch::PionPt,
                  branch::PionEta,
                  branch::PionPhi,
                  branch::PionRapidity,
                  branch::FourPionPt,
                  branch::FourPionEta,
                  branch::FourPionPhi,
                  branch::FourPionRapidity,
                  branch::FourPionMass);

DECLARE_SOA_TABLE(MCgen, "AOD", "MCgen",
                  branch::PionPt,
                  branch::PionEta,
                  branch::PionPhi,
                  branch::PionRapidity,
                  branch::FourPionPt,
                  branch::FourPionEta,
                  branch::FourPionPhi,
                  branch::FourPionRapidity,
                  branch::FourPionMass,
                  branch::FourPionPhiPair1,
                  branch::FourPionPhiPair2,
                  branch::FourPionCosThetaPair1,
                  branch::FourPionCosThetaPair2);

DECLARE_SOA_TABLE(SignalMCreco, "AOD", "SignalMCreco",
                  branch::PosX,
                  branch::PosY,
                  branch::PosZ,
                  branch::Fv0signal,
                  branch::Ft0asignal,
                  branch::Ft0csignal,
                  branch::Fddasignal,
                  branch::Fddcsignal,
                  branch::TimeFv0,
                  branch::TimeFt0a,
                  branch::TimeFt0c,
                  branch::TimeFdda,
                  branch::TimeFddc,
                  branch::TimeZna,
                  branch::TimeZnc,
                  branch::Dcaxy,
                  branch::Dcaz,
                  branch::TpcNsigmaPi,
                  branch::TpcNsigmaKa,
                  branch::TpcNsigmaPr,
                  branch::TpcNsigmaEl,
                  branch::TpcNsigmaMu,
                  branch::TofNsigmaPi,
                  branch::TofNsigmaKa,
                  branch::TofNsigmaPr,
                  branch::TofNsigmaEl,
                  branch::TofNsigmaMu,
                  branch::TpcChi2,
                  branch::TpcNClsFindable,
                  branch::ItsChi2,
                  branch::PionPt,
                  branch::PionEta,
                  branch::PionPhi,
                  branch::PionRapidity,
                  branch::FourPionPt,
                  branch::FourPionEta,
                  branch::FourPionPhi,
                  branch::FourPionRapidity,
                  branch::FourPionMass,
                  branch::FourPionPhiPair1,
                  branch::FourPionPhiPair2,
                  branch::FourPionCosThetaPair1,
                  branch::FourPionCosThetaPair2);
} // namespace o2::aod

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct ExclusiveRhoTo4Pi {
  SGSelector sgSelector;
  int kPiPlus = 211;
  int kPiMinus = -211;
  int kRhoPrime1700 = 30113;
  uint16_t numPVContrib = 4;
  Produces<aod::SignalData> sigFromData;
  Produces<aod::BkgroundData> bkgFromData;
  Produces<aod::MCgen> generatedMC;
  Produces<aod::SignalMCreco> sigFromMC;

  HistogramRegistry histosData{"histosData", {}, OutputObjHandlingPolicy::AnalysisObject, true, true};
  HistogramRegistry histosMCgen{"histosMCgen", {}, OutputObjHandlingPolicy::AnalysisObject, true, true};
  HistogramRegistry histosMCreco{"histosMCreco", {}, OutputObjHandlingPolicy::AnalysisObject, true, true};
  HistogramRegistry histosFast{"histosFast", {}, OutputObjHandlingPolicy::AnalysisObject, true, true};

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  Configurable<float> fv0Cut{"fv0Cut", 50., "FV0A threshold"};
  Configurable<float> ft0aCut{"ft0aCut", 150., "FT0A threshold"};
  Configurable<float> ft0cCut{"ft0cCut", 50., "FT0C threshold"};
  Configurable<float> zdcCut{"zdcCut", 1., "ZDC threshold"};

  Configurable<float> pvCut{"pvCut", 1.0, "Use Only PV tracks"};
  Configurable<float> dcaZcut{"dcaZcut", 2, "dcaZ cut"};
  Configurable<float> dcaXYcut{"dcaXYcut", 0, "dcaXY cut"};
  Configurable<float> tpcChi2Cut{"tpcChi2Cut", 4, "Max tpcChi2NCl"};
  Configurable<float> tpcNClsFindableCut{"tpcNClsFindableCut", 70, "Min tpcNClsFindable"};
  Configurable<float> itsChi2Cut{"itsChi2Cut", 36, "Max itsChi2NCl"};
  Configurable<float> etaCut{"etaCut", 0.9, "Track Pseudorapidity"};
  Configurable<float> pTcut{"pTcut", 0.15, "Track Pt"};

  Configurable<float> nSigmaTPCcut{"nSigmaTPCcut", 3, "TPC cut"};
  Configurable<float> nSigmaTOFcut{"nSigmaTOFcut", 3, "TOF cut"};

  Configurable<int> nBinsPt{"nBinsPt", 1000, "Number of bins for pT"};
  Configurable<int> nBinsInvariantMass{"nBinsInvariantMass", 1000, "Number of bins for Invariant Mass"};
  Configurable<float> invariantMassMin{"invariantMassMin", 0.8, "Minimum Invariant Mass"};
  Configurable<float> invariantMassMax{"invariantMassMax", 2.5, "Maximum Invariant Mass"};
  Configurable<int> nBinsRapidity{"nBinsRapidity", 1000, "Number of bins for Rapidity"};
  Configurable<int> nBinsPhi{"nBinsPhi", 360, "Number of bins for Phi"};
  Configurable<int> nBinsCosTheta{"nBinsCosTheta", 360, "Number of bins for cos Theta"};

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // Begin of Init Function-----------------------------------------------------------------------------------------------------------------------------------------------------
  void init(InitContext const&)
  {

    histosData.add("GapSide", "Gap Side; Events", kTH1F, {{4, -1.5, 2.5}});
    histosData.add("TrueGapSide", "Gap Side; Events", kTH1F, {{4, -1.5, 2.5}});
    histosData.add("EventCounts", "Total Events; Events", kTH1F, {{10, 0, 10}});

    histosData.add("vertexZ", "Vertex Z; Vertex Z [cm]; Counts", kTH1F, {{1000, -20, 20}});
    histosData.add("dcaXY", "dcaXY; dcaXY [cm]; Counts", kTH1F, {{10000, -5, 5}});
    histosData.add("dcaZ", "dcaZ; dcaZ [cm]; Counts", kTH1F, {{10000, -10, 10}});
    histosData.add("tpcChi2NCl", "TPC Chi2/NCl; Chi2/NCl; Counts", kTH1F, {{200, 0, 200}});
    histosData.add("itsChi2NCl", "ITS Chi2/NCl; Chi2/NCl; Counts", kTH1F, {{200, 0, 200}});
    histosData.add("tpcNClsFindable", "TPC N Cls Findable; N Cls Findable; Counts", kTH1F, {{200, 0, 200}});

    // TPC nSigma
    histosData.add("tpcNSigmaPi_WOTS", "TPC nSigma Pion without track selection; Events", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosData.add("tpcNSigmaPi_WTS", "TPC nSigma Pion with track selection; Events", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosData.add("tpcNSigmaPi_WTS_PID_Pi", "TPC nSigma Pion with track selection and PID Selection of Pi; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});

    // TPC nSigma of other particles with selected pion tracks
    histosData.add("tpcNSigmaKa_WTS_PID_Pi", "TPC nSigma Kaon with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosData.add("tpcNSigmaPr_WTS_PID_Pi", "TPC nSigma Proton with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosData.add("tpcNSigmaEl_WTS_PID_Pi", "TPC nSigma Electron with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosData.add("tpcNSigmaMu_WTS_PID_Pi", "TPC nSigma Muon with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});

    // TOF nSigma
    histosData.add("tofNSigmaPi_WTS", "TOF nSigma Pion with track selection; Events", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosData.add("tofNSigmaPi_WOTS", "TOF nSigma Pion without track selection; Events", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosData.add("tofNSigmaPi_WTS_PID_Pi", "TOF nSigma Pion with track selection and PID Selection of Pi; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});

    // TOF nSigma of other particles with selected pion tracks
    histosData.add("tofNSigmaKa_WTS_PID_Pi", "TOF nSigma Kaon with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosData.add("tofNSigmaPr_WTS_PID_Pi", "TOF nSigma Proton with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosData.add("tofNSigmaEl_WTS_PID_Pi", "TOF nSigma Electron with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosData.add("tofNSigmaMu_WTS_PID_Pi", "TOF nSigma Muon with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});

    // Track Transverse Momentum
    histosData.add("pT_track_WOTS", "pT without track selection; pT [GeV/c]; Counts", kTH1F, {{nBinsPt, 0, 2}});
    histosData.add("pT_track_WTS", "pT with track selection; pT [GeV/c]; Counts", kTH1F, {{nBinsPt, 0, 2}});
    histosData.add("pT_track_WTS_PID_Pi", "pT with track selection and PID selection of Pi; pT [GeV/c]; Events", kTH1F, {{nBinsPt, 0, 2}});
    histosData.add("pT_track_WTS_PID_Pi_contributed", "pT with track selection and PID selection of Pi which are contributed to selected event; pT [GeV/c]; Events", kTH1F, {{nBinsPt, 0, 2}});

    // Track Rapidity
    histosData.add("rapidity_track_WOTS", "Rapidity without track selection; y; Counts", kTH1F, {{nBinsRapidity, -2.5, 2.5}});
    histosData.add("rapidity_track_WTS", "Rapidity with track selection; y; Counts", kTH1F, {{nBinsRapidity, -2.5, 2.5}});
    histosData.add("rapidity_track_WTS_PID_Pi", "Rapidity with track selection and PID selection of Pi; y; Events", kTH1F, {{nBinsRapidity, -2.5, 2.5}});
    histosData.add("rapidity_track_WTS_PID_Pi_contributed", "Rapidity with track selection and PID selection of Pi which are contributed to selected event; y; Events", kTH1F, {{nBinsRapidity, -2.5, 2.5}});

    // Zero charge Event Transverse Momentum
    histosData.add("pT_event_0charge_WTS_PID_Pi", "Event pT in 0 Charge Events With Track Selection and PID Selection of Pi; pT [GeV/c]; Events", kTH1F, {{nBinsPt, 0, 2}});

    // Non Zero charge Event Transverse Momentum
    histosData.add("pT_event_non0charge_WTS_PID_Pi", "Event pT in Non 0 Charge Events With Track Selection and PID Selection of Pi; pT [GeV/c]; Events", kTH1F, {{nBinsPt, 0, 2}});

    // Rapidity of 0 charge Events
    histosData.add("rapidity_event_0charge_WTS_PID_Pi_domainA", "Rapidity of Events With Track Selection and PID Selection of Pi for p_{T} < 0.15 GeV/c; y; Events", kTH1F, {{1000, -2.5, 2.5}});
    histosData.add("rapidity_event_0charge_WTS_PID_Pi_domainB", "Rapidity of Events With Track Selection and PID Selection of Pi for 0.15< p_{T} < 0.80 GeV/c; y; Events", kTH1F, {{1000, -2.5, 2.5}});
    histosData.add("rapidity_event_0charge_WTS_PID_Pi_domainC", "Rapidity of Events With Track Selection and PID Selection of Pi for p_{T} > 0.80 GeV/c; y; Events", kTH1F, {{1000, -2.5, 2.5}});

    // Rapidity of non 0 charge Events
    histosData.add("rapidity_event_non0charge_WTS_PID_Pi_domainA", "Rapidity of Events With Track Selection and PID Selection of Pi for p_{T} < 0.15 GeV/c; y; Events", kTH1F, {{nBinsRapidity, -2.5, 2.5}});
    histosData.add("rapidity_event_non0charge_WTS_PID_Pi_domainB", "Rapidity of Events With Track Selection and PID Selection of Pi for 0.15< p_{T} < 0.80 GeV/c$; y; Events", kTH1F, {{nBinsRapidity, -2.5, 2.5}});
    histosData.add("rapidity_event_non0charge_WTS_PID_Pi_domainC", "Rapidity of Events With Track Selection and PID Selection of Pi for p_{T} > 0.80 GeV/c; y; Events", kTH1F, {{nBinsRapidity, -2.5, 2.5}});

    // Pair Invariant Mass
    histosData.add("invMass_pair_1", "Invariant Mass Distribution of 2 pions 1 ; m(#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{5000, 0, 5}});
    histosData.add("invMass_pair_2", "Invariant Mass Distribution of 2 pions 2 ; m(#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{5000, 0, 5}});
    histosData.add("invMass_pair_3", "Invariant Mass Distribution of 2 pions 3 ; m(#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{5000, 0, 5}});
    histosData.add("invMass_pair_4", "Invariant Mass Distribution of 2 pions 4 ; m(#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{5000, 0, 5}});

    // Invariant Mass of 0 charge events
    histosData.add("invMass_event_0charge_WTS_PID_Pi_domainA", "Invariant Mass Distribution of 0 charge Events with PID Selection of Pi for p_{T} < 0.15 GeV/c; m(#pi^{+}#pi^{-}#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}});       // pT < 0.15GeV
    histosData.add("invMass_event_0charge_WTS_PID_Pi_domainB", "Invariant Mass Distribution of 0 charge Events with PID Selection of Pi for 0.15< p_{T} < 0.80 GeV/c; m(#pi^{+}#pi^{-}#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}}); // 0.15GeV < pT < 0.8GeV
    histosData.add("invMass_event_0charge_WTS_PID_Pi_domainC", "Invariant Mass Distribution of 0 charge Events with PID Selection of Pi for p_{T} > 0.80 GeV/c; m(#pi^{+}#pi^{-}#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}});       // 0.8GeV < pT

    // Invariant mass of non 0 charge events
    histosData.add("invMass_event_non0charge_WTS_PID_Pi_domainA", "Invariant Mass Distribution of non 0 charge Events with PID Selection of Pi for p_{T} < 0.15 GeV/c; m(#pi^{+}#pi^{-}#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}});       // pT < 0.15GeV
    histosData.add("invMass_event_non0charge_WTS_PID_Pi_domainB", "Invariant Mass Distribution of non 0 charge Events with PID Selection of Pi for 0.15< p_{T} < 0.80 GeV/c; m(#pi^{+}#pi^{-}#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}}); // 0.15GeV < pT < 0.8GeV
    histosData.add("invMass_event_non0charge_WTS_PID_Pi_domainC", "Invariant Mass Distribution of non 0 charge Events with PID Selection of Pi for p_{T} > 0.80 GeV/c; m(#pi^{+}#pi^{-}#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}});       // 0.8GeV < pT

    // tpc signal
    histosData.add("tpcSignal", "TPC dEdx vs p; p [GeV/c]; dEdx [a.u.]", kTH2F, {{500, 0, 10}, {5000, 0.0, 5000.0}});
    histosData.add("tpcSignal_Pi", "TPC dEdx vs p for pions; p [GeV/c]; dEdx [a.u.]", kTH2F, {{500, 0, 10}, {5000, 0.0, 5000.0}});

    // tof beta
    histosData.add("tofBeta", "TOF beta vs p; p [GeV/c]; #beta", kTH2F, {{500, 0, 10}, {500, 0.0, 1.0}});
    histosData.add("tofBeta_Pi", "TOF beta vs p for pions; p [GeV/c]; #beta", kTH2F, {{500, 0, 10}, {500, 0.0, 1.0}});

    // Other signals
    histosData.add("FT0A", "T0A amplitude", kTH1F, {{2000, 0.0, 500.0}});
    histosData.add("FT0C", "T0C amplitude", kTH1F, {{2000, 0.0, 500.0}});
    histosData.add("ZDC_A", "ZDC amplitude", kTH1F, {{1000, 0.0, 15}});
    histosData.add("ZDC_C", "ZDC amplitude", kTH1F, {{1000, 0.0, 15}});
    histosData.add("V0A", "V0A amplitude", kTH1F, {{1000, 0.0, 100}});

    // Collin Soper Theta and Phi
    histosData.add("CS_phi_pair_1", "#phi Distribution; #phi; Events", kTH1F, {{nBinsPhi, -3.2, 3.2}});
    histosData.add("CS_phi_pair_2", "#phi Distribution; #phi; Events", kTH1F, {{nBinsPhi, -3.2, 3.2}});
    histosData.add("CS_costheta_pair_1", "#theta Distribution;cos(#theta); Counts", kTH1F, {{nBinsCosTheta, -1, 1}});
    histosData.add("CS_costheta_pair_2", "#theta Distribution;cos(#theta); Counts", kTH1F, {{nBinsCosTheta, -1, 1}});
    histosData.add("phi_cosTheta_pair_1", "Phi vs cosTheta; #phi; cos(#theta)", kTH2F, {{nBinsPhi, -3.2, 3.2}, {nBinsCosTheta, -1, 1}});
    histosData.add("phi_cosTheta_pair_2", "Phi vs cosTheta; #phi; cos(#theta)", kTH2F, {{nBinsPhi, -3.2, 3.2}, {nBinsCosTheta, -1, 1}});

    // MC Gen Stuff

    // counts
    histosMCgen.add("rhoPrimeCounts", "Total Rho prime Events; Events", kTH1F, {{10, 0, 10}});

    // Track Stuff
    histosMCgen.add("MCgen_particle_pT", "Generated pT; pT [GeV/c]; Events", kTH1F, {{nBinsPt, 0, 10}});
    histosMCgen.add("MCgen_particle_pT_contributed", "Generated pT; pT [GeV/c]; Events", kTH1F, {{nBinsPt, 0, 10}});
    histosMCgen.add("MCgen_particle_rapidity", "Generated Rapidity; y; Events", kTH1F, {{nBinsRapidity, -2.5, 2.5}});
    histosMCgen.add("MCgen_particle_rapidity_contributed", "Generated Rapidity; y; Events", kTH1F, {{nBinsRapidity, -2.5, 2.5}});

    // Pair Invariant Mass
    histosMCgen.add("MCgen_invMass_pair_1", "Invariant Mass Distribution of 2 pions 1 ; m(#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{5000, 0, 5}});
    histosMCgen.add("MCgen_invMass_pair_2", "Invariant Mass Distribution of 2 pions 2 ; m(#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{5000, 0, 5}});
    histosMCgen.add("MCgen_invMass_pair_3", "Invariant Mass Distribution of 2 pions 3 ; m(#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{5000, 0, 5}});
    histosMCgen.add("MCgen_invMass_pair_4", "Invariant Mass Distribution of 2 pions 4 ; m(#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{5000, 0, 5}});

    // Generated Transverse Momentum, Rapidty and Invariant Mass
    histosMCgen.add("MCgen_rhoPrime_pT", "Generated pT; pT [GeV/c]; Events", kTH1F, {{nBinsPt, 0, 2}});
    histosMCgen.add("MCgen_4pion_pT", "Generated pT; pT [GeV/c]; Events", kTH1F, {{nBinsPt, 0, 2}});
    histosMCgen.add("MCgen_4pion_rapidity", "Generated Rapidity; y; Events", kTH1F, {{nBinsRapidity, -2.5, 2.5}});
    histosMCgen.add("MCgen_4pion_invmass", "Invariant Mass of 4-Pions; m(4-pion); Events", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}});

    // Collin Soper Theta and Phi
    histosMCgen.add("MCgen_CS_phi_pair_1", "#phi Distribution; #phi; Events", kTH1F, {{nBinsPhi, -3.2, 3.2}});
    histosMCgen.add("MCgen_CS_phi_pair_2", "#phi Distribution; #phi; Events", kTH1F, {{nBinsPhi, -3.2, 3.2}});
    histosMCgen.add("MCgen_CS_costheta_pair_1", "#theta Distribution;cos(#theta); Events", kTH1F, {{nBinsCosTheta, -1, 1}});
    histosMCgen.add("MCgen_CS_costheta_pair_2", "#theta Distribution;cos(#theta); Events", kTH1F, {{nBinsCosTheta, -1, 1}});
    histosMCgen.add("MCgen_phi_cosTheta_pair_1", "Phi vs cosTheta; #phi; cos(#theta)", kTH2F, {{nBinsPhi, -3.2, 3.2}, {nBinsCosTheta, -1, 1}});
    histosMCgen.add("MCgen_phi_cosTheta_pair_2", "Phi vs cosTheta; #phi; cos(#theta)", kTH2F, {{nBinsPhi, -3.2, 3.2}, {nBinsCosTheta, -1, 1}});

    // MC Reco Stuff

    histosMCreco.add("vertexZ", "Vertex Z; Vertex Z [cm]; Counts", kTH1F, {{1000, -20, 20}});
    histosMCreco.add("dcaXY", "dcaXY; dcaXY [cm]; Counts", kTH1F, {{10000, -5, 5}});
    histosMCreco.add("dcaZ", "dcaZ; dcaZ [cm]; Counts", kTH1F, {{10000, -10, 10}});
    histosMCreco.add("tpcChi2NCl", "TPC Chi2/NCl; Chi2/NCl; Counts", kTH1F, {{200, 0, 200}});
    histosMCreco.add("itsChi2NCl", "ITS Chi2/NCl; Chi2/NCl; Counts", kTH1F, {{200, 0, 200}});
    histosMCreco.add("tpcNClsFindable", "TPC N Cls Findable; N Cls Findable; Counts", kTH1F, {{200, 0, 200}});

    histosMCreco.add("GapSide", "Gap Side; Events", kTH1F, {{4, -1.5, 2.5}});
    histosMCreco.add("TrueGapSide", "Gap Side; Events", kTH1F, {{4, -1.5, 2.5}});
    histosMCreco.add("EventCounts", "Total Events; Events", kTH1F, {{10, 0, 10}});

    // TPC nSigma
    histosMCreco.add("tpcNSigmaPi_WOTS", "TPC nSigma Pion without track selection; Events", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosMCreco.add("tpcNSigmaPi_WTS", "TPC nSigma Pion with track selection; Events", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosMCreco.add("tpcNSigmaPi_WTS_PID_Pi", "TPC nSigma Pion with track selection and PID Selection of Pi; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});

    // TPC nSigma of other particles with selected pion tracks
    histosMCreco.add("tpcNSigmaKa_WTS_PID_Pi", "TPC nSigma Kaon with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosMCreco.add("tpcNSigmaPr_WTS_PID_Pi", "TPC nSigma Proton with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosMCreco.add("tpcNSigmaEl_WTS_PID_Pi", "TPC nSigma Electron with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosMCreco.add("tpcNSigmaMu_WTS_PID_Pi", "TPC nSigma Muon with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});

    // TOF nSigma
    histosMCreco.add("tofNSigmaPi_WTS", "TOF nSigma Pion with track selection; Events", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosMCreco.add("tofNSigmaPi_WOTS", "TOF nSigma Pion without track selection; Events", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosMCreco.add("tofNSigmaPi_WTS_PID_Pi", "TOF nSigma Pion with track selection and PID Selection of Pi; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});

    // TOF nSigma of other particles with selected pion tracks
    histosMCreco.add("tofNSigmaKa_WTS_PID_Pi", "TOF nSigma Kaon with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosMCreco.add("tofNSigmaPr_WTS_PID_Pi", "TOF nSigma Proton with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosMCreco.add("tofNSigmaEl_WTS_PID_Pi", "TOF nSigma Electron with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});
    histosMCreco.add("tofNSigmaMu_WTS_PID_Pi", "TOF nSigma Muon with track selection and PID Selection of Pion; Entries", kTH2F, {{1000, -15, 15}, {nBinsPt, 0, 10}});

    // Track Transverse Momentum
    histosMCreco.add("pT_track_WOTS", "pT without track selection; pT [GeV/c]; Counts", kTH1F, {{nBinsPt, 0, 2}});
    histosMCreco.add("pT_track_WTS", "pT with track selection; pT [GeV/c]; Counts", kTH1F, {{nBinsPt, 0, 2}});
    histosMCreco.add("pT_track_WTS_PID_Pi", "pT with track selection and PID selection of Pi; pT [GeV/c]; Events", kTH1F, {{nBinsPt, 0, 2}});
    histosMCreco.add("pT_track_WTS_PID_Pi_contributed", "pT with track selection and PID selection of Pi which are contributed to selected event; pT [GeV/c]; Events", kTH1F, {{nBinsPt, 0, 2}});

    // Track Rapidity
    histosMCreco.add("rapidity_track_WOTS", "Rapidity without track selection; y; Counts", kTH1F, {{nBinsRapidity, -2.5, 2.5}});
    histosMCreco.add("rapidity_track_WTS", "Rapidity with track selection; y; Counts", kTH1F, {{nBinsRapidity, -2.5, 2.5}});
    histosMCreco.add("rapidity_track_WTS_PID_Pi", "Rapidity with track selection and PID selection of Pi; y; Events", kTH1F, {{nBinsRapidity, -2.5, 2.5}});
    histosMCreco.add("rapidity_track_WTS_PID_Pi_contributed", "Rapidity with track selection and PID selection of Pi which are contributed to selected event; y; Events", kTH1F, {{nBinsRapidity, -2.5, 2.5}});

    // Zero charge Event Transverse Momentum
    histosMCreco.add("pT_event_0charge_WTS_PID_Pi", "Event pT in 0 Charge Events With Track Selection and PID Selection of Pi; pT [GeV/c]; Events", kTH1F, {{nBinsPt, 0, 2}});

    // Non Zero charge Event Transverse Momentum
    histosMCreco.add("pT_event_non0charge_WTS_PID_Pi", "Event pT in Non 0 Charge Events With Track Selection and PID Selection of Pi; pT [GeV/c]; Events", kTH1F, {{nBinsPt, 0, 2}});

    // Rapidity of 0 charge Events
    histosMCreco.add("rapidity_event_0charge_WTS_PID_Pi_domainA", "Rapidity of Events With Track Selection and PID Selection of Pi for p_{T} < 0.15 GeV/c; y; Events", kTH1F, {{1000, -2.5, 2.5}});
    histosMCreco.add("rapidity_event_0charge_WTS_PID_Pi_domainB", "Rapidity of Events With Track Selection and PID Selection of Pi for 0.15< p_{T} < 0.80 GeV/c; y; Events", kTH1F, {{1000, -2.5, 2.5}});
    histosMCreco.add("rapidity_event_0charge_WTS_PID_Pi_domainC", "Rapidity of Events With Track Selection and PID Selection of Pi for p_{T} > 0.80 GeV/c; y; Events", kTH1F, {{1000, -2.5, 2.5}});

    // Rapidity of non 0 charge Events
    histosMCreco.add("rapidity_event_non0charge_WTS_PID_Pi_domainA", "Rapidity of Events With Track Selection and PID Selection of Pi for p_{T} < 0.15 GeV/c; y; Events", kTH1F, {{nBinsRapidity, -2.5, 2.5}});
    histosMCreco.add("rapidity_event_non0charge_WTS_PID_Pi_domainB", "Rapidity of Events With Track Selection and PID Selection of Pi for 0.15< p_{T} < 0.80 GeV/c$; y; Events", kTH1F, {{nBinsRapidity, -2.5, 2.5}});
    histosMCreco.add("rapidity_event_non0charge_WTS_PID_Pi_domainC", "Rapidity of Events With Track Selection and PID Selection of Pi for p_{T} > 0.80 GeV/c; y; Events", kTH1F, {{nBinsRapidity, -2.5, 2.5}});

    // Pair Invariant Mass
    histosMCreco.add("invMass_pair_1", "Invariant Mass Distribution of 2 pions 1 ; m(#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{5000, 0, 5}});
    histosMCreco.add("invMass_pair_2", "Invariant Mass Distribution of 2 pions 2 ; m(#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{5000, 0, 5}});
    histosMCreco.add("invMass_pair_3", "Invariant Mass Distribution of 2 pions 3 ; m(#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{5000, 0, 5}});
    histosMCreco.add("invMass_pair_4", "Invariant Mass Distribution of 2 pions 4 ; m(#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{5000, 0, 5}});

    // Invariant Mass of 0 charge events
    histosMCreco.add("invMass_event_0charge_WTS_PID_Pi_domainA", "Invariant Mass Distribution of 0 charge Events with PID Selection of Pi for p_{T} < 0.15 GeV/c; m(#pi^{+}#pi^{-}#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}});       // pT < 0.15GeV
    histosMCreco.add("invMass_event_0charge_WTS_PID_Pi_domainB", "Invariant Mass Distribution of 0 charge Events with PID Selection of Pi for 0.15< p_{T} < 0.80 GeV/c; m(#pi^{+}#pi^{-}#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}}); // 0.15GeV < pT < 0.8GeV
    histosMCreco.add("invMass_event_0charge_WTS_PID_Pi_domainC", "Invariant Mass Distribution of 0 charge Events with PID Selection of Pi for p_{T} > 0.80 GeV/c; m(#pi^{+}#pi^{-}#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}});       // 0.8GeV < pT

    // Invariant mass of non 0 charge events
    histosMCreco.add("invMass_event_non0charge_WTS_PID_Pi_domainA", "Invariant Mass Distribution of non 0 charge Events with PID Selection of Pi for p_{T} < 0.15 GeV/c; m(#pi^{+}#pi^{-}#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}});       // pT < 0.15GeV
    histosMCreco.add("invMass_event_non0charge_WTS_PID_Pi_domainB", "Invariant Mass Distribution of non 0 charge Events with PID Selection of Pi for 0.15< p_{T} < 0.80 GeV/c; m(#pi^{+}#pi^{-}#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}}); // 0.15GeV < pT < 0.8GeV
    histosMCreco.add("invMass_event_non0charge_WTS_PID_Pi_domainC", "Invariant Mass Distribution of non 0 charge Events with PID Selection of Pi for p_{T} > 0.80 GeV/c; m(#pi^{+}#pi^{-}#pi^{+}#pi^{-}) [GeV/c]", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}});       // 0.8GeV < pT

    // tpc signal
    histosMCreco.add("tpcSignal", "TPC dEdx vs p; p [GeV/c]; dEdx [a.u.]", kTH2F, {{500, 0, 10}, {5000, 0.0, 5000.0}});
    histosMCreco.add("tpcSignal_Pi", "TPC dEdx vs p for pions; p [GeV/c]; dEdx [a.u.]", kTH2F, {{500, 0, 10}, {5000, 0.0, 5000.0}});

    // tof beta
    histosMCreco.add("tofBeta", "TOF beta vs p; p [GeV/c]; #beta", kTH2F, {{500, 0, 10}, {500, 0.0, 1.0}});
    histosMCreco.add("tofBeta_Pi", "TOF beta vs p for pions; p [GeV/c]; #beta", kTH2F, {{500, 0, 10}, {500, 0.0, 1.0}});

    // Other signals
    histosMCreco.add("FT0A", "T0A amplitude", kTH1F, {{2000, 0.0, 500.0}});
    histosMCreco.add("FT0C", "T0C amplitude", kTH1F, {{2000, 0.0, 500.0}});
    histosMCreco.add("ZDC_A", "ZDC amplitude", kTH1F, {{1000, 0.0, 15}});
    histosMCreco.add("ZDC_C", "ZDC amplitude", kTH1F, {{1000, 0.0, 15}});
    histosMCreco.add("V0A", "V0A amplitude", kTH1F, {{1000, 0.0, 100}});

    // Collin Soper Theta and Phi
    histosMCreco.add("CS_phi_pair_1", "#phi Distribution; #phi; Events", kTH1F, {{nBinsPhi, -3.2, 3.2}});
    histosMCreco.add("CS_phi_pair_2", "#phi Distribution; #phi; Events", kTH1F, {{nBinsPhi, -3.2, 3.2}});
    histosMCreco.add("CS_costheta_pair_1", "#theta Distribution;cos(#theta); Counts", kTH1F, {{nBinsCosTheta, -1, 1}});
    histosMCreco.add("CS_costheta_pair_2", "#theta Distribution;cos(#theta); Counts", kTH1F, {{nBinsCosTheta, -1, 1}});
    histosMCreco.add("phi_cosTheta_pair_1", "Phi vs cosTheta; #phi; cos(#theta)", kTH2F, {{nBinsPhi, -3.2, 3.2}, {nBinsCosTheta, -1, 1}});
    histosMCreco.add("phi_cosTheta_pair_2", "Phi vs cosTheta; #phi; cos(#theta)", kTH2F, {{nBinsPhi, -3.2, 3.2}, {nBinsCosTheta, -1, 1}});

    histosFast.add("4PionMassWithCut", "", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}});
    histosFast.add("4PionMassFull", "", kTH1F, {{nBinsInvariantMass, invariantMassMin, invariantMassMax}});
    histosFast.add("4PionPt", "", kTH1F, {{nBinsPt, 0, 10}});
    histosFast.add("4PionRapidity", "", kTH1F, {{nBinsRapidity, -1, 1}});

  } // End of init function
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // Calculate the Collins-Soper Frame----------------------------------------------------------------------------------------------------------------------------
  double cosThetaCollinsSoperFrame(ROOT::Math::PtEtaPhiMVector pair1, ROOT::Math::PtEtaPhiMVector pair2, ROOT::Math::PtEtaPhiMVector fourpion)
  {
    double halfSqrtSnn = 2680.;
    double massOfLead208 = 193.6823;
    double momentumBeam = std::sqrt(halfSqrtSnn * halfSqrtSnn * 208 * 208 - massOfLead208 * massOfLead208);

    TLorentzVector pProjCM(0., 0., -momentumBeam, halfSqrtSnn * 208); // projectile
    TLorentzVector pTargCM(0., 0., momentumBeam, halfSqrtSnn * 208);  // target

    //  TVector3 beta = (-1. / fourpion.E()) * fourpion.Vect();
    ROOT::Math::PtEtaPhiMVector v1 = pair1;
    ROOT::Math::PtEtaPhiMVector v2 = pair2;
    ROOT::Math::PtEtaPhiMVector v12 = fourpion;

    // Boost to center of mass frame
    ROOT::Math::Boost boostv12{v12.BoostToCM()};
    ROOT::Math::XYZVectorF v1CM{(boostv12(v1).Vect()).Unit()};
    ROOT::Math::XYZVectorF v2CM{(boostv12(v2).Vect()).Unit()};
    ROOT::Math::XYZVectorF beam1CM{(boostv12(pProjCM).Vect()).Unit()};
    ROOT::Math::XYZVectorF beam2CM{(boostv12(pTargCM).Vect()).Unit()};

    // Axes
    ROOT::Math::XYZVectorF zaxisCS{((beam1CM.Unit() - beam2CM.Unit()).Unit())};

    double cosThetaCS = zaxisCS.Dot((v1CM));
    return cosThetaCS;
  } // End of cosThetaCollinsSoperFrame function------------------------------------------------------------------------------------------------------------------------

  // Calculate Phi in Collins-Soper Frame------------------------------------------------------------------------------------------------------------------------
  double phiCollinsSoperFrame(ROOT::Math::PtEtaPhiMVector pair1, ROOT::Math::PtEtaPhiMVector pair2, ROOT::Math::PtEtaPhiMVector fourpion)
  {
    // Half of the energy per pair of the colliding nucleons.
    double halfSqrtSnn = 2680.;
    double massOfLead208 = 193.6823;
    double momentumBeam = std::sqrt(halfSqrtSnn * halfSqrtSnn * 208 * 208 - massOfLead208 * massOfLead208);

    TLorentzVector pProjCM(0., 0., -momentumBeam, halfSqrtSnn * 208); // projectile
    TLorentzVector pTargCM(0., 0., momentumBeam, halfSqrtSnn * 208);  // target
    ROOT::Math::PtEtaPhiMVector v1 = pair1;
    ROOT::Math::PtEtaPhiMVector v2 = pair2;
    ROOT::Math::PtEtaPhiMVector v12 = fourpion;
    // Boost to center of mass frame
    ROOT::Math::Boost boostv12{v12.BoostToCM()};
    ROOT::Math::XYZVectorF v1CM{(boostv12(v1).Vect()).Unit()};
    ROOT::Math::XYZVectorF v2CM{(boostv12(v2).Vect()).Unit()};
    ROOT::Math::XYZVectorF beam1CM{(boostv12(pProjCM).Vect()).Unit()};
    ROOT::Math::XYZVectorF beam2CM{(boostv12(pTargCM).Vect()).Unit()};
    // Axes
    ROOT::Math::XYZVectorF zaxisCS{((beam1CM.Unit() - beam2CM.Unit()).Unit())};
    ROOT::Math::XYZVectorF yaxisCS{(beam1CM.Cross(beam2CM)).Unit()};
    ROOT::Math::XYZVectorF xaxisCS{(yaxisCS.Cross(zaxisCS)).Unit()};

    double phi = std::atan2(yaxisCS.Dot(v1CM), xaxisCS.Dot(v1CM));
    return phi;
  } // End of phiCollinsSoperFrame function------------------------------------------------------------------------------------------------------------------------

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  using UDtracksfull = soa::Join<aod::UDTracks, aod::UDTracksPID, aod::UDTracksExtra, aod::UDTracksFlags, aod::UDTracksDCA>;
  using UDCollisionsFull = soa::Join<aod::UDCollisions, aod::SGCollisions, aod::UDCollisionsSels, aod::UDZdcsReduced>; //
  using UDCollisionFull = UDCollisionsFull::iterator;
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // Begin of Process function--------------------------------------------------------------------------------------------------------------------------------------------------
  void processData(UDCollisionFull const& collision, UDtracksfull const& tracks)
  {

    if (std::abs(collision.posZ()) > 10) {
      return;
    }

    int gapSide = collision.gapSide();
    std::vector<float> parameters = {pvCut, dcaZcut, dcaXYcut, tpcChi2Cut, tpcNClsFindableCut, itsChi2Cut, etaCut, pTcut};
    int truegapSide = sgSelector.trueGap(collision, fv0Cut, ft0aCut, ft0cCut, zdcCut);
    histosData.fill(HIST("GapSide"), gapSide);
    histosData.fill(HIST("TrueGapSide"), truegapSide);
    histosData.fill(HIST("EventCounts"), 1);
    gapSide = truegapSide;

    if ((gapSide != 2)) {
      return;
    }
    histosData.fill(HIST("vertexZ"), collision.posZ());
    histosData.fill(HIST("V0A"), collision.totalFV0AmplitudeA());
    histosData.fill(HIST("FT0A"), collision.totalFT0AmplitudeA());
    histosData.fill(HIST("FT0C"), collision.totalFT0AmplitudeC());
    histosData.fill(HIST("ZDC_A"), collision.energyCommonZNA());
    histosData.fill(HIST("ZDC_C"), collision.energyCommonZNC());

    if (collision.numContrib() != 4) {
      return;
    }

    std::vector<decltype(tracks.begin())> WOTS_tracks;
    std::vector<decltype(tracks.begin())> WTS_tracks;
    std::vector<decltype(tracks.begin())> WTS_PID_Pi_tracks;
    std::vector<decltype(tracks.begin())> Pi_plus_tracks;
    std::vector<decltype(tracks.begin())> Pi_minus_tracks;

    for (const auto& t0 : tracks) {

      WOTS_tracks.push_back(t0);

      if (trackselector(t0, parameters)) {
        WTS_tracks.push_back(t0);

        if (selectionPIDPion(t0, true, nSigmaTPCcut, nSigmaTOFcut)) {
          WTS_PID_Pi_tracks.push_back(t0);
          if (t0.sign() == 1) {
            Pi_plus_tracks.push_back(t0);
          }
          if (t0.sign() == -1) {
            Pi_minus_tracks.push_back(t0);
          }
        } // End of Selection PID Pion

      } // End of track selections

    } // End of loop over tracks

    int numTracksWOTS = static_cast<int>(WOTS_tracks.size());
    int numTracksWTS = static_cast<int>(WTS_tracks.size());
    int numTracksWTSandPIDpi = static_cast<int>(WTS_PID_Pi_tracks.size());
    int numPiPlusTracks = static_cast<int>(Pi_plus_tracks.size());
    int numPionMinusTRacks = static_cast<int>(Pi_minus_tracks.size());

    TLorentzVector tempWOTS;
    for (int i = 0; i < numTracksWOTS; i++) {
      tempWOTS.SetXYZM(WOTS_tracks[i].px(), WOTS_tracks[i].py(), WOTS_tracks[i].pz(), o2::constants::physics::MassPionCharged);
      histosData.fill(HIST("tpcNSigmaPi_WOTS"), WOTS_tracks[i].tpcNSigmaPi(), std::sqrt(WOTS_tracks[i].px() * WOTS_tracks[i].px() + WOTS_tracks[i].py() * WOTS_tracks[i].py()));
      histosData.fill(HIST("tofNSigmaPi_WOTS"), WOTS_tracks[i].tofNSigmaPi(), std::sqrt(WOTS_tracks[i].px() * WOTS_tracks[i].px() + WOTS_tracks[i].py() * WOTS_tracks[i].py()));
      histosData.fill(HIST("pT_track_WOTS"), std::sqrt(WOTS_tracks[i].px() * WOTS_tracks[i].px() + WOTS_tracks[i].py() * WOTS_tracks[i].py()));
      histosData.fill(HIST("rapidity_track_WOTS"), tempWOTS.Rapidity());
    } // End of loop over tracks without selection

    TLorentzVector tempWTS;
    for (int i = 0; i < numTracksWTS; i++) {
      tempWTS.SetXYZM(WTS_tracks[i].px(), WTS_tracks[i].py(), WTS_tracks[i].pz(), o2::constants::physics::MassPionCharged);
      histosData.fill(HIST("tpcSignal"), std::sqrt(WTS_tracks[i].px() * WTS_tracks[i].px() + WTS_tracks[i].py() * WTS_tracks[i].py() + WTS_tracks[i].pz() * WTS_tracks[i].pz()), WTS_tracks[i].tpcSignal());
      histosData.fill(HIST("tofBeta"), std::sqrt(WTS_tracks[i].px() * WTS_tracks[i].px() + WTS_tracks[i].py() * WTS_tracks[i].py() + WTS_tracks[i].pz() * WTS_tracks[i].pz()), WTS_tracks[i].beta());
      histosData.fill(HIST("tpcNSigmaPi_WTS"), WTS_tracks[i].tpcNSigmaPi(), std::sqrt(WTS_tracks[i].px() * WTS_tracks[i].px() + WTS_tracks[i].py() * WTS_tracks[i].py()));
      histosData.fill(HIST("tofNSigmaPi_WTS"), WTS_tracks[i].tofNSigmaPi(), std::sqrt(WTS_tracks[i].px() * WTS_tracks[i].px() + WTS_tracks[i].py() * WTS_tracks[i].py()));
      histosData.fill(HIST("pT_track_WTS"), std::sqrt(WTS_tracks[i].px() * WTS_tracks[i].px() + WTS_tracks[i].py() * WTS_tracks[i].py()));
      histosData.fill(HIST("rapidity_track_WTS"), tempWTS.Rapidity());

      histosData.fill(HIST("itsChi2NCl"), WTS_tracks[i].itsChi2NCl());
      histosData.fill(HIST("tpcChi2NCl"), WTS_tracks[i].tpcChi2NCl());
      histosData.fill(HIST("tpcNClsFindable"), WTS_tracks[i].tpcNClsFindable());
      histosData.fill(HIST("dcaXY"), WTS_tracks[i].dcaXY());
      histosData.fill(HIST("dcaZ"), WTS_tracks[i].dcaZ());

    } // End of loop over tracks with selection only

    TLorentzVector tempWTSPIDPi;
    for (int i = 0; i < numTracksWTSandPIDpi; i++) {

      tempWTSPIDPi.SetXYZM(WTS_PID_Pi_tracks[i].px(), WTS_PID_Pi_tracks[i].py(), WTS_PID_Pi_tracks[i].pz(), o2::constants::physics::MassPionCharged);

      histosData.fill(HIST("tpcSignal_Pi"), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py() + WTS_PID_Pi_tracks[i].pz() * WTS_PID_Pi_tracks[i].pz()), WTS_PID_Pi_tracks[i].tpcSignal());
      histosData.fill(HIST("tofBeta_Pi"), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py() + WTS_PID_Pi_tracks[i].pz() * WTS_PID_Pi_tracks[i].pz()), WTS_PID_Pi_tracks[i].beta());

      histosData.fill(HIST("tpcNSigmaPi_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tpcNSigmaPi(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosData.fill(HIST("tpcNSigmaKa_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tpcNSigmaKa(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosData.fill(HIST("tpcNSigmaPr_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tpcNSigmaPr(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosData.fill(HIST("tpcNSigmaEl_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tpcNSigmaEl(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosData.fill(HIST("tpcNSigmaMu_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tpcNSigmaMu(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));

      histosData.fill(HIST("tofNSigmaPi_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tofNSigmaPi(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosData.fill(HIST("tofNSigmaKa_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tofNSigmaKa(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosData.fill(HIST("tofNSigmaPr_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tofNSigmaPr(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosData.fill(HIST("tofNSigmaEl_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tofNSigmaEl(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosData.fill(HIST("tofNSigmaMu_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tofNSigmaMu(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));

      histosData.fill(HIST("pT_track_WTS_PID_Pi"), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosData.fill(HIST("rapidity_track_WTS_PID_Pi"), tempWTSPIDPi.Rapidity());
    } // End of loop over tracks with selection and PID selection of Pions

    if (numTracksWTSandPIDpi != 4) {
      return;
    }

    // Selecting Events with net charge = 0
    if (numPionMinusTRacks == 2 && numPiPlusTracks == 2) {

      std::vector<double> dcaxy;
      std::vector<double> dcaz;

      std::vector<double> tpcnsigPi;
      std::vector<double> tpcnsigKa;
      std::vector<double> tpcnsigPr;
      std::vector<double> tpcnsigEl;
      std::vector<double> tpcnsigMu;

      std::vector<double> tofnsigPi;
      std::vector<double> tofnsigKa;
      std::vector<double> tofnsigPr;
      std::vector<double> tofnsigEl;
      std::vector<double> tofnsigMu;

      std::vector<double> tpcchi2;
      std::vector<double> tpcnclsfindable;
      std::vector<double> itschi2;

      std::vector<double> pipt;
      std::vector<double> pieta;
      std::vector<double> piphi;
      std::vector<double> pirapidity;

      TLorentzVector p1, p2, p3, p4, p1234;
      ROOT::Math::PtEtaPhiMVector k1, k2, k3, k4, k1234, k13, k14, k23, k24;

      p1.SetXYZM(Pi_plus_tracks[0].px(), Pi_plus_tracks[0].py(), Pi_plus_tracks[0].pz(), o2::constants::physics::MassPionCharged);
      p2.SetXYZM(Pi_plus_tracks[1].px(), Pi_plus_tracks[1].py(), Pi_plus_tracks[1].pz(), o2::constants::physics::MassPionCharged);
      p3.SetXYZM(Pi_minus_tracks[0].px(), Pi_minus_tracks[0].py(), Pi_minus_tracks[0].pz(), o2::constants::physics::MassPionCharged);
      p4.SetXYZM(Pi_minus_tracks[1].px(), Pi_minus_tracks[1].py(), Pi_minus_tracks[1].pz(), o2::constants::physics::MassPionCharged);

      histosData.fill(HIST("pT_track_WTS_PID_Pi_contributed"), p1.Pt());
      histosData.fill(HIST("pT_track_WTS_PID_Pi_contributed"), p2.Pt());
      histosData.fill(HIST("pT_track_WTS_PID_Pi_contributed"), p3.Pt());
      histosData.fill(HIST("pT_track_WTS_PID_Pi_contributed"), p4.Pt());

      histosData.fill(HIST("rapidity_track_WTS_PID_Pi_contributed"), p1.Rapidity());
      histosData.fill(HIST("rapidity_track_WTS_PID_Pi_contributed"), p2.Rapidity());
      histosData.fill(HIST("rapidity_track_WTS_PID_Pi_contributed"), p3.Rapidity());
      histosData.fill(HIST("rapidity_track_WTS_PID_Pi_contributed"), p4.Rapidity());

      k1.SetCoordinates(p1.Pt(), p1.Eta(), p1.Phi(), o2::constants::physics::MassPionCharged);
      k2.SetCoordinates(p2.Pt(), p2.Eta(), p2.Phi(), o2::constants::physics::MassPionCharged);
      k3.SetCoordinates(p3.Pt(), p3.Eta(), p3.Phi(), o2::constants::physics::MassPionCharged);
      k4.SetCoordinates(p4.Pt(), p4.Eta(), p4.Phi(), o2::constants::physics::MassPionCharged);

      dcaxy.push_back(Pi_plus_tracks[0].dcaXY());
      dcaxy.push_back(Pi_plus_tracks[1].dcaXY());
      dcaxy.push_back(Pi_minus_tracks[0].dcaXY());
      dcaxy.push_back(Pi_minus_tracks[1].dcaXY());

      dcaz.push_back(Pi_plus_tracks[0].dcaZ());
      dcaz.push_back(Pi_plus_tracks[1].dcaZ());
      dcaz.push_back(Pi_minus_tracks[0].dcaZ());
      dcaz.push_back(Pi_minus_tracks[1].dcaZ());

      tpcnsigPi.push_back(Pi_plus_tracks[0].tpcNSigmaPi());
      tpcnsigPi.push_back(Pi_plus_tracks[1].tpcNSigmaPi());
      tpcnsigPi.push_back(Pi_minus_tracks[0].tpcNSigmaPi());
      tpcnsigPi.push_back(Pi_minus_tracks[1].tpcNSigmaPi());

      tpcnsigKa.push_back(Pi_plus_tracks[0].tpcNSigmaKa());
      tpcnsigKa.push_back(Pi_plus_tracks[1].tpcNSigmaKa());
      tpcnsigKa.push_back(Pi_minus_tracks[0].tpcNSigmaKa());
      tpcnsigKa.push_back(Pi_minus_tracks[1].tpcNSigmaKa());

      tpcnsigPr.push_back(Pi_plus_tracks[0].tpcNSigmaPr());
      tpcnsigPr.push_back(Pi_plus_tracks[1].tpcNSigmaPr());
      tpcnsigPr.push_back(Pi_minus_tracks[0].tpcNSigmaPr());
      tpcnsigPr.push_back(Pi_minus_tracks[1].tpcNSigmaPr());

      tpcnsigEl.push_back(Pi_plus_tracks[0].tpcNSigmaEl());
      tpcnsigEl.push_back(Pi_plus_tracks[1].tpcNSigmaEl());
      tpcnsigEl.push_back(Pi_minus_tracks[0].tpcNSigmaEl());
      tpcnsigEl.push_back(Pi_minus_tracks[1].tpcNSigmaEl());

      tpcnsigMu.push_back(Pi_plus_tracks[0].tpcNSigmaMu());
      tpcnsigMu.push_back(Pi_plus_tracks[1].tpcNSigmaMu());
      tpcnsigMu.push_back(Pi_minus_tracks[0].tpcNSigmaMu());
      tpcnsigMu.push_back(Pi_minus_tracks[1].tpcNSigmaMu());

      tofnsigPi.push_back(Pi_plus_tracks[0].tofNSigmaPi());
      tofnsigPi.push_back(Pi_plus_tracks[1].tofNSigmaPi());
      tofnsigPi.push_back(Pi_minus_tracks[0].tofNSigmaPi());
      tofnsigPi.push_back(Pi_minus_tracks[1].tofNSigmaPi());

      tofnsigKa.push_back(Pi_plus_tracks[0].tofNSigmaKa());
      tofnsigKa.push_back(Pi_plus_tracks[1].tofNSigmaKa());
      tofnsigKa.push_back(Pi_minus_tracks[0].tofNSigmaKa());
      tofnsigKa.push_back(Pi_minus_tracks[1].tofNSigmaKa());

      tofnsigPr.push_back(Pi_plus_tracks[0].tofNSigmaPr());
      tofnsigPr.push_back(Pi_plus_tracks[1].tofNSigmaPr());
      tofnsigPr.push_back(Pi_minus_tracks[0].tofNSigmaPr());
      tofnsigPr.push_back(Pi_minus_tracks[1].tofNSigmaPr());

      tofnsigEl.push_back(Pi_plus_tracks[0].tofNSigmaEl());
      tofnsigEl.push_back(Pi_plus_tracks[1].tofNSigmaEl());
      tofnsigEl.push_back(Pi_minus_tracks[0].tofNSigmaEl());
      tofnsigEl.push_back(Pi_minus_tracks[1].tofNSigmaEl());

      tofnsigMu.push_back(Pi_plus_tracks[0].tofNSigmaMu());
      tofnsigMu.push_back(Pi_plus_tracks[1].tofNSigmaMu());
      tofnsigMu.push_back(Pi_minus_tracks[0].tofNSigmaMu());
      tofnsigMu.push_back(Pi_minus_tracks[1].tofNSigmaMu());

      tpcchi2.push_back(Pi_plus_tracks[0].tpcChi2NCl());
      tpcchi2.push_back(Pi_plus_tracks[1].tpcChi2NCl());
      tpcchi2.push_back(Pi_minus_tracks[0].tpcChi2NCl());
      tpcchi2.push_back(Pi_minus_tracks[1].tpcChi2NCl());

      tpcnclsfindable.push_back(Pi_plus_tracks[0].tpcNClsFindable());
      tpcnclsfindable.push_back(Pi_plus_tracks[1].tpcNClsFindable());
      tpcnclsfindable.push_back(Pi_minus_tracks[0].tpcNClsFindable());
      tpcnclsfindable.push_back(Pi_minus_tracks[1].tpcNClsFindable());

      itschi2.push_back(Pi_plus_tracks[0].itsChi2NCl());
      itschi2.push_back(Pi_plus_tracks[1].itsChi2NCl());
      itschi2.push_back(Pi_minus_tracks[0].itsChi2NCl());
      itschi2.push_back(Pi_minus_tracks[1].itsChi2NCl());

      pipt.push_back(p1.Pt());
      pipt.push_back(p2.Pt());
      pipt.push_back(p3.Pt());
      pipt.push_back(p4.Pt());

      pieta.push_back(p1.Eta());
      pieta.push_back(p2.Eta());
      pieta.push_back(p3.Eta());
      pieta.push_back(p4.Eta());

      piphi.push_back(p1.Phi());
      piphi.push_back(p2.Phi());
      piphi.push_back(p3.Phi());
      piphi.push_back(p4.Phi());

      pirapidity.push_back(p1.Rapidity());
      pirapidity.push_back(p2.Rapidity());
      pirapidity.push_back(p3.Rapidity());
      pirapidity.push_back(p4.Rapidity());

      p1234 = p1 + p2 + p3 + p4;
      k1234 = k1 + k2 + k3 + k4;

      k13 = k1 + k3;
      k14 = k1 + k4;
      k23 = k2 + k3;
      k24 = k2 + k4;

      double fourPiPhiPair1 = phiCollinsSoperFrame(k13, k24, k1234);
      double fourPiPhiPair2 = phiCollinsSoperFrame(k14, k23, k1234);
      double fourPiCosThetaPair1 = cosThetaCollinsSoperFrame(k13, k24, k1234);
      double fourPiCosThetaPair2 = cosThetaCollinsSoperFrame(k14, k23, k1234);

      sigFromData(
        collision.posX(), collision.posY(), collision.posZ(),
        collision.totalFV0AmplitudeA(), collision.totalFT0AmplitudeA(), collision.totalFT0AmplitudeC(), collision.totalFDDAmplitudeA(), collision.totalFDDAmplitudeC(),
        collision.timeFV0A(), collision.timeFT0A(), collision.timeFT0C(), collision.timeFDDA(), collision.timeFDDC(),
        collision.timeZNA(), collision.timeZNC(),
        dcaxy, dcaz,
        tpcnsigPi, tpcnsigKa, tpcnsigPr, tpcnsigEl, tpcnsigMu,
        tofnsigPi, tofnsigKa, tofnsigPr, tofnsigEl, tofnsigMu,
        tpcchi2, tpcnclsfindable, itschi2,
        pipt, pieta, piphi, pirapidity,
        p1234.Pt(), p1234.Eta(), p1234.Phi(), p1234.Rapidity(), p1234.M(),
        fourPiPhiPair1, fourPiPhiPair2, fourPiCosThetaPair1, fourPiCosThetaPair2);

      if (std::fabs(p1234.Rapidity()) < 0.5) {
        histosData.fill(HIST("pT_event_0charge_WTS_PID_Pi"), p1234.Pt());
        if (p1234.Pt() < 0.15) {
          histosData.fill(HIST("rapidity_event_0charge_WTS_PID_Pi_domainA"), p1234.Rapidity());
          histosData.fill(HIST("invMass_event_0charge_WTS_PID_Pi_domainA"), p1234.M());

          histosData.fill(HIST("invMass_pair_1"), (p1 + p3).M());
          histosData.fill(HIST("invMass_pair_2"), (p1 + p4).M());
          histosData.fill(HIST("invMass_pair_3"), (p2 + p3).M());
          histosData.fill(HIST("invMass_pair_4"), (p2 + p4).M());

          histosData.fill(HIST("CS_phi_pair_1"), fourPiPhiPair1);
          histosData.fill(HIST("CS_phi_pair_2"), fourPiPhiPair2);
          histosData.fill(HIST("CS_costheta_pair_1"), fourPiCosThetaPair1);
          histosData.fill(HIST("CS_costheta_pair_2"), fourPiCosThetaPair2);

          histosData.fill(HIST("phi_cosTheta_pair_1"), fourPiPhiPair1, fourPiCosThetaPair1);
          histosData.fill(HIST("phi_cosTheta_pair_2"), fourPiPhiPair2, fourPiCosThetaPair2);
        }
        if (p1234.Pt() > 0.15 && p1234.Pt() < 0.80) {
          histosData.fill(HIST("rapidity_event_0charge_WTS_PID_Pi_domainB"), p1234.Rapidity());
          histosData.fill(HIST("invMass_event_0charge_WTS_PID_Pi_domainB"), p1234.M());
        }
        if (p1234.Pt() > 0.80) {
          histosData.fill(HIST("rapidity_event_0charge_WTS_PID_Pi_domainC"), p1234.Rapidity());
          histosData.fill(HIST("invMass_event_0charge_WTS_PID_Pi_domainC"), p1234.M());
        }
      } // End of Rapidity range selection

    } // End of Analysis for 0 charge events

    // Selecting Events with net charge != 0 for estimation of background
    if (numPionMinusTRacks != 2 && numPiPlusTracks != 2) {

      std::vector<double> dcaxy;
      std::vector<double> dcaz;

      std::vector<double> tpcnsigPi;
      std::vector<double> tpcnsigKa;
      std::vector<double> tpcnsigPr;
      std::vector<double> tpcnsigEl;
      std::vector<double> tpcnsigMu;

      std::vector<double> tofnsigPi;
      std::vector<double> tofnsigKa;
      std::vector<double> tofnsigPr;
      std::vector<double> tofnsigEl;
      std::vector<double> tofnsigMu;

      std::vector<double> tpcchi2;
      std::vector<double> tpcnclsfindable;
      std::vector<double> itschi2;

      std::vector<double> pipt;
      std::vector<double> pieta;
      std::vector<double> piphi;
      std::vector<double> pirapidity;

      TLorentzVector p1, p2, p3, p4, p1234;
      TLorentzVector tempVec;
      p1.SetXYZM(WTS_PID_Pi_tracks[0].px(), WTS_PID_Pi_tracks[0].py(), WTS_PID_Pi_tracks[0].pz(), o2::constants::physics::MassPionCharged);
      p2.SetXYZM(WTS_PID_Pi_tracks[1].px(), WTS_PID_Pi_tracks[1].py(), WTS_PID_Pi_tracks[1].pz(), o2::constants::physics::MassPionCharged);
      p3.SetXYZM(WTS_PID_Pi_tracks[2].px(), WTS_PID_Pi_tracks[2].py(), WTS_PID_Pi_tracks[2].pz(), o2::constants::physics::MassPionCharged);
      p4.SetXYZM(WTS_PID_Pi_tracks[3].px(), WTS_PID_Pi_tracks[3].py(), WTS_PID_Pi_tracks[3].pz(), o2::constants::physics::MassPionCharged);

      dcaxy.push_back(WTS_PID_Pi_tracks[0].dcaXY());
      dcaxy.push_back(WTS_PID_Pi_tracks[1].dcaXY());
      dcaxy.push_back(WTS_PID_Pi_tracks[2].dcaXY());
      dcaxy.push_back(WTS_PID_Pi_tracks[3].dcaXY());

      dcaz.push_back(WTS_PID_Pi_tracks[0].dcaZ());
      dcaz.push_back(WTS_PID_Pi_tracks[1].dcaZ());
      dcaz.push_back(WTS_PID_Pi_tracks[2].dcaZ());
      dcaz.push_back(WTS_PID_Pi_tracks[3].dcaZ());

      tpcnsigPi.push_back(WTS_PID_Pi_tracks[0].tpcNSigmaPi());
      tpcnsigPi.push_back(WTS_PID_Pi_tracks[1].tpcNSigmaPi());
      tpcnsigPi.push_back(WTS_PID_Pi_tracks[2].tpcNSigmaPi());
      tpcnsigPi.push_back(WTS_PID_Pi_tracks[3].tpcNSigmaPi());

      tpcnsigKa.push_back(WTS_PID_Pi_tracks[0].tpcNSigmaKa());
      tpcnsigKa.push_back(WTS_PID_Pi_tracks[1].tpcNSigmaKa());
      tpcnsigKa.push_back(WTS_PID_Pi_tracks[2].tpcNSigmaKa());
      tpcnsigKa.push_back(WTS_PID_Pi_tracks[3].tpcNSigmaKa());

      tpcnsigPr.push_back(WTS_PID_Pi_tracks[0].tpcNSigmaPr());
      tpcnsigPr.push_back(WTS_PID_Pi_tracks[1].tpcNSigmaPr());
      tpcnsigPr.push_back(WTS_PID_Pi_tracks[2].tpcNSigmaPr());
      tpcnsigPr.push_back(WTS_PID_Pi_tracks[3].tpcNSigmaPr());

      tpcnsigEl.push_back(WTS_PID_Pi_tracks[0].tpcNSigmaEl());
      tpcnsigEl.push_back(WTS_PID_Pi_tracks[1].tpcNSigmaEl());
      tpcnsigEl.push_back(WTS_PID_Pi_tracks[2].tpcNSigmaEl());
      tpcnsigEl.push_back(WTS_PID_Pi_tracks[3].tpcNSigmaEl());

      tpcnsigMu.push_back(WTS_PID_Pi_tracks[0].tpcNSigmaMu());
      tpcnsigMu.push_back(WTS_PID_Pi_tracks[1].tpcNSigmaMu());
      tpcnsigMu.push_back(WTS_PID_Pi_tracks[2].tpcNSigmaMu());
      tpcnsigMu.push_back(WTS_PID_Pi_tracks[3].tpcNSigmaMu());

      tofnsigPi.push_back(WTS_PID_Pi_tracks[0].tofNSigmaPi());
      tofnsigPi.push_back(WTS_PID_Pi_tracks[1].tofNSigmaPi());
      tofnsigPi.push_back(WTS_PID_Pi_tracks[2].tofNSigmaPi());
      tofnsigPi.push_back(WTS_PID_Pi_tracks[3].tofNSigmaPi());

      tofnsigKa.push_back(WTS_PID_Pi_tracks[0].tofNSigmaKa());
      tofnsigKa.push_back(WTS_PID_Pi_tracks[1].tofNSigmaKa());
      tofnsigKa.push_back(WTS_PID_Pi_tracks[2].tofNSigmaKa());
      tofnsigKa.push_back(WTS_PID_Pi_tracks[3].tofNSigmaKa());

      tofnsigPr.push_back(WTS_PID_Pi_tracks[0].tofNSigmaPr());
      tofnsigPr.push_back(WTS_PID_Pi_tracks[1].tofNSigmaPr());
      tofnsigPr.push_back(WTS_PID_Pi_tracks[2].tofNSigmaPr());
      tofnsigPr.push_back(WTS_PID_Pi_tracks[3].tofNSigmaPr());

      tofnsigEl.push_back(WTS_PID_Pi_tracks[0].tofNSigmaEl());
      tofnsigEl.push_back(WTS_PID_Pi_tracks[1].tofNSigmaEl());
      tofnsigEl.push_back(WTS_PID_Pi_tracks[2].tofNSigmaEl());
      tofnsigEl.push_back(WTS_PID_Pi_tracks[3].tofNSigmaEl());

      tofnsigMu.push_back(WTS_PID_Pi_tracks[0].tofNSigmaMu());
      tofnsigMu.push_back(WTS_PID_Pi_tracks[1].tofNSigmaMu());
      tofnsigMu.push_back(WTS_PID_Pi_tracks[2].tofNSigmaMu());
      tofnsigMu.push_back(WTS_PID_Pi_tracks[3].tofNSigmaMu());

      tpcchi2.push_back(WTS_PID_Pi_tracks[0].tpcChi2NCl());
      tpcchi2.push_back(WTS_PID_Pi_tracks[1].tpcChi2NCl());
      tpcchi2.push_back(WTS_PID_Pi_tracks[2].tpcChi2NCl());
      tpcchi2.push_back(WTS_PID_Pi_tracks[3].tpcChi2NCl());

      tpcnclsfindable.push_back(WTS_PID_Pi_tracks[0].tpcNClsFindable());
      tpcnclsfindable.push_back(WTS_PID_Pi_tracks[1].tpcNClsFindable());
      tpcnclsfindable.push_back(WTS_PID_Pi_tracks[2].tpcNClsFindable());
      tpcnclsfindable.push_back(WTS_PID_Pi_tracks[3].tpcNClsFindable());

      itschi2.push_back(WTS_PID_Pi_tracks[0].itsChi2NCl());
      itschi2.push_back(WTS_PID_Pi_tracks[1].itsChi2NCl());
      itschi2.push_back(WTS_PID_Pi_tracks[2].itsChi2NCl());
      itschi2.push_back(WTS_PID_Pi_tracks[3].itsChi2NCl());

      pipt.push_back(p1.Pt());
      pipt.push_back(p2.Pt());
      pipt.push_back(p3.Pt());
      pipt.push_back(p4.Pt());

      pieta.push_back(p1.Eta());
      pieta.push_back(p2.Eta());
      pieta.push_back(p3.Eta());
      pieta.push_back(p4.Eta());

      piphi.push_back(p1.Phi());
      piphi.push_back(p2.Phi());
      piphi.push_back(p3.Phi());
      piphi.push_back(p4.Phi());

      pirapidity.push_back(p1.Rapidity());
      pirapidity.push_back(p2.Rapidity());
      pirapidity.push_back(p3.Rapidity());
      pirapidity.push_back(p4.Rapidity());

      p1234 = p1 + p2 + p3 + p4;

      bkgFromData(
        collision.posX(), collision.posY(), collision.posZ(),
        collision.totalFV0AmplitudeA(), collision.totalFT0AmplitudeA(), collision.totalFT0AmplitudeC(), collision.totalFDDAmplitudeA(), collision.totalFDDAmplitudeC(),
        collision.timeFV0A(), collision.timeFT0A(), collision.timeFT0C(), collision.timeFDDA(), collision.timeFDDC(),
        collision.timeZNA(), collision.timeZNC(),
        dcaxy, dcaz,
        tpcnsigPi, tpcnsigKa, tpcnsigPr, tpcnsigEl, tpcnsigMu,
        tofnsigPi, tofnsigKa, tofnsigPr, tofnsigEl, tofnsigMu,
        tpcchi2, tpcnclsfindable, itschi2,
        pipt, pieta, piphi, pirapidity,
        p1234.Pt(), p1234.Eta(), p1234.Phi(), p1234.Rapidity(), p1234.M());

      if (std::fabs(p1234.Rapidity()) < 0.5) {
        histosData.fill(HIST("pT_event_non0charge_WTS_PID_Pi"), p1234.Pt());

        if (p1234.Pt() < 0.15) {
          histosData.fill(HIST("rapidity_event_non0charge_WTS_PID_Pi_domainA"), p1234.Rapidity());
          histosData.fill(HIST("invMass_event_non0charge_WTS_PID_Pi_domainA"), p1234.M());
        }
        if (p1234.Pt() > 0.15 && p1234.Pt() < 0.80) {
          histosData.fill(HIST("rapidity_event_non0charge_WTS_PID_Pi_domainB"), p1234.Rapidity());
          histosData.fill(HIST("invMass_event_non0charge_WTS_PID_Pi_domainB"), p1234.M());
        }
        if (p1234.Pt() > 0.80) {
          histosData.fill(HIST("rapidity_event_non0charge_WTS_PID_Pi_domainC"), p1234.Rapidity());
          histosData.fill(HIST("invMass_event_non0charge_WTS_PID_Pi_domainC"), p1234.M());
        }
      } // End of Rapidity range selection

    } // End of Analysis for non 0 charge events

  } // End of 4 Pion Analysis Process function for Data
  PROCESS_SWITCH(ExclusiveRhoTo4Pi, processData, "The Process for 4 Pion Analysis from data", true);
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  Filter collCuts = (nabs(o2::aod::collision::posZ) < 10.0f) && (o2::aod::collision::numContrib == numPVContrib);
  Filter fitCuts = (o2::aod::udcollision::totalFT0AmplitudeA < ft0aCut) && (o2::aod::udcollision::totalFT0AmplitudeC < ft0cCut) && (o2::aod::udcollision::totalFV0AmplitudeA < fv0Cut);
  Filter zdcCuts = (o2::aod::udzdc::energyCommonZNA < zdcCut) && (o2::aod::udzdc::energyCommonZNC < zdcCut);
  Filter trackCuts = (o2::aod::track::tpcChi2NCl < tpcChi2Cut) && (o2::aod::track::tpcNClsFindable > tpcNClsFindableCut) && (o2::aod::track::itsChi2NCl < itsChi2Cut) && (nabs(o2::aod::track::eta) < etaCut) && (o2::aod::track::pt > pTcut) && (nabs(o2::aod::track::dcaZ) < dcaZcut) && (nabs(o2::aod::track::dcaXY) < dcaXYcut);
  Filter udtrackCuts = (o2::aod::udtrack::isPVContributor == true);
  Filter pidCuts = (nabs(o2::aod::pidtpc::tpcNSigmaPi) < nSigmaTPCcut);
  using FilteredTracks = soa::Filtered<soa::Join<aod::UDTracks, aod::UDTracksPID, aod::UDTracksExtra, aod::UDTracksFlags, aod::UDTracksDCA>>;
  using FilteredCollisions = soa::Filtered<soa::Join<aod::UDCollisions, aod::SGCollisions, aod::UDCollisionsSels, aod::UDZdcsReduced>>;
  using FilteredCollisionsFull = FilteredCollisions::iterator;

  // // Begin of FAST Process function--------------------------------------------------------------------------------------------------------------------------------------------------
  void processDataFast(FilteredCollisionsFull const& collision, FilteredTracks const& tracks)
  {

    if (tracks.size() != 4) {
      return;
    }

    std::vector<decltype(tracks.begin())> pionPlusTracks;
    std::vector<decltype(tracks.begin())> pionMinusTracks;

    for (const auto& track : tracks) {
      if ((!selectionPIDPion(track, true, nSigmaTPCcut, nSigmaTOFcut))) {
        continue;
      }
      if (track.sign() == 1) {
        pionPlusTracks.push_back(track);
      }
      if (track.sign() == -1) {
        pionMinusTracks.push_back(track);
      }
    } // end of loop over tracks

    if ((pionPlusTracks.size() + pionMinusTracks.size()) != 4) {
      return;
    }

    if (pionPlusTracks.size() == 2 || pionMinusTracks.size() == 2) {

      std::vector<double> dcaxy;
      std::vector<double> dcaz;

      std::vector<double> tpcnsigPi;
      std::vector<double> tpcnsigKa;
      std::vector<double> tpcnsigPr;
      std::vector<double> tpcnsigEl;
      std::vector<double> tpcnsigMu;

      std::vector<double> tofnsigPi;
      std::vector<double> tofnsigKa;
      std::vector<double> tofnsigPr;
      std::vector<double> tofnsigEl;
      std::vector<double> tofnsigMu;

      std::vector<double> tpcchi2;
      std::vector<double> tpcnclsfindable;
      std::vector<double> itschi2;

      std::vector<double> pipt;
      std::vector<double> pieta;
      std::vector<double> piphi;
      std::vector<double> pirapidity;

      TLorentzVector p1, p2, p3, p4, p1234;
      ROOT::Math::PtEtaPhiMVector k1, k2, k3, k4, k1234, k13, k14, k23, k24;

      p1.SetXYZM(pionPlusTracks[0].px(), pionPlusTracks[0].py(), pionPlusTracks[0].pz(), o2::constants::physics::MassPionCharged);
      p2.SetXYZM(pionPlusTracks[1].px(), pionPlusTracks[1].py(), pionPlusTracks[1].pz(), o2::constants::physics::MassPionCharged);
      p3.SetXYZM(pionMinusTracks[0].px(), pionMinusTracks[0].py(), pionMinusTracks[0].pz(), o2::constants::physics::MassPionCharged);
      p4.SetXYZM(pionMinusTracks[1].px(), pionMinusTracks[1].py(), pionMinusTracks[1].pz(), o2::constants::physics::MassPionCharged);

      k1.SetCoordinates(p1.Pt(), p1.Eta(), p1.Phi(), o2::constants::physics::MassPionCharged);
      k2.SetCoordinates(p2.Pt(), p2.Eta(), p2.Phi(), o2::constants::physics::MassPionCharged);
      k3.SetCoordinates(p3.Pt(), p3.Eta(), p3.Phi(), o2::constants::physics::MassPionCharged);
      k4.SetCoordinates(p4.Pt(), p4.Eta(), p4.Phi(), o2::constants::physics::MassPionCharged);

      dcaxy.push_back(pionPlusTracks[0].dcaXY());
      dcaxy.push_back(pionPlusTracks[1].dcaXY());
      dcaxy.push_back(pionMinusTracks[0].dcaXY());
      dcaxy.push_back(pionMinusTracks[1].dcaXY());

      dcaz.push_back(pionPlusTracks[0].dcaZ());
      dcaz.push_back(pionPlusTracks[1].dcaZ());
      dcaz.push_back(pionMinusTracks[0].dcaZ());
      dcaz.push_back(pionMinusTracks[1].dcaZ());

      tpcnsigPi.push_back(pionPlusTracks[0].tpcNSigmaPi());
      tpcnsigPi.push_back(pionPlusTracks[1].tpcNSigmaPi());
      tpcnsigPi.push_back(pionMinusTracks[0].tpcNSigmaPi());
      tpcnsigPi.push_back(pionMinusTracks[1].tpcNSigmaPi());

      tpcnsigKa.push_back(pionPlusTracks[0].tpcNSigmaKa());
      tpcnsigKa.push_back(pionPlusTracks[1].tpcNSigmaKa());
      tpcnsigKa.push_back(pionMinusTracks[0].tpcNSigmaKa());
      tpcnsigKa.push_back(pionMinusTracks[1].tpcNSigmaKa());

      tpcnsigPr.push_back(pionPlusTracks[0].tpcNSigmaPr());
      tpcnsigPr.push_back(pionPlusTracks[1].tpcNSigmaPr());
      tpcnsigPr.push_back(pionMinusTracks[0].tpcNSigmaPr());
      tpcnsigPr.push_back(pionMinusTracks[1].tpcNSigmaPr());

      tpcnsigEl.push_back(pionPlusTracks[0].tpcNSigmaEl());
      tpcnsigEl.push_back(pionPlusTracks[1].tpcNSigmaEl());
      tpcnsigEl.push_back(pionMinusTracks[0].tpcNSigmaEl());
      tpcnsigEl.push_back(pionMinusTracks[1].tpcNSigmaEl());

      tpcnsigMu.push_back(pionPlusTracks[0].tpcNSigmaMu());
      tpcnsigMu.push_back(pionPlusTracks[1].tpcNSigmaMu());
      tpcnsigMu.push_back(pionMinusTracks[0].tpcNSigmaMu());
      tpcnsigMu.push_back(pionMinusTracks[1].tpcNSigmaMu());

      tofnsigPi.push_back(pionPlusTracks[0].tofNSigmaPi());
      tofnsigPi.push_back(pionPlusTracks[1].tofNSigmaPi());
      tofnsigPi.push_back(pionMinusTracks[0].tofNSigmaPi());
      tofnsigPi.push_back(pionMinusTracks[1].tofNSigmaPi());

      tofnsigKa.push_back(pionPlusTracks[0].tofNSigmaKa());
      tofnsigKa.push_back(pionPlusTracks[1].tofNSigmaKa());
      tofnsigKa.push_back(pionMinusTracks[0].tofNSigmaKa());
      tofnsigKa.push_back(pionMinusTracks[1].tofNSigmaKa());

      tofnsigPr.push_back(pionPlusTracks[0].tofNSigmaPr());
      tofnsigPr.push_back(pionPlusTracks[1].tofNSigmaPr());
      tofnsigPr.push_back(pionMinusTracks[0].tofNSigmaPr());
      tofnsigPr.push_back(pionMinusTracks[1].tofNSigmaPr());

      tofnsigEl.push_back(pionPlusTracks[0].tofNSigmaEl());
      tofnsigEl.push_back(pionPlusTracks[1].tofNSigmaEl());
      tofnsigEl.push_back(pionMinusTracks[0].tofNSigmaEl());
      tofnsigEl.push_back(pionMinusTracks[1].tofNSigmaEl());

      tofnsigMu.push_back(pionPlusTracks[0].tofNSigmaMu());
      tofnsigMu.push_back(pionPlusTracks[1].tofNSigmaMu());
      tofnsigMu.push_back(pionMinusTracks[0].tofNSigmaMu());
      tofnsigMu.push_back(pionMinusTracks[1].tofNSigmaMu());

      tpcchi2.push_back(pionPlusTracks[0].tpcChi2NCl());
      tpcchi2.push_back(pionPlusTracks[1].tpcChi2NCl());
      tpcchi2.push_back(pionMinusTracks[0].tpcChi2NCl());
      tpcchi2.push_back(pionMinusTracks[1].tpcChi2NCl());

      tpcnclsfindable.push_back(pionPlusTracks[0].tpcNClsFindable());
      tpcnclsfindable.push_back(pionPlusTracks[1].tpcNClsFindable());
      tpcnclsfindable.push_back(pionMinusTracks[0].tpcNClsFindable());
      tpcnclsfindable.push_back(pionMinusTracks[1].tpcNClsFindable());

      itschi2.push_back(pionPlusTracks[0].itsChi2NCl());
      itschi2.push_back(pionPlusTracks[1].itsChi2NCl());
      itschi2.push_back(pionMinusTracks[0].itsChi2NCl());
      itschi2.push_back(pionMinusTracks[1].itsChi2NCl());

      pipt.push_back(p1.Pt());
      pipt.push_back(p2.Pt());
      pipt.push_back(p3.Pt());
      pipt.push_back(p4.Pt());

      pieta.push_back(p1.Eta());
      pieta.push_back(p2.Eta());
      pieta.push_back(p3.Eta());
      pieta.push_back(p4.Eta());

      piphi.push_back(p1.Phi());
      piphi.push_back(p2.Phi());
      piphi.push_back(p3.Phi());
      piphi.push_back(p4.Phi());

      pirapidity.push_back(p1.Rapidity());
      pirapidity.push_back(p2.Rapidity());
      pirapidity.push_back(p3.Rapidity());
      pirapidity.push_back(p4.Rapidity());

      p1234 = p1 + p2 + p3 + p4;
      k1234 = k1 + k2 + k3 + k4;

      k13 = k1 + k3;
      k14 = k1 + k4;
      k23 = k2 + k3;
      k24 = k2 + k4;

      double fourPiPhiPair1 = phiCollinsSoperFrame(k13, k24, k1234);
      double fourPiPhiPair2 = phiCollinsSoperFrame(k14, k23, k1234);
      double fourPiCosThetaPair1 = cosThetaCollinsSoperFrame(k13, k24, k1234);
      double fourPiCosThetaPair2 = cosThetaCollinsSoperFrame(k14, k23, k1234);

      sigFromData(
        collision.posX(), collision.posY(), collision.posZ(),
        collision.totalFV0AmplitudeA(), collision.totalFT0AmplitudeA(), collision.totalFT0AmplitudeC(), collision.totalFDDAmplitudeA(), collision.totalFDDAmplitudeC(),
        collision.timeFV0A(), collision.timeFT0A(), collision.timeFT0C(), collision.timeFDDA(), collision.timeFDDC(),
        collision.timeZNA(), collision.timeZNC(),
        dcaxy, dcaz,
        tpcnsigPi, tpcnsigKa, tpcnsigPr, tpcnsigEl, tpcnsigMu,
        tofnsigPi, tofnsigKa, tofnsigPr, tofnsigEl, tofnsigMu,
        tpcchi2, tpcnclsfindable, itschi2,
        pipt, pieta, piphi, pirapidity,
        p1234.Pt(), p1234.Eta(), p1234.Phi(), p1234.Rapidity(), p1234.M(),
        fourPiPhiPair1, fourPiPhiPair2, fourPiCosThetaPair1, fourPiCosThetaPair2);

      histosFast.fill(HIST("4PionPt"), p1234.Pt());
      histosFast.fill(HIST("4PionRapidity"), p1234.Rapidity());
      histosFast.fill(HIST("4PionMassFull"), p1234.M());

      if ((p1234.Pt() < 0.15) && (std::abs(p1234.Rapidity()) < 0.5)) {
        histosFast.fill(HIST("4PionMassWithCut"), p1234.M());
      }
    } // End 0 charge event

    if (pionPlusTracks.size() != 2 && pionMinusTracks.size() != 2) {
      std::vector<decltype(tracks.begin())> allTracks;
      for (int i = 0; i < pionPlusTracks.size(); i++) {
        allTracks.push_back(pionPlusTracks[i]);
      }
      for (int i = 0; i < pionMinusTracks.size(); i++) {
        allTracks.push_back(pionMinusTracks[i]);
      }

      TLorentzVector p1, p2, p3, p4, p1234;

      p1.SetXYZM(allTracks[0].px(), allTracks[0].py(), allTracks[0].pz(), o2::constants::physics::MassPionCharged);
      p2.SetXYZM(allTracks[1].px(), allTracks[1].py(), allTracks[1].pz(), o2::constants::physics::MassPionCharged);
      p3.SetXYZM(allTracks[2].px(), allTracks[2].py(), allTracks[2].pz(), o2::constants::physics::MassPionCharged);
      p4.SetXYZM(allTracks[3].px(), allTracks[3].py(), allTracks[3].pz(), o2::constants::physics::MassPionCharged);

      std::vector<double> dcaxy;
      std::vector<double> dcaz;

      std::vector<double> tpcnsigPi;
      std::vector<double> tpcnsigKa;
      std::vector<double> tpcnsigPr;
      std::vector<double> tpcnsigEl;
      std::vector<double> tpcnsigMu;

      std::vector<double> tofnsigPi;
      std::vector<double> tofnsigKa;
      std::vector<double> tofnsigPr;
      std::vector<double> tofnsigEl;
      std::vector<double> tofnsigMu;

      std::vector<double> tpcchi2;
      std::vector<double> tpcnclsfindable;
      std::vector<double> itschi2;

      std::vector<double> pipt;
      std::vector<double> pieta;
      std::vector<double> piphi;
      std::vector<double> pirapidity;

      TLorentzVector tempPionVect;
      for (int i = 0; i < allTracks.size(); i++) {
        tempPionVect.SetXYZM(allTracks[i].px(), allTracks[i].py(), allTracks[i].pz(), o2::constants::physics::MassPionCharged);
        dcaxy.push_back(allTracks[i].dcaXY());
        dcaz.push_back(allTracks[i].dcaZ());
        tpcnsigPi.push_back(allTracks[i].tpcNSigmaPi());
        tpcnsigKa.push_back(allTracks[i].tpcNSigmaKa());
        tpcnsigPr.push_back(allTracks[i].tpcNSigmaPr());
        tpcnsigEl.push_back(allTracks[i].tpcNSigmaEl());
        tpcnsigMu.push_back(allTracks[i].tpcNSigmaMu());
        tofnsigPi.push_back(allTracks[i].tofNSigmaPi());
        tofnsigKa.push_back(allTracks[i].tofNSigmaKa());
        tofnsigPr.push_back(allTracks[i].tofNSigmaPr());
        tofnsigEl.push_back(allTracks[i].tofNSigmaEl());
        tofnsigMu.push_back(allTracks[i].tofNSigmaMu());
        tpcchi2.push_back(allTracks[i].tpcChi2NCl());
        tpcnclsfindable.push_back(allTracks[i].tpcNClsFindable());
        itschi2.push_back(allTracks[i].itsChi2NCl());
        pipt.push_back(tempPionVect.Pt());
        pieta.push_back(tempPionVect.Eta());
        piphi.push_back(tempPionVect.Phi());
        pirapidity.push_back(tempPionVect.Rapidity());
      }

      p1234 = p1 + p2 + p3 + p4;

      bkgFromData(
        collision.posX(), collision.posY(), collision.posZ(),
        collision.totalFV0AmplitudeA(), collision.totalFT0AmplitudeA(), collision.totalFT0AmplitudeC(), collision.totalFDDAmplitudeA(), collision.totalFDDAmplitudeC(),
        collision.timeFV0A(), collision.timeFT0A(), collision.timeFT0C(), collision.timeFDDA(), collision.timeFDDC(),
        collision.timeZNA(), collision.timeZNC(),
        dcaxy, dcaz,
        tpcnsigPi, tpcnsigKa, tpcnsigPr, tpcnsigEl, tpcnsigMu,
        tofnsigPi, tofnsigKa, tofnsigPr, tofnsigEl, tofnsigMu,
        tpcchi2, tpcnclsfindable, itschi2,
        pipt, pieta, piphi, pirapidity,
        p1234.Pt(), p1234.Eta(), p1234.Phi(), p1234.Rapidity(), p1234.M());

    } // end of non 0 charge event

  } // End of 4 Pion Analysis Process function for Fast Data
  PROCESS_SWITCH(ExclusiveRhoTo4Pi, processDataFast, "The Process for 4 Pion Analysis from data fast", true);
  // //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // Begin of MC Generation function-----------------------------------------------------------------------------------------------------------------------------------------------
  void processMCgen(aod::UDMcCollisions::iterator const&, aod::UDMcParticles const& mcParts)
  {
    std::vector<TLorentzVector> piPlusvectors;
    std::vector<TLorentzVector> piMinusvectors;
    TLorentzVector motherVector, tempVector, p1, p2, p3, p4;
    TLorentzVector p1234;

    bool flag = false;

    for (const auto& particle : mcParts) {
      tempVector.SetXYZM(particle.px(), particle.py(), particle.pz(), o2::constants::physics::MassPionCharged);

      if (!particle.has_mothers()) {
        continue;
      }

      for (const auto& mother : particle.mothers_as<aod::UDMcParticles>()) {
        if (mother.pdgCode() == kRhoPrime1700) {
          motherVector.SetXYZM(mother.px(), mother.py(), mother.pz(), o2::constants::physics::MassPionCharged);
          histosMCgen.fill(HIST("MCgen_rhoPrime_pT"), motherVector.Pt());

          if (flag == false) {
            histosMCgen.fill(HIST("rhoPrimeCounts"), 5);
          }
          flag = true;
          if (particle.pdgCode() == kPiPlus) {
            histosMCgen.fill(HIST("MCgen_particle_pT"), tempVector.Pt());
            histosMCgen.fill(HIST("MCgen_particle_rapidity"), tempVector.Rapidity());
            piPlusvectors.push_back(tempVector);
          }
          if (particle.pdgCode() == kPiMinus) {
            histosMCgen.fill(HIST("MCgen_particle_pT"), tempVector.Pt());
            histosMCgen.fill(HIST("MCgen_particle_rapidity"), tempVector.Rapidity());
            piMinusvectors.push_back(tempVector);
          }
        } // End of Mother ID 30113 rho prime
      } // End of loop over mothers
    } // End of loop over MC particles

    if (piPlusvectors.size() != 2 || piMinusvectors.size() != 2) {
      return;
    }

    std::vector<double> pipt;
    std::vector<double> pieta;
    std::vector<double> piphi;
    std::vector<double> pirapidity;

    pipt.push_back(piPlusvectors[0].Pt());
    pipt.push_back(piPlusvectors[1].Pt());
    pipt.push_back(piMinusvectors[0].Pt());
    pipt.push_back(piMinusvectors[1].Pt());

    pieta.push_back(piPlusvectors[0].Eta());
    pieta.push_back(piPlusvectors[1].Eta());
    pieta.push_back(piMinusvectors[0].Eta());
    pieta.push_back(piMinusvectors[1].Eta());

    piphi.push_back(piPlusvectors[0].Phi());
    piphi.push_back(piPlusvectors[1].Phi());
    piphi.push_back(piMinusvectors[0].Phi());
    piphi.push_back(piMinusvectors[1].Phi());

    pirapidity.push_back(piPlusvectors[0].Rapidity());
    pirapidity.push_back(piPlusvectors[1].Rapidity());
    pirapidity.push_back(piMinusvectors[0].Rapidity());
    pirapidity.push_back(piMinusvectors[1].Rapidity());

    p1234 = piPlusvectors[0] + piPlusvectors[1] + piMinusvectors[0] + piMinusvectors[1];

    histosMCgen.fill(HIST("MCgen_particle_pT_contributed"), piPlusvectors[0].Pt());
    histosMCgen.fill(HIST("MCgen_particle_pT_contributed"), piPlusvectors[1].Pt());
    histosMCgen.fill(HIST("MCgen_particle_pT_contributed"), piMinusvectors[0].Pt());
    histosMCgen.fill(HIST("MCgen_particle_pT_contributed"), piMinusvectors[1].Pt());

    histosMCgen.fill(HIST("MCgen_particle_rapidity_contributed"), piPlusvectors[0].Rapidity());
    histosMCgen.fill(HIST("MCgen_particle_rapidity_contributed"), piPlusvectors[1].Rapidity());
    histosMCgen.fill(HIST("MCgen_particle_rapidity_contributed"), piMinusvectors[0].Rapidity());
    histosMCgen.fill(HIST("MCgen_particle_rapidity_contributed"), piMinusvectors[1].Rapidity());

    histosMCgen.fill(HIST("MCgen_4pion_pT"), p1234.Pt());
    histosMCgen.fill(HIST("MCgen_4pion_rapidity"), p1234.Rapidity());
    histosMCgen.fill(HIST("MCgen_4pion_invmass"), p1234.M());

    histosMCgen.fill(HIST("MCgen_invMass_pair_1"), (piPlusvectors[0] + piMinusvectors[0]).M());
    histosMCgen.fill(HIST("MCgen_invMass_pair_2"), (piPlusvectors[0] + piMinusvectors[1]).M());
    histosMCgen.fill(HIST("MCgen_invMass_pair_3"), (piPlusvectors[1] + piMinusvectors[0]).M());
    histosMCgen.fill(HIST("MCgen_invMass_pair_4"), (piPlusvectors[1] + piMinusvectors[1]).M());

    ROOT::Math::PtEtaPhiMVector k1, k2, k3, k4, k1234, k13, k14, k23, k24;

    k1.SetCoordinates(piPlusvectors[0].Pt(), piPlusvectors[0].Eta(), piPlusvectors[0].Phi(), o2::constants::physics::MassPionCharged);
    k2.SetCoordinates(piPlusvectors[1].Pt(), piPlusvectors[1].Eta(), piPlusvectors[1].Phi(), o2::constants::physics::MassPionCharged);
    k3.SetCoordinates(piMinusvectors[0].Pt(), piMinusvectors[0].Eta(), piMinusvectors[0].Phi(), o2::constants::physics::MassPionCharged);
    k4.SetCoordinates(piMinusvectors[1].Pt(), piMinusvectors[1].Eta(), piMinusvectors[1].Phi(), o2::constants::physics::MassPionCharged);

    k1234 = k1 + k2 + k3 + k4;

    k13 = k1 + k3;
    k14 = k1 + k4;
    k23 = k2 + k3;
    k24 = k2 + k4;

    auto phiPair1 = phiCollinsSoperFrame(k13, k24, k1234);
    auto phiPair2 = phiCollinsSoperFrame(k14, k23, k1234);
    auto cosThetaPair1 = cosThetaCollinsSoperFrame(k13, k24, k1234);
    auto cosThetaPair2 = cosThetaCollinsSoperFrame(k14, k23, k1234);

    generatedMC(pipt, pieta, piphi, pirapidity,
                p1234.Pt(), p1234.Eta(), p1234.Phi(), p1234.Rapidity(), p1234.M(),
                phiPair1, phiPair2, cosThetaPair1, cosThetaPair2);

    histosMCgen.fill(HIST("MCgen_CS_phi_pair_1"), phiPair1);
    histosMCgen.fill(HIST("MCgen_CS_phi_pair_2"), phiPair2);
    histosMCgen.fill(HIST("MCgen_CS_costheta_pair_1"), cosThetaPair1);
    histosMCgen.fill(HIST("MCgen_CS_costheta_pair_2"), cosThetaPair2);
    histosMCgen.fill(HIST("MCgen_phi_cosTheta_pair_1"), phiPair1, cosThetaPair1);
    histosMCgen.fill(HIST("MCgen_phi_cosTheta_pair_2"), phiPair2, cosThetaPair2);

  } // End of 4 Pion MC Generation Process function
  PROCESS_SWITCH(ExclusiveRhoTo4Pi, processMCgen, "The Process for 4 Pion Analysis from MC Generation", false);

  using CollisionStuff = soa::Join<aod::UDCollisions_001, aod::SGCollisions, aod::UDCollisionsSels, aod::UDZdcsReduced, aod::UDMcCollsLabels>;
  using CollisionTotal = CollisionStuff::iterator;
  using TrackStuff = soa::Join<aod::UDTracks, aod::UDTracksPID, aod::UDTracksExtra, aod::UDTracksFlags, aod::UDTracksDCA, aod::UDMcTrackLabels>;

  void processMCrec(CollisionTotal const& collision, TrackStuff const& tracks)
  {

    if (std::abs(collision.posZ()) > 10) {
      return;
    }

    if (!collision.has_udMcCollision()) {
      return;
    }

    int gapSide = collision.gapSide();
    std::vector<float> parameters = {pvCut, dcaZcut, dcaXYcut, tpcChi2Cut, tpcNClsFindableCut, itsChi2Cut, etaCut, pTcut};
    int truegapSide = sgSelector.trueGap(collision, fv0Cut, ft0aCut, ft0cCut, zdcCut);
    histosMCreco.fill(HIST("GapSide"), gapSide);
    histosMCreco.fill(HIST("TrueGapSide"), truegapSide);
    histosMCreco.fill(HIST("EventCounts"), 1);
    gapSide = truegapSide;

    if ((gapSide != 2)) {
      return;
    }

    histosMCreco.fill(HIST("vertexZ"), collision.posZ());
    histosMCreco.fill(HIST("V0A"), collision.totalFV0AmplitudeA());
    histosMCreco.fill(HIST("FT0A"), collision.totalFT0AmplitudeA());
    histosMCreco.fill(HIST("FT0C"), collision.totalFT0AmplitudeC());
    histosMCreco.fill(HIST("ZDC_A"), collision.energyCommonZNA());
    histosMCreco.fill(HIST("ZDC_C"), collision.energyCommonZNC());

    if (collision.numContrib() != 4) {
      return;
    }

    std::vector<decltype(tracks.begin())> WOTS_tracks;
    std::vector<decltype(tracks.begin())> WTS_tracks;
    std::vector<decltype(tracks.begin())> WTS_PID_Pi_tracks;
    std::vector<decltype(tracks.begin())> Pi_plus_tracks;
    std::vector<decltype(tracks.begin())> Pi_minus_tracks;

    for (const auto& t0 : tracks) {

      WOTS_tracks.push_back(t0);

      if (trackselector(t0, parameters) && t0.has_udMcParticle()) {
        WTS_tracks.push_back(t0);

        if (selectionPIDPion(t0, true, nSigmaTPCcut, nSigmaTOFcut)) {
          WTS_PID_Pi_tracks.push_back(t0);
          if (t0.sign() == 1) {
            Pi_plus_tracks.push_back(t0);
          }
          if (t0.sign() == -1) {
            Pi_minus_tracks.push_back(t0);
          }
        } // End of Selection PID Pion

      } // End of track selections

    } // End of loop over tracks

    int numTracksWOTS = static_cast<int>(WOTS_tracks.size());
    int numTracksWTS = static_cast<int>(WTS_tracks.size());
    int numTracksWTSandPIDpi = static_cast<int>(WTS_PID_Pi_tracks.size());
    int numPiPlusTracks = static_cast<int>(Pi_plus_tracks.size());
    int numPionMinusTRacks = static_cast<int>(Pi_minus_tracks.size());

    TLorentzVector tempWOTS;
    for (int i = 0; i < numTracksWOTS; i++) {
      tempWOTS.SetXYZM(WOTS_tracks[i].px(), WOTS_tracks[i].py(), WOTS_tracks[i].pz(), o2::constants::physics::MassPionCharged);
      histosMCreco.fill(HIST("tpcNSigmaPi_WOTS"), WOTS_tracks[i].tpcNSigmaPi(), std::sqrt(WOTS_tracks[i].px() * WOTS_tracks[i].px() + WOTS_tracks[i].py() * WOTS_tracks[i].py()));
      histosMCreco.fill(HIST("tofNSigmaPi_WOTS"), WOTS_tracks[i].tofNSigmaPi(), std::sqrt(WOTS_tracks[i].px() * WOTS_tracks[i].px() + WOTS_tracks[i].py() * WOTS_tracks[i].py()));
      histosMCreco.fill(HIST("pT_track_WOTS"), std::sqrt(WOTS_tracks[i].px() * WOTS_tracks[i].px() + WOTS_tracks[i].py() * WOTS_tracks[i].py()));
      histosMCreco.fill(HIST("rapidity_track_WOTS"), tempWOTS.Rapidity());

    } // End of loop over tracks without selection

    TLorentzVector tempWTS;
    for (int i = 0; i < numTracksWTS; i++) {
      tempWTS.SetXYZM(WTS_tracks[i].px(), WTS_tracks[i].py(), WTS_tracks[i].pz(), o2::constants::physics::MassPionCharged);
      histosMCreco.fill(HIST("tpcSignal"), std::sqrt(WTS_tracks[i].px() * WTS_tracks[i].px() + WTS_tracks[i].py() * WTS_tracks[i].py() + WTS_tracks[i].pz() * WTS_tracks[i].pz()), WTS_tracks[i].tpcSignal());
      histosMCreco.fill(HIST("tofBeta"), std::sqrt(WTS_tracks[i].px() * WTS_tracks[i].px() + WTS_tracks[i].py() * WTS_tracks[i].py() + WTS_tracks[i].pz() * WTS_tracks[i].pz()), WTS_tracks[i].beta());
      histosMCreco.fill(HIST("tpcNSigmaPi_WTS"), WTS_tracks[i].tpcNSigmaPi(), std::sqrt(WTS_tracks[i].px() * WTS_tracks[i].px() + WTS_tracks[i].py() * WTS_tracks[i].py()));
      histosMCreco.fill(HIST("tofNSigmaPi_WTS"), WTS_tracks[i].tofNSigmaPi(), std::sqrt(WTS_tracks[i].px() * WTS_tracks[i].px() + WTS_tracks[i].py() * WTS_tracks[i].py()));
      histosMCreco.fill(HIST("pT_track_WTS"), std::sqrt(WTS_tracks[i].px() * WTS_tracks[i].px() + WTS_tracks[i].py() * WTS_tracks[i].py()));
      histosMCreco.fill(HIST("rapidity_track_WTS"), tempWTS.Rapidity());

      histosMCreco.fill(HIST("itsChi2NCl"), WTS_tracks[i].itsChi2NCl());
      histosMCreco.fill(HIST("tpcChi2NCl"), WTS_tracks[i].tpcChi2NCl());
      histosMCreco.fill(HIST("tpcNClsFindable"), WTS_tracks[i].tpcNClsFindable());
      histosMCreco.fill(HIST("dcaXY"), WTS_tracks[i].dcaXY());
      histosMCreco.fill(HIST("dcaZ"), WTS_tracks[i].dcaZ());
    } // End of loop over tracks with selection only

    TLorentzVector tempWTSPIDPi;
    for (int i = 0; i < numTracksWTSandPIDpi; i++) {

      tempWTSPIDPi.SetXYZM(WTS_PID_Pi_tracks[i].px(), WTS_PID_Pi_tracks[i].py(), WTS_PID_Pi_tracks[i].pz(), o2::constants::physics::MassPionCharged);

      histosMCreco.fill(HIST("tpcSignal_Pi"), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py() + WTS_PID_Pi_tracks[i].pz() * WTS_PID_Pi_tracks[i].pz()), WTS_PID_Pi_tracks[i].tpcSignal());
      histosMCreco.fill(HIST("tofBeta_Pi"), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py() + WTS_PID_Pi_tracks[i].pz() * WTS_PID_Pi_tracks[i].pz()), WTS_PID_Pi_tracks[i].beta());

      histosMCreco.fill(HIST("tpcNSigmaPi_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tpcNSigmaPi(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosMCreco.fill(HIST("tpcNSigmaKa_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tpcNSigmaKa(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosMCreco.fill(HIST("tpcNSigmaPr_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tpcNSigmaPr(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosMCreco.fill(HIST("tpcNSigmaEl_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tpcNSigmaEl(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosMCreco.fill(HIST("tpcNSigmaMu_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tpcNSigmaMu(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));

      histosMCreco.fill(HIST("tofNSigmaPi_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tofNSigmaPi(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosMCreco.fill(HIST("tofNSigmaKa_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tofNSigmaKa(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosMCreco.fill(HIST("tofNSigmaPr_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tofNSigmaPr(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosMCreco.fill(HIST("tofNSigmaEl_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tofNSigmaEl(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosMCreco.fill(HIST("tofNSigmaMu_WTS_PID_Pi"), WTS_PID_Pi_tracks[i].tofNSigmaMu(), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));

      histosMCreco.fill(HIST("pT_track_WTS_PID_Pi"), std::sqrt(WTS_PID_Pi_tracks[i].px() * WTS_PID_Pi_tracks[i].px() + WTS_PID_Pi_tracks[i].py() * WTS_PID_Pi_tracks[i].py()));
      histosMCreco.fill(HIST("rapidity_track_WTS_PID_Pi"), tempWTSPIDPi.Rapidity());
    } // End of loop over tracks with selection and PID selection of Pions

    if (numTracksWTSandPIDpi != 4) {
      return;
    }

    // Selecting Events with net charge = 0
    if (numPionMinusTRacks == 2 && numPiPlusTracks == 2) {

      std::vector<double> dcaxy;
      std::vector<double> dcaz;

      std::vector<double> tpcnsigPi;
      std::vector<double> tpcnsigKa;
      std::vector<double> tpcnsigPr;
      std::vector<double> tpcnsigEl;
      std::vector<double> tpcnsigMu;

      std::vector<double> tofnsigPi;
      std::vector<double> tofnsigKa;
      std::vector<double> tofnsigPr;
      std::vector<double> tofnsigEl;
      std::vector<double> tofnsigMu;

      std::vector<double> tpcchi2;
      std::vector<double> tpcnclsfindable;
      std::vector<double> itschi2;

      std::vector<double> pipt;
      std::vector<double> pieta;
      std::vector<double> piphi;
      std::vector<double> pirapidity;

      TLorentzVector p1, p2, p3, p4, p1234;
      ROOT::Math::PtEtaPhiMVector k1, k2, k3, k4, k1234, k13, k14, k23, k24;

      p1.SetXYZM(Pi_plus_tracks[0].px(), Pi_plus_tracks[0].py(), Pi_plus_tracks[0].pz(), o2::constants::physics::MassPionCharged);
      p2.SetXYZM(Pi_plus_tracks[1].px(), Pi_plus_tracks[1].py(), Pi_plus_tracks[1].pz(), o2::constants::physics::MassPionCharged);
      p3.SetXYZM(Pi_minus_tracks[0].px(), Pi_minus_tracks[0].py(), Pi_minus_tracks[0].pz(), o2::constants::physics::MassPionCharged);
      p4.SetXYZM(Pi_minus_tracks[1].px(), Pi_minus_tracks[1].py(), Pi_minus_tracks[1].pz(), o2::constants::physics::MassPionCharged);

      histosMCreco.fill(HIST("pT_track_WTS_PID_Pi_contributed"), p1.Pt());
      histosMCreco.fill(HIST("pT_track_WTS_PID_Pi_contributed"), p2.Pt());
      histosMCreco.fill(HIST("pT_track_WTS_PID_Pi_contributed"), p3.Pt());
      histosMCreco.fill(HIST("pT_track_WTS_PID_Pi_contributed"), p4.Pt());

      histosMCreco.fill(HIST("rapidity_track_WTS_PID_Pi_contributed"), p1.Rapidity());
      histosMCreco.fill(HIST("rapidity_track_WTS_PID_Pi_contributed"), p2.Rapidity());
      histosMCreco.fill(HIST("rapidity_track_WTS_PID_Pi_contributed"), p3.Rapidity());
      histosMCreco.fill(HIST("rapidity_track_WTS_PID_Pi_contributed"), p4.Rapidity());

      k1.SetCoordinates(p1.Pt(), p1.Eta(), p1.Phi(), o2::constants::physics::MassPionCharged);
      k2.SetCoordinates(p2.Pt(), p2.Eta(), p2.Phi(), o2::constants::physics::MassPionCharged);
      k3.SetCoordinates(p3.Pt(), p3.Eta(), p3.Phi(), o2::constants::physics::MassPionCharged);
      k4.SetCoordinates(p4.Pt(), p4.Eta(), p4.Phi(), o2::constants::physics::MassPionCharged);

      dcaxy.push_back(Pi_plus_tracks[0].dcaXY());
      dcaxy.push_back(Pi_plus_tracks[1].dcaXY());
      dcaxy.push_back(Pi_minus_tracks[0].dcaXY());
      dcaxy.push_back(Pi_minus_tracks[1].dcaXY());

      dcaz.push_back(Pi_plus_tracks[0].dcaZ());
      dcaz.push_back(Pi_plus_tracks[1].dcaZ());
      dcaz.push_back(Pi_minus_tracks[0].dcaZ());
      dcaz.push_back(Pi_minus_tracks[1].dcaZ());

      tpcnsigPi.push_back(Pi_plus_tracks[0].tpcNSigmaPi());
      tpcnsigPi.push_back(Pi_plus_tracks[1].tpcNSigmaPi());
      tpcnsigPi.push_back(Pi_minus_tracks[0].tpcNSigmaPi());
      tpcnsigPi.push_back(Pi_minus_tracks[1].tpcNSigmaPi());

      tpcnsigKa.push_back(Pi_plus_tracks[0].tpcNSigmaKa());
      tpcnsigKa.push_back(Pi_plus_tracks[1].tpcNSigmaKa());
      tpcnsigKa.push_back(Pi_minus_tracks[0].tpcNSigmaKa());
      tpcnsigKa.push_back(Pi_minus_tracks[1].tpcNSigmaKa());

      tpcnsigPr.push_back(Pi_plus_tracks[0].tpcNSigmaPr());
      tpcnsigPr.push_back(Pi_plus_tracks[1].tpcNSigmaPr());
      tpcnsigPr.push_back(Pi_minus_tracks[0].tpcNSigmaPr());
      tpcnsigPr.push_back(Pi_minus_tracks[1].tpcNSigmaPr());

      tpcnsigEl.push_back(Pi_plus_tracks[0].tpcNSigmaEl());
      tpcnsigEl.push_back(Pi_plus_tracks[1].tpcNSigmaEl());
      tpcnsigEl.push_back(Pi_minus_tracks[0].tpcNSigmaEl());
      tpcnsigEl.push_back(Pi_minus_tracks[1].tpcNSigmaEl());

      tpcnsigMu.push_back(Pi_plus_tracks[0].tpcNSigmaMu());
      tpcnsigMu.push_back(Pi_plus_tracks[1].tpcNSigmaMu());
      tpcnsigMu.push_back(Pi_minus_tracks[0].tpcNSigmaMu());
      tpcnsigMu.push_back(Pi_minus_tracks[1].tpcNSigmaMu());

      tofnsigPi.push_back(Pi_plus_tracks[0].tofNSigmaPi());
      tofnsigPi.push_back(Pi_plus_tracks[1].tofNSigmaPi());
      tofnsigPi.push_back(Pi_minus_tracks[0].tofNSigmaPi());
      tofnsigPi.push_back(Pi_minus_tracks[1].tofNSigmaPi());

      tofnsigKa.push_back(Pi_plus_tracks[0].tofNSigmaKa());
      tofnsigKa.push_back(Pi_plus_tracks[1].tofNSigmaKa());
      tofnsigKa.push_back(Pi_minus_tracks[0].tofNSigmaKa());
      tofnsigKa.push_back(Pi_minus_tracks[1].tofNSigmaKa());

      tofnsigPr.push_back(Pi_plus_tracks[0].tofNSigmaPr());
      tofnsigPr.push_back(Pi_plus_tracks[1].tofNSigmaPr());
      tofnsigPr.push_back(Pi_minus_tracks[0].tofNSigmaPr());
      tofnsigPr.push_back(Pi_minus_tracks[1].tofNSigmaPr());

      tofnsigEl.push_back(Pi_plus_tracks[0].tofNSigmaEl());
      tofnsigEl.push_back(Pi_plus_tracks[1].tofNSigmaEl());
      tofnsigEl.push_back(Pi_minus_tracks[0].tofNSigmaEl());
      tofnsigEl.push_back(Pi_minus_tracks[1].tofNSigmaEl());

      tofnsigMu.push_back(Pi_plus_tracks[0].tofNSigmaMu());
      tofnsigMu.push_back(Pi_plus_tracks[1].tofNSigmaMu());
      tofnsigMu.push_back(Pi_minus_tracks[0].tofNSigmaMu());
      tofnsigMu.push_back(Pi_minus_tracks[1].tofNSigmaMu());

      tpcchi2.push_back(Pi_plus_tracks[0].tpcChi2NCl());
      tpcchi2.push_back(Pi_plus_tracks[1].tpcChi2NCl());
      tpcchi2.push_back(Pi_minus_tracks[0].tpcChi2NCl());
      tpcchi2.push_back(Pi_minus_tracks[1].tpcChi2NCl());

      tpcnclsfindable.push_back(Pi_plus_tracks[0].tpcNClsFindable());
      tpcnclsfindable.push_back(Pi_plus_tracks[1].tpcNClsFindable());
      tpcnclsfindable.push_back(Pi_minus_tracks[0].tpcNClsFindable());
      tpcnclsfindable.push_back(Pi_minus_tracks[1].tpcNClsFindable());

      itschi2.push_back(Pi_plus_tracks[0].itsChi2NCl());
      itschi2.push_back(Pi_plus_tracks[1].itsChi2NCl());
      itschi2.push_back(Pi_minus_tracks[0].itsChi2NCl());
      itschi2.push_back(Pi_minus_tracks[1].itsChi2NCl());

      pipt.push_back(p1.Pt());
      pipt.push_back(p2.Pt());
      pipt.push_back(p3.Pt());
      pipt.push_back(p4.Pt());

      pieta.push_back(p1.Eta());
      pieta.push_back(p2.Eta());
      pieta.push_back(p3.Eta());
      pieta.push_back(p4.Eta());

      piphi.push_back(p1.Phi());
      piphi.push_back(p2.Phi());
      piphi.push_back(p3.Phi());
      piphi.push_back(p4.Phi());

      pirapidity.push_back(p1.Rapidity());
      pirapidity.push_back(p2.Rapidity());
      pirapidity.push_back(p3.Rapidity());
      pirapidity.push_back(p4.Rapidity());

      p1234 = p1 + p2 + p3 + p4;
      k1234 = k1 + k2 + k3 + k4;

      k13 = k1 + k3;
      k14 = k1 + k4;
      k23 = k2 + k3;
      k24 = k2 + k4;

      double phiPair1 = phiCollinsSoperFrame(k13, k24, k1234);
      double phiPair2 = phiCollinsSoperFrame(k14, k23, k1234);
      double cosThetaPair1 = cosThetaCollinsSoperFrame(k13, k24, k1234);
      double cosThetaPair2 = cosThetaCollinsSoperFrame(k14, k23, k1234);

      sigFromMC(
        collision.posX(), collision.posY(), collision.posZ(),
        collision.totalFV0AmplitudeA(), collision.totalFT0AmplitudeA(), collision.totalFT0AmplitudeC(), collision.totalFDDAmplitudeA(), collision.totalFDDAmplitudeC(),
        collision.timeFV0A(), collision.timeFT0A(), collision.timeFT0C(), collision.timeFDDA(), collision.timeFDDC(),
        collision.timeZNA(), collision.timeZNC(),
        dcaxy, dcaz,
        tpcnsigPi, tpcnsigKa, tpcnsigPr, tpcnsigEl, tpcnsigMu,
        tofnsigPi, tofnsigKa, tofnsigPr, tofnsigEl, tofnsigMu,
        tpcchi2, tpcnclsfindable, itschi2,
        pipt, pieta, piphi, pirapidity,
        p1234.Pt(), p1234.Eta(), p1234.Phi(), p1234.Rapidity(), p1234.M(),
        phiPair1, phiPair2, cosThetaPair1, cosThetaPair2);

      if (std::fabs(p1234.Rapidity()) < 0.5) {
        histosMCreco.fill(HIST("pT_event_0charge_WTS_PID_Pi"), p1234.Pt());
        if (p1234.Pt() < 0.15) {
          histosMCreco.fill(HIST("rapidity_event_0charge_WTS_PID_Pi_domainA"), p1234.Rapidity());
          histosMCreco.fill(HIST("invMass_event_0charge_WTS_PID_Pi_domainA"), p1234.M());

          histosMCreco.fill(HIST("invMass_pair_1"), (p1 + p3).M());
          histosMCreco.fill(HIST("invMass_pair_2"), (p1 + p4).M());
          histosMCreco.fill(HIST("invMass_pair_3"), (p2 + p3).M());
          histosMCreco.fill(HIST("invMass_pair_4"), (p2 + p4).M());

          histosMCreco.fill(HIST("CS_phi_pair_1"), phiPair1);
          histosMCreco.fill(HIST("CS_phi_pair_2"), phiPair2);
          histosMCreco.fill(HIST("CS_costheta_pair_1"), cosThetaPair1);
          histosMCreco.fill(HIST("CS_costheta_pair_2"), cosThetaPair2);
          histosMCreco.fill(HIST("phi_cosTheta_pair_1"), phiPair1, cosThetaPair1);
          histosMCreco.fill(HIST("phi_cosTheta_pair_2"), phiPair2, cosThetaPair2);
        }
        if (p1234.Pt() > 0.15 && p1234.Pt() < 0.80) {
          histosMCreco.fill(HIST("rapidity_event_0charge_WTS_PID_Pi_domainB"), p1234.Rapidity());
          histosMCreco.fill(HIST("invMass_event_0charge_WTS_PID_Pi_domainB"), p1234.M());
        }
        if (p1234.Pt() > 0.80) {
          histosMCreco.fill(HIST("rapidity_event_0charge_WTS_PID_Pi_domainC"), p1234.Rapidity());
          histosMCreco.fill(HIST("invMass_event_0charge_WTS_PID_Pi_domainC"), p1234.M());
        }
      } // End of Rapidity range selection

    } // End of Analysis for 0 charge events

    // Selecting Events with net charge != 0 for estimation of background
    if (numPionMinusTRacks != 2 && numPiPlusTracks != 2) {

      TLorentzVector p1, p2, p3, p4, p1234;
      p1.SetXYZM(WTS_PID_Pi_tracks[0].px(), WTS_PID_Pi_tracks[0].py(), WTS_PID_Pi_tracks[0].pz(), o2::constants::physics::MassPionCharged);
      p2.SetXYZM(WTS_PID_Pi_tracks[1].px(), WTS_PID_Pi_tracks[1].py(), WTS_PID_Pi_tracks[1].pz(), o2::constants::physics::MassPionCharged);
      p3.SetXYZM(WTS_PID_Pi_tracks[2].px(), WTS_PID_Pi_tracks[2].py(), WTS_PID_Pi_tracks[2].pz(), o2::constants::physics::MassPionCharged);
      p4.SetXYZM(WTS_PID_Pi_tracks[3].px(), WTS_PID_Pi_tracks[3].py(), WTS_PID_Pi_tracks[3].pz(), o2::constants::physics::MassPionCharged);

      p1234 = p1 + p2 + p3 + p4;

      if (std::fabs(p1234.Rapidity()) < 0.5) {
        histosMCreco.fill(HIST("pT_event_non0charge_WTS_PID_Pi"), p1234.Pt());

        if (p1234.Pt() < 0.15) {
          histosMCreco.fill(HIST("rapidity_event_non0charge_WTS_PID_Pi_domainA"), p1234.Rapidity());
          histosMCreco.fill(HIST("invMass_event_non0charge_WTS_PID_Pi_domainA"), p1234.M());
        }
        if (p1234.Pt() > 0.15 && p1234.Pt() < 0.80) {
          histosMCreco.fill(HIST("rapidity_event_non0charge_WTS_PID_Pi_domainB"), p1234.Rapidity());
          histosMCreco.fill(HIST("invMass_event_non0charge_WTS_PID_Pi_domainB"), p1234.M());
        }
        if (p1234.Pt() > 0.80) {
          histosMCreco.fill(HIST("rapidity_event_non0charge_WTS_PID_Pi_domainC"), p1234.Rapidity());
          histosMCreco.fill(HIST("invMass_event_non0charge_WTS_PID_Pi_domainC"), p1234.M());
        }
      } // End of Rapidity range selection

    } // End of Analysis for non 0 charge events

  } // End of 4 Pion Analysis Process function for MC Reconstruction
  PROCESS_SWITCH(ExclusiveRhoTo4Pi, processMCrec, "The Process for 4 Pion Analysis from MC Reconstruction", false);

}; // End of Struct exclusiveRhoTo4Pi
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    adaptAnalysisTask<ExclusiveRhoTo4Pi>(cfgc)};
}
