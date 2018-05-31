# -*- coding: utf-8 -*-

import FWCore.ParameterSet.Config as cms

#from RecoJets.Configuration.RecoPFJets_cff import ak4PFJets, ak4PFJetsCHS
from RecoJets.Configuration.RecoGenJets_cff import ak4GenJets
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
from RecoJets.JetProducers.PFJetParameters_cfi import *
from RecoJets.JetProducers.GenJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff import *
from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.patSequences_cff import *
#from RecoJets.JetProducers.pileupjetidproducer_cfi import *
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.jetTools import *
#from RecoJets.JetProducers.QGTagger_cfi import QGTagger
from triggerlists import *
from filelists import *

## Modified version of jetToolBox from https://github.com/cms-jet/jetToolbox
## Options for PUMethod: Puppi, CS, SK, CHS

# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms

process = cms.Process("Ntuplizer")
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Conditions
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('RecoJets.Configuration.GenJetParticles_cff')
process.load('RecoJets.Configuration.RecoGenJets_cff')
process.load('RecoJets.JetProducers.TrackJetParameters_cfi')
process.load('RecoJets.JetProducers.PileupJetIDParams_cfi')
process.load("PhysicsTools.PatAlgos.patSequences_cff")

process.GlobalTag.globaltag = "80X_dataRun2_2016LegacyRepro_v4"

##-------------------- Import the JEC services -----------------------
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Input
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))#0000))

#triggers = trgZBD16 
triggers = trgAK4D16 
follows = trgAK8D16
#inFiles = ZBD16
inFiles = JHTD16 

process.source = cms.Source("PoolSource", fileNames = inFiles )

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Services
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.load('CommonTools.UtilAlgos.TFileService_cfi')
process.TFileService.fileName=cms.string('DATA.root')

process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")

process.patJets.addTagInfos = True
process.patJets.addAssociatedTracks = True

#process.load('RecoJets.JetProducers.QGTagger_cfi')
#process.QGTagger.srcJets = cms.InputTag('patJets')    # Could be reco::PFJetCollection or pat::JetCollection (both AOD and miniAOD)
#process.QGTagger.jetsLabel  = cms.string('QGL_AK4PFchs')        # Other options: see https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion

#process.out.outputCommands += ['keep *_QGTagger_*_*']
process.load("PhysicsTools.PatAlgos.patSequences_cff")

process.ak4 =  cms.EDAnalyzer('ProcessedTreeProducerBTag',
  ## jet collections ###########################
  pfjets          = cms.InputTag('slimmedJets'),
  pfjetschs       = cms.InputTag('slimmedJets'),
  pfpujetid       = cms.string('AK4PFpileupJetIdEvaluator:fullDiscriminant'),
  pfchsjetpuid    = cms.string('AK4PFCHSpileupJetIdEvaluator:fullDiscriminant'),
  ## MET collection ####
  pfmetT1         = cms.InputTag('slimmedMETs'),
  genjets         = cms.untracked.InputTag('slimmedGenJets'),
  ## database entry for the uncertainties ######
  PFPayloadName   = cms.string(''),
  PFPayloadNameCHS= cms.string(''),
  jecUncSrc       = cms.untracked.string(''),
  jecUncSrcCHS    = cms.string(''),
  jecUncSrcNames  = cms.vstring(''),
  ## set the conditions for good Vtx counting ##
  offlineVertices = cms.InputTag('offlineSlimmedPrimaryVertices'),
  beamSpot        = cms.InputTag('offlineBeamSpot'),
  goodVtxNdof     = cms.double(4),
  goodVtxZ        = cms.double(24),
  ## rho #######################################
  srcCaloRho      = cms.InputTag('fixedGridRhoFastjetAllCalo'),
  srcPFRho        = cms.InputTag('fixedGridRhoFastjetAll'),
  srcPULabel      = cms.untracked.InputTag('slimmedAddPileupInfo'),
  ## preselection cuts #########################
  maxY            = cms.double(5.0),
  minPFPt         = cms.double(15.0),
  minPFPtThirdJet = cms.double(5.0),
  minJJMass       = cms.double(0.0),
  minNPFJets      = cms.int32(1),
  minGenPt        = cms.untracked.double(20),
  isMCarlo        = cms.untracked.bool(False),
  useGenInfo      = cms.untracked.bool(False),
  AK4             = cms.untracked.bool(True),      
  ## trigger ###################################
  printTriggerMenu= cms.untracked.bool(True),
  processName     = cms.untracked.string('HLT'),
  triggerName     = triggers,
  triggerFollow   = follows,
  triggerResults  = cms.untracked.InputTag("TriggerResults","","HLT"),
  prescales       = cms.InputTag("patTrigger"),
  prescalesL1Min  = cms.InputTag("patTrigger","l1min"), 
  prescalesL1Max  = cms.InputTag("patTrigger","l1max"), 
  #triggerObjects  = cms.InputTag("slimmedPatTrigger"),
  ## jec services ##############################
  EventInfo       = cms.untracked.InputTag("generator"),
  GenParticles    = cms.untracked.InputTag("genparticles"),
  jetFlavourInfos = cms.untracked.InputTag("genJetFlavourInfos"),
  saveWeights     = cms.bool(False),                      
)

process.goodVertices = cms.EDFilter("VertexSelector",
  filter = cms.bool(False),
  src = cms.InputTag("offlineSlimmedPrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
)

#Try scheduled processs
process.path = cms.Path(process.goodVertices*
                        process.ak4)

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Output and Log
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.options.allowUnscheduled = cms.untracked.bool(True)
