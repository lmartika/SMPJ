#include "SMPJ/AnalysisFW/plugins/ProcessedHadrons.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenLumiInfoHeader.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"

using std::cout;
using std::endl;
using std::vector;

ProcessedHadrons::ProcessedHadrons(edm::ParameterSet const& cfg):
  mIsMCarlo(                                                                                cfg.getUntrackedParameter<bool>("isMCarlo",false)),
  mGenParticles(consumes<reco::GenParticleCollection>(                             cfg.getUntrackedParameter<edm::InputTag>("packedGenParticles",edm::InputTag("")))),
  mCands(mayConsume<pat::PackedCandidateCollection>(                                                          edm::InputTag("packedPFCandidates"))),
  // Trigger
  mFilterNames(                                                                 cfg.getParameter<std::vector<std::string> >("filterName")),
  mFilterBitEcal(mayConsume<bool>(                                                                            edm::InputTag("ecalBadCalibReducedMINIAODFilter"))),
  mFilterBitsRECO(                                                            mayConsume<edm::TriggerResults>(edm::InputTag("TriggerResults","","RECO"))),
  mFilterBitsPAT(                                                             mayConsume<edm::TriggerResults>(edm::InputTag("TriggerResults","","PAT"))),
  mTriggerNames(                                                                cfg.getParameter<std::vector<std::string> >("triggerName")),
  mTriggerBits(                                                               mayConsume<edm::TriggerResults>(edm::InputTag("TriggerResults","","HLT")))
{
  mTree = fs->make<TTree>("ProcessedTree","ProcessedTree");
  mTree->Branch("Pt", &mPt, "Pt/F");
  mTree->Branch("Eta", &mEta, "Eta/F");
  mTree->Branch("Phi", &mPhi, "Phi/F");
  mTree->Branch("E", &mE, "E/F");
  mTree->Branch("Ef_ECALRaw", &mEfEr, "Ef_ECALRaw/F");
  mTree->Branch("Ef_ECAL", &mEfE, "Ef_ECAL/F");
  mTree->Branch("Ef_HCALRaw", &mEfHr, "Ef_HCALRaw/F");
  mTree->Branch("Ef_HCAL", &mEfH, "Ef_HCAL/F");
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedHadrons::beginJob() {
  // Generic boolean indicators
  mSatisfactory = true;
  mNewTrigs = false;
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedHadrons::endJob() {
  cout << "Happily finished processing! :)" << endl;
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedHadrons::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup) {
  bool changed = true;
  if (!mHLTConfig.init(iRun,iSetup,"HLT",changed)) {
    cout << "ProcessedHadrons::analyze: config extraction failure with process name HLT!" << endl;
    return;
  }
  if (!changed) return;
  cout << "Running on global tag " << mHLTConfig.globalTag() << "!!!" << endl;

  // Additional steps to be performed first time in the analysis loop!
  // For MC, this means only updating MET Filter info
  mNewTrigs = true;

  if (mIsMCarlo) return;

  cout << "New trigger menu found!!!" << endl;

  // The triggers that are actively and passively monitored will be set up according to the wishes of the user.
  // We check mHLTConfig, which triggers of these are actually present.
  mSatisfactory = mSatisfactory and trigUpdate(mTriggerNames,mTriggerIndex); 
}
//////////////////////////////////////////////////////////////////////////////////////////
bool ProcessedHadrons::trigUpdate(const vector<string> &tNames, vector<int> &tIndex) {
  tIndex.clear();
  const auto &hltNames = mHLTConfig.triggerNames();
  const unsigned int nHLTs = mHLTConfig.size();
  assert(hltNames.size()==nHLTs);

  mTriggerNamesIndexMap.clear();
  mTriggerNamesMap.clear();
  mTriggerIndexMap.clear();
  for (const auto &tName : tNames) {
    // Create a temporary string and strip the zero from the end, in order to use this with regex.
    string tmpName = tName;
    tmpName.pop_back();
    regex trgversions(Form("%s([0-9]*)",tmpName.c_str()));
    if (tName.back()!='0' and tmpName.back()!='v') {
      cout << "In the 'simpleTrigs = True' mode the trigger names should end with 'v0'! Please correct!" << endl;
      return false;
    }
    // Loop through all the hlt names with any version number matching the generic trigger type.
    int counter = 0;
    for (const auto &hName : hltNames) {
      if (regex_match(hName,trgversions)) {
        ++counter;
        if (mTriggerNamesMap.count(tName)==0) {
          mTriggerNamesMap[tName] = vector<string>();
          mTriggerIndexMap[tName] = vector<int>();
        }

        mTriggerNamesIndexMap[hName] = tIndex.size();
        mTriggerNamesMap[tName].emplace_back(hName); 
        mTriggerIndexMap[tName].emplace_back(mHLTConfig.triggerIndex(hName)); 
        cout << "Monitored trigger " << hName << " " << mTriggerIndexMap[tName].back() << " exists" << endl;
      }
    }
    tIndex.push_back(counter==0 ? -1 : counter);
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedHadrons::analyze(edm::Event const& event, edm::EventSetup const& iSetup) {
  // If the trigger information is not satisfactory, we should not enter the event analysis.
  if (!mSatisfactory) return;

  //-------------- Filter Info ---------------------------------------- 

  // Update the filter positions only when the trigger menu has changed.
  // These steps cannot be taken within "beginRun", as event.triggerNames() is not available there. 
  if (mNewTrigs) {
    mNewTrigs = false; // In data, this is set to false later.

    cout << "Found MET filters:" << endl;
    mFilterPAT = false;
    for (unsigned ifloc = 0; ifloc<2; ++ifloc) {
      edm::Handle<edm::TriggerResults> fBits;
      if (ifloc==0) {
        event.getByToken(mFilterBitsRECO,fBits); 
      } else { 
        event.getByToken(mFilterBitsPAT,fBits);
        mFilterPAT = true;
      }

      const edm::TriggerNames &fNames = event.triggerNames(*fBits);
      mFilterMissing.clear();
      mFilterIndex.clear();
      for (unsigned fltpos = 0; fltpos < mFilterNames.size(); ++fltpos) { // Loop through the requested filter names
        auto &flt = mFilterNames[fltpos];
        int fltIdx = -1;
        for (unsigned int iflt=0; iflt<fBits->size(); ++iflt) { // Try to find this
          string filterName = fNames.triggerName(iflt);
          if (filterName==flt) {
            fltIdx = iflt;
            break;
          }
        }
        mFilterIndex.push_back(fltIdx);
        if (fltIdx==-1) mFilterMissing.emplace_back(fltpos);
        else cout << " " << flt << " :)" << endl;
      }
      if (mFilterMissing.size() < mFilterNames.size()) break;
    }
    cout << "Using MET filters from " << (mFilterPAT ? "PAT" : "RECO") << endl;
    mFilterEcalBad = -1;
    if (mFilterMissing.size()>0) {
      for (auto &fltpos : mFilterMissing) {
        auto &flt = mFilterNames[fltpos];
        if (flt == "Flag_ecalBadCalibReducedMINIAODFilter") {
          cout << "MET filter " << flt << " requires special attention." << endl;
          mFilterEcalBad = fltpos;
        } else {
          cout << "MET filter " << flt << " missing :(" << endl;
        }
      }
    }
  }

  // Fetching data using tokens.
  edm::Handle<edm::TriggerResults> filterBits;
  event.getByToken(mFilterPAT ? mFilterBitsPAT : mFilterBitsRECO,filterBits); 
  const edm::TriggerNames &filterNames = event.triggerNames(*filterBits);

  // Go through the filters and check that all of them are good
  assert(mFilterIndex.size()==mFilterNames.size());
  for (auto iflt = 0u; iflt<mFilterIndex.size(); ++iflt) {
    int fltIdx = mFilterIndex[iflt];
    string flt2;
    bool reject = false;
    if (fltIdx<0) {
      if (mFilterEcalBad>=0 and iflt == abs(mFilterEcalBad)) {
        // This filter needs to be fetched separately.
        edm::Handle<bool> filterBitEcal;
        event.getByToken(mFilterBitEcal,filterBitEcal);
        reject =  !(*filterBitEcal);
        flt2 = "Flag_ecalBadCalibReducedMINIAODFilter";
      } else {
        continue; // If a filter was not found, we skip it
      }
    } else {
      reject = !filterBits->accept(fltIdx);
      flt2 = filterNames.triggerName(fltIdx);
    }
    
    string flt = mFilterNames[iflt];
    if (flt!=flt2) {
      cout << "Mismatch in filter names: " << flt << " " << flt2 << endl;
      mSatisfactory = false;
      return;
    }
    if (reject) return;
  }

  //-------------- Trigger Info (exlusively Data) --------------------- 
  edm::Handle<edm::TriggerResults> triggerBits;
  event.getByToken(mTriggerBits,triggerBits);
  if (!mIsMCarlo) {
    // Trigger counts
    unsigned fire = 0;
    // Primary triggers
    for (auto itrig = 0u; itrig<mTriggerNames.size(); ++itrig) {
      int tIdx = mTriggerIndex[itrig]; 
      if (tIdx<0) continue; // We skip the triggers not present (perfectly normal!)
      const string &tName = mTriggerNames[itrig];

      const auto &tIndex2 = mTriggerIndexMap[tName];
      for (unsigned itrig2 = 0; itrig2 < tIndex2.size(); ++itrig2) {
        const auto &tIdx2 = tIndex2[itrig2];
        if (triggerBits->accept(tIdx2)) ++fire;
      } 
    }
    if (fire==0) return;
  } // !mIsMCarlo
  
  // PF Candidates (for beta calculus)
  edm::Handle<pat::PackedCandidateCollection> cands;
  event.getByToken(mCands, cands);

  // Pick the PF candidates removed by CHS (fromPV==0)
  for (auto cidx = 0u; cidx<cands->size(); ++cidx) {
    const auto &c = cands->at(cidx);
    if (c.pt()<5) continue;
    if (c.isPhoton() or c.isElectron() or c.isMuon()) continue;
    //if (fabs(c.pdgId())!=211) continue;
    if (!c.isIsolatedChargedHadron()) continue;
    double calob = c.rawCaloFraction();
    double caloa = c.caloFraction();
    double hb = c.rawHcalFraction()*calob;
    double ha = c.hcalFraction()*caloa;
    double eb = calob-hb;
    double ea = caloa-ha;
    //hb *= c.energy();
    //ha *= c.energy();
    //ea *= c.energy();
    //eb *= c.energy();
    //cout << c.pt() << " " << c.pdgId() << " " << eb << "/" << ea << " " << hb << "/" << ha << endl;
    mPt  = c.pt();
    mEta = c.eta();
    mPhi = c.phi();
    mE   = c.energy();
    mEfEr = eb;
    mEfE  = ea;
    mEfHr = hb;
    mEfH  = ha;

    mTree->Fill();
  }
  //edm::Handle<GenParticleCollection> gcands;
  //event.getByToken(mGenParticles, gcands);

  //for (auto gidx = 0u; gidx<gcands->size(); ++gidx) {
  //  const auto &gc = gcands->at(gidx);
  //  cout << gc.pt() << endl;
  //}
}

void ProcessedHadrons::beginLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& set) {
}


//////////////////////////////////////////////////////////////////////////////////////////
DEFINE_FWK_MODULE(ProcessedHadrons);
