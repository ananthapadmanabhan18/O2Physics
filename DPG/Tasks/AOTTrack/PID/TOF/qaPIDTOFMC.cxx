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

///
/// \file   qaPIDTOFMC.cxx
/// \author Nicolò Jacazio
/// \brief  Task to produce QA output of the PID with TOF running on the MC.
///

// O2 includes
#include "Framework/AnalysisTask.h"
#include "Framework/HistogramRegistry.h"
#include "Framework/StaticFor.h"
#include "Common/DataModel/PIDResponse.h"
#include "Framework/runDataProcessing.h"

using namespace o2;
using namespace o2::framework;
using namespace o2::framework::expressions;
using namespace o2::track;

static constexpr int Np = 9;
static constexpr int NpNp = Np * Np;

std::array<std::shared_ptr<TH1>, Np> hParticlePt;
std::array<std::shared_ptr<TH1>, Np> hParticleP;
std::array<std::shared_ptr<TH1>, Np> hParticleEta;

std::array<std::shared_ptr<TH1>, Np> hTrackPt;
std::array<std::shared_ptr<TH1>, Np> hTrackP;
std::array<std::shared_ptr<TH1>, Np> hTrackEta;
std::array<std::shared_ptr<TH1>, Np> hTrackLength;

std::array<std::shared_ptr<TH2>, Np> hSignalMC;
std::array<std::shared_ptr<TH2>, Np> hSignalMCprm;
std::array<std::shared_ptr<TH2>, Np> hSignalMCstr;
std::array<std::shared_ptr<TH2>, Np> hSignalMCmat;

std::array<std::shared_ptr<TH2>, Np> hNSigma;
std::array<std::shared_ptr<TH2>, Np> hNSigmaprm;
std::array<std::shared_ptr<TH2>, Np> hNSigmastr;
std::array<std::shared_ptr<TH2>, Np> hNSigmamat;

std::array<std::shared_ptr<TH2>, NpNp> hNSigmaMC;
std::array<std::shared_ptr<TH2>, NpNp> hNSigmaMCprm;
std::array<std::shared_ptr<TH2>, NpNp> hNSigmaMCstr;
std::array<std::shared_ptr<TH2>, NpNp> hNSigmaMCmat;

std::array<std::shared_ptr<TH2>, NpNp> hDeltaMCEvTime;
std::array<std::shared_ptr<TH2>, NpNp> hDeltaMCEvTimeprm;
std::array<std::shared_ptr<TH2>, NpNp> hDeltaMCEvTimestr;
std::array<std::shared_ptr<TH2>, NpNp> hDeltaMCEvTimemat;

/// Task to produce the TOF QA plots
struct pidTofQaMc {
  SliceCache cache;
  static constexpr const char* pT[Np] = {"e", "#mu", "#pi", "K", "p", "d", "t", "^{3}He", "#alpha"};
  static constexpr const char* pName[Np] = {"El", "Mu", "Pi", "Ka", "Pr", "De", "Tr", "He", "Al"};
  static constexpr int PDGs[Np] = {11, 13, 211, 321, 2212, 1000010020, 1000010030, 1000020030};
  HistogramRegistry histos{"Histos", {}, OutputObjHandlingPolicy::AnalysisObject};

  Configurable<int> checkPrimaries{"checkPrimaries", 1,
                                   "Whether to check physical primary and secondaries particles for the resolution."};
  Configurable<int> pdgSign{"pdgSign", 0, "Sign of the PDG, -1 0 or 1"};
  Configurable<int> doEl{"doEl", 0, "Process electrons"};
  Configurable<int> doMu{"doMu", 0, "Process muons"};
  Configurable<int> doPi{"doPi", 0, "Process pions"};
  Configurable<int> doKa{"doKa", 0, "Process kaons"};
  Configurable<int> doPr{"doPr", 0, "Process protons"};
  Configurable<int> doDe{"doDe", 0, "Process deuterons"};
  Configurable<int> doTr{"doTr", 0, "Process tritons"};
  Configurable<int> doHe{"doHe", 0, "Process helium3"};
  Configurable<int> doAl{"doAl", 0, "Process alpha"};
  ConfigurableAxis binsPt{"binsPt", {2000, 0.f, 20.f}, "Binning of the pT axis"};
  ConfigurableAxis binsNsigma{"binsNsigma", {2000, -50.f, 50.f}, "Binning of the NSigma axis"};
  ConfigurableAxis binsDelta{"binsDelta", {2000, -500.f, 500.f}, "Binning of the Delta axis"};
  ConfigurableAxis binsSignal{"binsSignal", {6000, 0, 2000}, "Binning of the TPC signal axis"};
  ConfigurableAxis binsLength{"binsLength", {1000, 0, 3000}, "Binning of the Length axis"};
  ConfigurableAxis binsEta{"binsEta", {100, -4, 4}, "Binning of the Eta axis"};
  Configurable<float> minEta{"minEta", -0.8, "Minimum eta in range"};
  Configurable<float> maxEta{"maxEta", 0.8, "Maximum eta in range"};
  Configurable<int> nMinNumberOfContributors{"nMinNumberOfContributors", 2, "Minimum required number of contributors to the vertex"};
  Configurable<int> logAxis{"logAxis", 0, "Flag to use a logarithmic pT axis, in this case the pT limits are the expontents"}; // TODO: support log axis

  template <uint8_t mcID>
  void addParticleHistos(const AxisSpec& ptAxis, const AxisSpec& pAxis, const AxisSpec& signalAxis)
  {
    switch (mcID) {
      case 0:
        if (!doEl) {
          return;
        }
        break;
      case 1:
        if (!doMu) {
          return;
        }
        break;
      case 2:
        if (!doPi) {
          return;
        }
        break;
      case 3:
        if (!doKa) {
          return;
        }
        break;
      case 4:
        if (!doPr) {
          return;
        }
        break;
      case 5:
        if (!doDe) {
          return;
        }
        break;
      case 6:
        if (!doTr) {
          return;
        }
        break;
      case 7:
        if (!doHe) {
          return;
        }
        break;
      case 8:
        if (!doAl) {
          return;
        }
        break;
      default:
        LOG(fatal) << "Can't interpret index";
    }

    const AxisSpec lengthAxis{binsLength, "Track length (cm)"};
    const AxisSpec etaAxis{binsEta, "#it{#eta}"};
    const AxisSpec nSigmaAxis{binsNsigma, Form("N_{#sigma}^{TOF}(%s)", pT[mcID])};
    const AxisSpec deltaAxis{binsDelta, Form("#Delta^{TOF}(%s)", pT[mcID])};

    // Particle info
    hParticlePt[mcID] = histos.add<TH1>(Form("particlept/%s", pName[mcID]), pT[mcID], kTH1D, {ptAxis});
    hParticleP[mcID] = histos.add<TH1>(Form("particlep/%s", pName[mcID]), pT[mcID], kTH1D, {pAxis});
    hParticleEta[mcID] = histos.add<TH1>(Form("particleeta/%s", pName[mcID]), pT[mcID], kTH1D, {etaAxis});

    // Track info
    hTrackPt[mcID] = histos.add<TH1>(Form("trackpt/%s", pName[mcID]), pT[mcID], kTH1D, {ptAxis});
    hTrackP[mcID] = histos.add<TH1>(Form("trackp/%s", pName[mcID]), pT[mcID], kTH1D, {pAxis});
    hTrackEta[mcID] = histos.add<TH1>(Form("tracketa/%s", pName[mcID]), pT[mcID], kTH1D, {etaAxis});
    hTrackLength[mcID] = histos.add<TH1>(Form("tracklength/%s", pName[mcID]), pT[mcID], kTH1D, {lengthAxis});

    // NSigma
    hSignalMC[mcID] = histos.add<TH2>(Form("signalMC/%s", pName[mcID]), pT[mcID], HistType::kTH2F, {pAxis, signalAxis});
    hNSigma[mcID] = histos.add<TH2>(Form("nsigma/%s", pName[mcID]), pT[mcID], HistType::kTH2F, {ptAxis, nSigmaAxis});
    hDeltaMCEvTime[mcID] = histos.add<TH2>(Form("hdeltamcevtime/%s", pName[mcID]), pT[mcID], HistType::kTH2F, {ptAxis, deltaAxis});

    if (!checkPrimaries) {
      return;
    }
    hSignalMCprm[mcID] = histos.add<TH2>(Form("signalMCprm/%s", pName[mcID]), pT[mcID], HistType::kTH2F, {pAxis, signalAxis});
    hSignalMCstr[mcID] = histos.add<TH2>(Form("signalMCstr/%s", pName[mcID]), pT[mcID], HistType::kTH2F, {pAxis, signalAxis});
    hSignalMCmat[mcID] = histos.add<TH2>(Form("signalMCmat/%s", pName[mcID]), pT[mcID], HistType::kTH2F, {pAxis, signalAxis});

    hNSigmaprm[mcID] = histos.add<TH2>(Form("nsigmaprm/%s", pName[mcID]), Form("Primary %s", pT[mcID]), HistType::kTH2F, {ptAxis, nSigmaAxis});
    hNSigmastr[mcID] = histos.add<TH2>(Form("nsigmastr/%s", pName[mcID]), Form("Secondary %s from decay", pT[mcID]), HistType::kTH2F, {ptAxis, nSigmaAxis});
    hNSigmamat[mcID] = histos.add<TH2>(Form("nsigmamat/%s", pName[mcID]), Form("Secondary %s from material", pT[mcID]), HistType::kTH2F, {ptAxis, nSigmaAxis});

    hDeltaMCEvTimeprm[mcID] = histos.add<TH2>(Form("hdeltamcevtimeprm/%s", pName[mcID]), pT[mcID], HistType::kTH2F, {ptAxis, deltaAxis});
    hDeltaMCEvTimestr[mcID] = histos.add<TH2>(Form("hdeltamcevtimestr/%s", pName[mcID]), pT[mcID], HistType::kTH2F, {ptAxis, deltaAxis});
    hDeltaMCEvTimemat[mcID] = histos.add<TH2>(Form("hdeltamcevtimemat/%s", pName[mcID]), pT[mcID], HistType::kTH2F, {ptAxis, deltaAxis});
  }

  template <uint8_t mcID, uint8_t massID>
  void addParticleMCHistos(const AxisSpec& ptAxis, const AxisSpec&, const AxisSpec&)
  {
    switch (mcID) {
      case 0:
        if (!doEl) {
          return;
        }
        break;
      case 1:
        if (!doMu) {
          return;
        }
        break;
      case 2:
        if (!doPi) {
          return;
        }
        break;
      case 3:
        if (!doKa) {
          return;
        }
        break;
      case 4:
        if (!doPr) {
          return;
        }
        break;
      case 5:
        if (!doDe) {
          return;
        }
        break;
      case 6:
        if (!doTr) {
          return;
        }
        break;
      case 7:
        if (!doHe) {
          return;
        }
        break;
      case 8:
        if (!doAl) {
          return;
        }
        break;
      default:
        LOG(fatal) << "Can't interpret index";
    }

    const AxisSpec nSigmaAxis{binsNsigma, Form("N_{#sigma}^{TOF}(%s)", pT[massID])};

    hNSigmaMC[mcID * Np + massID] = histos.add<TH2>(Form("nsigmaMC/%s/%s", pName[mcID], pName[massID]), pT[mcID], HistType::kTH2F, {ptAxis, nSigmaAxis});
    if (checkPrimaries) {
      hNSigmaMCprm[mcID * Np + massID] = histos.add<TH2>(Form("nsigmaMCprm/%s/%s", pName[mcID], pName[massID]), pT[mcID], HistType::kTH2F, {ptAxis, nSigmaAxis});
      hNSigmaMCprm[mcID * Np + massID] = histos.add<TH2>(Form("nsigmaMCstr/%s/%s", pName[mcID], pName[massID]), pT[mcID], HistType::kTH2F, {ptAxis, nSigmaAxis});
      hNSigmaMCprm[mcID * Np + massID] = histos.add<TH2>(Form("nsigmaMCmat/%s/%s", pName[mcID], pName[massID]), pT[mcID], HistType::kTH2F, {ptAxis, nSigmaAxis});
    }
  }

  void init(o2::framework::InitContext&)
  {
    AxisSpec pAxis{binsPt, "#it{p} (GeV/#it{c})"};
    AxisSpec ptAxis{binsPt, "#it{p}_{T} (GeV/#it{c})"};
    if (logAxis) {
      pAxis.makeLogarithmic();
      ptAxis.makeLogarithmic();
    }
    const AxisSpec betaAxis{1000, 0, 1.2, "TOF #beta"};

    histos.add("event/T0", ";Tracks with TOF;T0 (ps);Counts", HistType::kTH2F, {{1000, 0, 1000}, {1000, -1000, 1000}});

    histos.add("event/vertexz", ";Vtx_{z} (cm);Entries", kTH1F, {{100, -20, 20}});

    static_for<0, 8>([&](auto i) {
      static_for<0, 8>([&](auto j) {
        addParticleMCHistos<i, j>(ptAxis, pAxis, betaAxis);
      });
      addParticleHistos<i>(ptAxis, pAxis, betaAxis);
    });

    histos.add("event/tofbeta", "All", HistType::kTH2F, {pAxis, betaAxis});
    if (checkPrimaries) {
      histos.add("event/tofbetaPrm", "Primaries", HistType::kTH2F, {pAxis, betaAxis});
      // histos.add("event/tofbetaSec", "Secondaries", HistType::kTH2F, {pAxis, betaAxis});
      histos.add("event/tofbetaStr", "Secondaries from weak decays", HistType::kTH2F, {pAxis, betaAxis});
      histos.add("event/tofbetaMat", "Secondaries from material", HistType::kTH2F, {pAxis, betaAxis});
    }
    // Print output histograms statistics
    LOG(info) << "Size of the histograms in qaPIDTOFMC";
    histos.print();
  }

  template <uint8_t mcID, typename T>
  void fillParticleInfoForPdg(const T& particle)
  {
    switch (pdgSign.value) {
      case 0:
        if (abs(particle.pdgCode()) != PDGs[mcID]) {
          return;
        }
        break;
      case 1:
        if (particle.pdgCode() != PDGs[mcID]) {
          return;
        }
        break;
      case 2:
        if (particle.pdgCode() != -PDGs[mcID]) {
          return;
        }
        break;
      default:
        LOG(fatal) << "Can't interpret pdgSign";
    }
    switch (mcID) {
      case 0:
        if (!doEl) {
          return;
        }
        break;
      case 1:
        if (!doMu) {
          return;
        }
        break;
      case 2:
        if (!doPi) {
          return;
        }
        break;
      case 3:
        if (!doKa) {
          return;
        }
        break;
      case 4:
        if (!doPr) {
          return;
        }
        break;
      case 5:
        if (!doDe) {
          return;
        }
        break;
      case 6:
        if (!doTr) {
          return;
        }
        break;
      case 7:
        if (!doHe) {
          return;
        }
        break;
      case 8:
        if (!doAl) {
          return;
        }
        break;
      default:
        LOG(fatal) << "Can't interpret index";
    }

    hParticlePt[mcID]->Fill(particle.pt());
    hParticleP[mcID]->Fill(particle.p());
    hParticleEta[mcID]->Fill(particle.p());
  }

  template <uint8_t mcID, typename T, typename TT, typename ET>
  void fillTrackInfoForPdg(const T& track, const TT& particle, const ET& collisionMC)
  {
    switch (mcID) {
      case 0:
        if (!doEl) {
          return;
        }
        break;
      case 1:
        if (!doMu) {
          return;
        }
        break;
      case 2:
        if (!doPi) {
          return;
        }
        break;
      case 3:
        if (!doKa) {
          return;
        }
        break;
      case 4:
        if (!doPr) {
          return;
        }
        break;
      case 5:
        if (!doDe) {
          return;
        }
        break;
      case 6:
        if (!doTr) {
          return;
        }
        break;
      case 7:
        if (!doHe) {
          return;
        }
        break;
      case 8:
        if (!doAl) {
          return;
        }
        break;
      default:
        LOG(fatal) << "Can't interpret index";
    }

    const float nsigma = o2::aod::pidutils::tofNSigma<mcID>(track);

    // Fill for all
    hNSigma[mcID]->Fill(track.pt(), nsigma);
    float expTime = 0.f;
    switch (mcID) {
      case 0:
        expTime = track.tofExpTimeEl();
        break;
      case 1:
        expTime = track.tofExpTimeMu();
        break;
      case 2:
        expTime = track.tofExpTimePi();
        break;
      case 3:
        expTime = track.tofExpTimeKa();
        break;
      case 4:
        expTime = track.tofExpTimePr();
        break;
      case 5:
        expTime = track.tofExpTimeDe();
        break;
      case 6:
        expTime = track.tofExpTimeTr();
        break;
      case 7:
        expTime = track.tofExpTimeHe();
        break;
      case 8:
        expTime = track.tofExpTimeAl();
        break;
      default:
        break;
    }
    hDeltaMCEvTime[mcID]->Fill(track.pt(), track.tofSignal() - expTime - collisionMC.t());

    if (checkPrimaries) {
      if (!particle.isPhysicalPrimary()) {
        if (particle.getProcess() == 4) {
          hNSigmastr[mcID]->Fill(track.pt(), nsigma);
        } else {
          hNSigmamat[mcID]->Fill(track.pt(), nsigma);
        }
      } else {
        hNSigmaprm[mcID]->Fill(track.pt(), nsigma);
      }
    }

    switch (pdgSign.value) {
      case 0:
        if (abs(particle.pdgCode()) != PDGs[mcID]) {
          return;
        }
        break;
      case 1:
        if (particle.pdgCode() != PDGs[mcID]) {
          return;
        }
        break;
      case 2:
        if (particle.pdgCode() != -PDGs[mcID]) {
          return;
        }
        break;
      default:
        LOG(fatal) << "Can't interpret pdgSign";
    }

    // Track info
    hTrackPt[mcID]->Fill(track.pt());
    hTrackP[mcID]->Fill(track.p());
    hTrackEta[mcID]->Fill(track.eta());
    hTrackLength[mcID]->Fill(track.length());

    // PID info
    hSignalMC[mcID]->Fill(track.p(), track.tofBeta());

    if (checkPrimaries) {
      if (!particle.isPhysicalPrimary()) {
        if (particle.getProcess() == 4) {
          hSignalMCstr[mcID]->Fill(track.p(), track.tofBeta());
        } else {
          hSignalMCmat[mcID]->Fill(track.p(), track.tofBeta());
        }
      } else {
        hSignalMCprm[mcID]->Fill(track.p(), track.tofBeta());
      }
    }
  }

  template <uint8_t mcID, uint8_t massID, typename T, typename TT>
  void fillPIDInfoForPdg(const T& track, const TT& particle)
  {
    switch (mcID) {
      case 0:
        if (!doEl) {
          return;
        }
        break;
      case 1:
        if (!doMu) {
          return;
        }
        break;
      case 2:
        if (!doPi) {
          return;
        }
        break;
      case 3:
        if (!doKa) {
          return;
        }
        break;
      case 4:
        if (!doPr) {
          return;
        }
        break;
      case 5:
        if (!doDe) {
          return;
        }
        break;
      case 6:
        if (!doTr) {
          return;
        }
        break;
      case 7:
        if (!doHe) {
          return;
        }
        break;
      case 8:
        if (!doAl) {
          return;
        }
        break;
      default:
        LOG(fatal) << "Can't interpret index";
    }

    switch (pdgSign.value) {
      case 0:
        if (abs(particle.pdgCode()) != PDGs[mcID]) {
          return;
        }
        break;
      case 1:
        if (particle.pdgCode() != PDGs[mcID]) {
          return;
        }
        break;
      case 2:
        if (particle.pdgCode() != -PDGs[mcID]) {
          return;
        }
        break;
      default:
        LOG(fatal) << "Can't interpret pdgSign";
    }

    const float nsigmaMassID = o2::aod::pidutils::tofNSigma<massID>(track);

    hNSigmaMC[mcID * Np + massID]->Fill(track.pt(), nsigmaMassID);
    if (checkPrimaries) {
      if (!particle.isPhysicalPrimary()) {
        if (particle.getProcess() == 4) {
          hNSigmaMCstr[mcID * Np + massID]->Fill(track.pt(), nsigmaMassID);
        } else {
          hNSigmaMCmat[mcID * Np + massID]->Fill(track.pt(), nsigmaMassID);
        }
      } else {
        hNSigmaMCprm[mcID * Np + massID]->Fill(track.pt(), nsigmaMassID);
      }
    }
  }

  using Trks = soa::Join<aod::Tracks, aod::TracksExtra,
                         aod::pidTOFFullEl, aod::pidTOFFullMu, aod::pidTOFFullPi,
                         aod::pidTOFFullKa, aod::pidTOFFullPr, aod::pidTOFFullDe,
                         aod::pidTOFFullTr, aod::pidTOFFullHe, aod::pidTOFFullAl,
                         aod::TOFSignal, aod::TOFEvTime, aod::McTrackLabels>;
  Preslice<Trks> perCol = aod::track::collisionId;
  Preslice<aod::McParticles> perMCCol = aod::mcparticle::mcCollisionId;

  void process(soa::Join<aod::Collisions, aod::McCollisionLabels, aod::EvSels> const& collisions,
               Trks& tracks,
               aod::McParticles& mcParticles,
               aod::McCollisions&)
  {
    for (const auto& collision : collisions) {
      if (collision.numContrib() < nMinNumberOfContributors) {
        return;
      }
      if (!collision.sel8()) {
        continue;
      }
      if (!collision.has_mcCollision()) {
        continue;
      }
      const auto particlesInCollision = mcParticles.sliceByCached(aod::mcparticle::mcCollisionId, collision.mcCollision().globalIndex(), cache);

      for (const auto& p : particlesInCollision) {
        static_for<0, 8>([&](auto i) {
          fillParticleInfoForPdg<i>(p);
        });
      }

      const auto& tracksInCollision = tracks.sliceByCached(aod::track::collisionId, collision.globalIndex(), cache);
      // tracksInCollision.bindExternalIndices(&mcParticles);
      const auto& tracksWithPid = soa::Attach<Trks, aod::TOFBeta>(tracksInCollision);
      // tracksInCollision.copyIndexBindings(tracksWithPid);
      const float collisionTime_ps = collision.collisionTime() * 1000.f;
      unsigned int nTracksWithTOF = 0;
      for (const auto& t : tracksWithPid) {
        //
        if (!t.hasTOF()) { // Skipping tracks without TOF
          continue;
        }
        if (t.eta() < minEta || t.eta() > maxEta) {
          continue;
        }

        nTracksWithTOF++;

        // Fill for all
        const float beta = t.tofBeta();
        histos.fill(HIST("event/tofbeta"), t.p(), beta);
        if (!t.has_mcParticle()) {
          continue;
        }

        const auto& particle = t.mcParticle();

        if (checkPrimaries) {
          if (!particle.isPhysicalPrimary()) {
            // histos.fill(HIST("event/tofbetaSec"), t.p(), beta);
            if (particle.getProcess() == 4) {
              histos.fill(HIST("event/tofbetaStr"), t.tpcInnerParam(), t.tpcSignal());
            } else {
              histos.fill(HIST("event/tofbetaMat"), t.tpcInnerParam(), t.tpcSignal());
            }
          } else {
            histos.fill(HIST("event/tofbetaPrm"), t.p(), beta);
          }
        }

        // Fill with PDG codes
        static_for<0, 8>([&](auto i) {
          static_for<0, 8>([&](auto j) {
            fillPIDInfoForPdg<i, j>(t, particle);
          });
          fillTrackInfoForPdg<i>(t, particle, collision.mcCollision());
        });
      } // track loop
      histos.fill(HIST("event/T0"), nTracksWithTOF, collisionTime_ps);
      histos.fill(HIST("event/vertexz"), collision.posZ());
    } // collision loop
  } // process()
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc) { return WorkflowSpec{adaptAnalysisTask<pidTofQaMc>(cfgc)}; }
