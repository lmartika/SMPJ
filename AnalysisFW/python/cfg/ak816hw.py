# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms

from RecoJets.Configuration.RecoGenJets_cff import ak4GenJets
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
from RecoJets.JetProducers.PFJetParameters_cfi import *
from RecoJets.JetProducers.GenJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *
from RecoJets.JetProducers.QGTagger_cfi import QGTagger
from PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff import *
from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.patSequences_cff import *
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.JetMCAlgos.AK4PFJetsMCFlavourInfos_cfi import ak4JetFlavourInfos
import sys

from triggerlists import *
from filterlists import *
from filelists import *


process = cms.Process("Ntuplizer")
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Conditions
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("RecoJets.Configuration.GenJetParticles_cff")
process.load("RecoJets.Configuration.RecoGenJets_cff")
process.load("RecoJets.JetProducers.TrackJetParameters_cfi")
process.load("RecoJets.JetProducers.PileupJetIDParams_cfi")
process.load("PhysicsTools.PatAlgos.patSequences_cff")

##-------------------- Import the JEC services -----------------------
process.load("JetMETCorrections.Configuration.DefaultJEC_cff")

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Input
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

process.GlobalTag.globaltag = "80X_mcRun2_asymptotic_2016_TrancheIV_v6"

triggers=cms.vstring()

follows=cms.vstring()

filters=fltlist["16"]["mc"]

inFiles=QCD16Mor17HS1
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.source = cms.Source("PoolSource", fileNames = inFiles )

process.load("CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi")
process.load("CommonTools.RecoAlgos.HBHENoiseFilter_cfi")

process.HBHENoiseFilterResultProducerNoMinZ = process.HBHENoiseFilterResultProducer.clone(minZeros = cms.int32(99999))

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Services
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = cms.untracked.string("ERROR")
process.MessageLogger = cms.Service("MessageLogger",
        destinations = cms.untracked.vstring(
                "errors"
        ),
        errors = cms.untracked.PSet(
                threshold = cms.untracked.string("ERROR")
        ),
)
process.load("CommonTools.UtilAlgos.TFileService_cfi")
process.TFileService.fileName=cms.string("DATA.root")
jetname='slimmedJetsAK8'
gjetname='slimmedGenJetsAK8'

process.load("PhysicsTools.JetMCAlgos.HadronAndPartonSelector_cfi")
process.selectedHadronsAndPartons.particles = "prunedGenParticles"
process.selectedHadronsAndPartons.partonMode = cms.string("Auto")
process.selectedHadronsAndPartons.src = "generator"
process.physDefHadronsAndPartons = process.selectedHadronsAndPartons.clone( fullChainPhysPartons = cms.bool(False) )

process.jetFlavs = ak4JetFlavourInfos.clone( jets = gjetname,
                                             partons = cms.InputTag("selectedHadronsAndPartons","algorithmicPartons") )
process.jetFlavsPD = process.jetFlavs.clone( partons = cms.InputTag("physDefHadronsAndPartons","physicsPartons") )
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.ak8 =  cms.EDAnalyzer('ProcessedTreeProducerBTag',
  ## jet collections ###########################
  pfjetschs       = cms.InputTag(jetname),
  pfchsjetpuid    = cms.string('pileupJetId:fullDiscriminant'),
  runYear         = cms.untracked.string('2016'),
  ## MET collection ####
  pfmetT1         = cms.InputTag('slimmedMETs'),
  genjets         = cms.untracked.InputTag(gjetname),
  ## database entry for the uncertainties ######
  PFPayloadName   = cms.string(''),
  jecUncSrc       = cms.untracked.string(''),
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
  maxEta          = cms.double(5.0),
  minPFPt         = cms.double(15.0),
  minPFPtThirdJet = cms.double(5.0),
  minNPFJets      = cms.uint32(0),
  minGenPt        = cms.untracked.double(15.0),
  isMCarlo        = cms.untracked.bool(True),
  useGenInfo      = cms.untracked.bool(True),
  AK4             = cms.untracked.bool(False),
  ZB              = cms.untracked.bool(False),
  ## trigger ###################################
  printTriggerMenu= cms.untracked.bool(False),
  processName     = cms.untracked.string('HLT'),
  filterName      = filters,
  triggerName     = triggers,
  triggerFollow   = follows,
  filterFlags     = cms.untracked.InputTag('TriggerResults','','RECO'),
  triggerResults  = cms.untracked.InputTag('TriggerResults','','HLT'),
  triggerHLTObjs  = cms.untracked.InputTag('selectedPatTrigger'),
  triggerL1Objs   = cms.untracked.InputTag('caloStage2Digis','Jet'),
  triggerL1HTObjs = cms.untracked.InputTag('caloStage2Digis','EtSum'),
  #triggerAllObjs = cms.untracked.InputTag('gtStage2Digis','GlobalAlgBlk'),
  prescales       = cms.InputTag('patTrigger'),
  prescalesL1Min  = cms.InputTag('patTrigger','l1min'),
  prescalesL1Max  = cms.InputTag('patTrigger','l1max'),
  ## gen ##############################
  EventInfo       = cms.untracked.InputTag('generator'),
  GenParticles    = cms.untracked.InputTag('prunedGenParticles'),
  jetFlavInfos    = cms.untracked.InputTag('jetFlavs'),
  jetFlavInfosPD  = cms.untracked.InputTag('jetFlavsPD'),
  mcType          = cms.untracked.int32(1),
  saveWeights     = cms.bool(False)
)

process.path = cms.Path(process.jetFlavs*
                        process.jetFlavsPD*
                        process.ak8)

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Output and Log
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.options.allowUnscheduled = cms.untracked.bool(True)
