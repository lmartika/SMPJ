# -*- coding: utf-8 -*-

import FWCore.ParameterSet.Config as cms
from os import *

from RecoJets.Configuration.RecoPFJets_cff import ak4PFJets, ak4PFJetsCHS
from RecoJets.Configuration.RecoGenJets_cff import ak4GenJets
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
from RecoJets.JetProducers.PFJetParameters_cfi import *
from RecoJets.JetProducers.GenJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff import *
from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.patSequences_cff import *
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from RecoJets.JetProducers.QGTagger_cfi import QGTagger
from JetMETCorrections.Type1MET.correctionTermsPfMetType0PFCandidate_cff import *
from JetMETCorrections.Type1MET.correctedMet_cff import pfMetT0pcTxy
from PhysicsTools.JetMCAlgos.HadronAndPartonSelector_cfi import selectedHadronsAndPartons
from PhysicsTools.JetMCAlgos.AK4PFJetsMCFlavourInfos_cfi import ak4JetFlavourInfos
from Configuration.AlCa.GlobalTag import GlobalTag

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

process.GlobalTag.globaltag = "94X_mc2017_realistic_v12"

# srcJets could be reco::PFJetCollection or pat::JetCollection (both AOD and miniAOD)process.load('RecoJets.JetProducers.QGTagger_cfi')
# other options for jetsLabel: see https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion
process.load('RecoJets.JetProducers.QGTagger_cfi')
process.QGTagger.srcJets = cms.InputTag('ak4PFJetsCHS')
process.QGTagger.jetsLabel  = cms.string('QGL_AK4PFchs') 

##-------------------- Import the JEC services -----------------------
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Services
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

process.load('FWCore.MessageLogger.MessageLogger_cfi')
#process.MessageLogger = cms.Service("MessageLogger",
#        destinations = cms.untracked.vstring(                           #1
#                'myOutputFile'                                          #2
#        ),
#        myOutputFile = cms.untracked.PSet(                              #3
#                threshold = cms.untracked.string( 'WARNING' )           #4
#        ),
#)                                                                       #5
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.load('CommonTools.UtilAlgos.TFileService_cfi')
process.TFileService.fileName=cms.string('DATA.root')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Input
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

inFiles = cms.untracked.vstring( '/store/mc/RunIIFall17DRPremix/QCD_Pt-15to7000_TuneCUETHS1_Flat_13TeV_herwigpp/AODSIM/94X_mc2017_realistic_v10-v1/50000/1461FC6C-A0EC-E711-9E52-0CC47A4D7658.root' )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
process.source = cms.Source("PoolSource", fileNames = inFiles )

## Modified version of jetToolBox from https://github.com/cms-jet/jetToolbox
## Options for PUMethod: Puppi, CS, SK, CHS
def jetToolbox( proc, jetType, jetSequence ):
  bTagDiscriminators = [#'trackCountingHighEffBJetTags',
                        #'trackCountingHighPurBJetTags',
                        #'pfTrackCountingHighEffBJetTags',
                        #'pfTrackCountingHighPurBJetTags',
                        #'softPFMuonByIP3dBJetTags',
                        #'softPFElectronByIP3dBJetTags',
                        #'softPFMuonBJetTags',
                        #'softPFElectronBJetTags',
                        #'simpleSecondaryVertexHighEffBJetTags',
                        #'simpleSecondaryVertexHighPurBJetTags',
                        #'pfSimpleSecondaryVertexHighEffBJetTags',
                        #'pfSimpleSecondaryVertexHighPurBJetTags',
                        #'combinedSecondaryVertexV2BJetTags',
                        #'pfDeepCSVJetTags:probc',
                        #'pfDeepCSVJetTags:probb',
                        #'pfDeepCSVJetTags:probbb',
                        #'pfDeepCSVJetTags:probcc',
                        #'pfDeepCSVJetTags:probudsg', 
                        #'pfDeepCMVAJetTags:probc',
                        #'pfDeepCMVAJetTags:probb',
                        #'pfDeepCMVAJetTags:probbb',
                        #'pfDeepCMVAJetTags:probcc', 
                        #'pfDeepCMVAJetTags:probudsg',
                        #'pfNegativeDeepCSVJetTags:probb',
                        #'pfNegativeDeepCSVJetTags:probc', 
                        #'pfNegativeDeepCSVJetTags:probcc',
                        #'pfNegativeDeepCSVJetTags:probbb',
                        #'pfNegativeDeepCSVJetTags:probudsg',
                        #'pfPositiveDeepCSVJetTags:probb',
                        #'pfPositiveDeepCSVJetTags:probc',
                        #'pfPositiveDeepCSVJetTags:probcc',
                        #'pfPositiveDeepCSVJetTags:probbb', 
                        #'pfPositiveDeepCSVJetTags:probudsg', 
                        #'pfNegativeDeepCMVAJetTags:probb',
                        #'pfNegativeDeepCMVAJetTags:probc',
                        #'pfNegativeDeepCMVAJetTags:probcc',
                        #'pfNegativeDeepCMVAJetTags:probbb',
                        #'pfNegativeDeepCMVAJetTags:probudsg',
                        #'pfPositiveDeepCMVAJetTags:probb',
                        #'pfPositiveDeepCMVAJetTags:probc',
                        #'pfPositiveDeepCMVAJetTags:probcc',
                        #'pfPositiveDeepCMVAJetTags:probbb',
                        #'pfPositiveDeepCMVAJetTags:probudsg',
                        #'deepFlavourJetTags:probb',
                        #'deepFlavourJetTags:probc',
                        #'deepFlavourJetTags:probudsg',
                        #'deepFlavourJetTags:probbb',
                        #'deepFlavourJetTags:probcc',
                        #'negativeDeepFlavourJetTags:probb',
                        #'negativeDeepFlavourJetTags:probc',
                        #'negativeDeepFlavourJetTags:probudsg',
                        #'negativeDeepFlavourJetTags:probbb',
                        #'negativeDeepFlavourJetTags:probcc',
                        #'positiveDeepFlavourJetTags:probb',
                        #'positiveDeepFlavourJetTags:probc',
                        #'positiveDeepFlavourJetTags:probudsg',
                        #'positiveDeepFlavourJetTags:probbb',
                        #'poositiveDeepFlavourJetTags:probcc',
                        'pfTrackCountingHighEffBJetTags',
                        'pfTrackCountingHighPurBJetTags',
                        'pfJetProbabilityBJetTags',
                        'pfJetBProbabilityBJetTags',
                        'pfSimpleSecondaryVertexHighEffBJetTags',
                        'pfSimpleSecondaryVertexHighPurBJetTags',
                        'pfCombinedCvsLJetTags',
                        'pfCombinedCvsBJetTags',
                        'pfBoostedDoubleSecondaryVertexAK8BJetTags',
                        'pfCombinedSecondaryVertexV2BJetTags',
                        'pfPositiveCombinedSecondaryVertexV2BJetTags',  #implemented
                        'pfNegativeCombinedSecondaryVertexV2BJetTags',  #implemented
                        'pfCombinedInclusiveSecondaryVertexV2BJetTags', #implemented
                        'pfCombinedMVAV2BJetTags',                      #implemented
                        'pfJetProbabilityBJetTags']                     #implemented

  print '|---- jetToolbox: Initialyzing collection...'
  algorithm='AntiKt' # CambridgeAachen' , 'Kt'
  size = jetType[-1:] #[-1:] takes the last char from string 'akX'
  jetSize = float('0.'+jetType[-1:])
  jetALGO = jetType.upper()
  jetalgo = jetType.lower()
  PUMethod='CHS'
  jetNAME = jetALGO+'PF'+PUMethod 
  jetname = jetalgo+'PFJets'+PUMethod 
  gjetname = jetalgo+'GenJetsNoNu'
  print 'Running processes with: '+jetNAME+' jet algorithm with radius parameter '+str(jetSize)

  JEC = ( 'AK'+size+'PF'+PUMethod.lower(), ['L1FastJet', 'L2Relative', 'L3Absolute'] , 'None')
  print '|---- jetToolBox: Applying these corrections: '+str(JEC)

  #################################################################################
  ####### Toolbox start
  #################################################################################
  
  jetSeq = cms.Sequence()
  
  # Creating GenJets
  proc.load('RecoJets.Configuration.GenJetParticles_cff')
  setattr( proc, gjetname, ak4GenJets.clone( src = 'genParticlesForJetsNoNu', rParam = jetSize, jetAlgorithm = algorithm ) )
  jetSeq += getattr(proc, gjetname )
  
  # Creating AK4PFJetsCHS
  proc.load('CommonTools.ParticleFlow.pfNoPileUpJME_cff')
  setattr( proc, jetname, ak4PFJetsCHS.clone(src=cms.InputTag('pfNoPileUpJME'),doAreaFastjet=True,rParam=jetSize,jetAlgorithm=algorithm) )
  jetSeq += getattr(proc, jetname )
  
  ####  Creating PATjets
  addJetCollection( proc,
                    labelName = jetNAME,
                    jetSource = cms.InputTag(jetname),
                    algo = jetalgo,
                    rParam = jetSize,
                    btagDiscriminators = bTagDiscriminators,
                    jetCorrections =  JEC,
                    pfCandidates = cms.InputTag( 'particleFlow' ), #redundant (default)
                    svSource = cms.InputTag('inclusiveCandidateSecondaryVertices'), # redundant (default)
                    pvSource = cms.InputTag( 'offlinePrimaryVertices' ), # redundant (default) 
                    getJetMCFlavour = True,  # redundant (default)
                    genJetCollection = cms.InputTag(gjetname),
                    jetTrackAssociation = True )
  getattr(proc,'patJets'+jetNAME).addTagInfos = cms.bool(True)
  getattr(proc,'patJets'+jetNAME).addAssociatedTracks = cms.bool(True)

  setattr( proc, 'QGTagger'+jetNAME, QGTagger.clone(srcJets=cms.InputTag(jetname),jetsLabel=cms.string('QGL_AK4PFchs')) )
  getattr( proc, 'patJets'+jetNAME).userData.userFloats.src += ['QGTagger'+jetNAME+':qgLikelihood']
  jetSeq += getattr(proc, 'QGTagger'+jetNAME )

  getattr(proc, 'patJetPartons').particles = cms.InputTag('genParticles')
  # This is already done in addJetCollection, but I worry that the changes done to patJets do not get updated to     selectedPatJets
  setattr(proc, 'selectedPatJets'+jetNAME, selectedPatJets.clone(src='patJets'+jetNAME) )
  # return
  setattr(proc, jetSequence, jetSeq)

jetToolbox( process, 'ak4', 'ak4JetSubs' )

# PAT Layer 1
process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")
process.load("PhysicsTools.PatAlgos.patSequences_cff")

# Tagging info
genJetCollection = 'ak4GenJetsNoNu'
genParticleCollection = 'genParticles'
process.selectedHadronsAndPartons = selectedHadronsAndPartons.clone( particles = genParticleCollection, partonMode = cms.string("Herwig++") )
process.genJetFlavourInfos = ak4JetFlavourInfos.clone( jets = genJetCollection )
#You need to create another collection to give the physics definition 
process.genJetFlavourInfosPhysicsDef = ak4JetFlavourInfos.clone( jets = genJetCollection, partons = cms.InputTag("selectedHadronsAndPartons","physicsPartons"))

process.ak4 =  cms.EDAnalyzer( 'ProcessedTreeProducerBTag',
                               ## jet collections ###########################
                               pfjetschs                 = cms.InputTag('selectedPatJetsAK4PFCHS'),
                               pfpujetid                 = cms.string('AK4PFpileupJetIdEvaluator:fullDiscriminant'),
                               pfchsjetpuid              = cms.string('AK4PFCHSpileupJetIdEvaluator:fullDiscriminant'),
                               ## MET collection ####
                               pfmet                     = cms.InputTag('patMETs'),
                               ## Gen jets
                               genjets                   = cms.untracked.InputTag('ak4GenJetsNoNu'),
                               ## database entry for the uncertainties ######
                               PFPayloadNameCHS          = cms.string('AK4PFchs'),
                               jecUncSrcCHS              = cms.string(''),
                               jecUncSrcNames            = cms.vstring(''),
                               ## set the conditions for good Vtx counting ##
                               offlineVertices           = cms.InputTag('offlinePrimaryVertices'),
                               beamSpot                  = cms.InputTag('offlineBeamSpot'),
                               goodVtxNdof               = cms.double(4),
                               goodVtxZ                  = cms.double(24),
                               ## rho #######################################
                               srcCaloRho                = cms.InputTag('fixedGridRhoFastjetAllCalo'),
                               srcPFRho                  = cms.InputTag('fixedGridRhoFastjetAll'),
                               srcPULabel                = cms.untracked.InputTag('addPileupInfo'),
                               ## preselection cuts #########################
                               maxY                      = cms.double(5.0),
                               minPFPt                   = cms.double(10.0),
                               minPFPtThirdJet           = cms.double(5.0),
                               minNPFJets                = cms.int32(1),
                               minGenPt                  = cms.untracked.double(20.0),
                               minJJMass                 = cms.double(-1),
                               isMCarlo                  = cms.untracked.bool(True),
                               useGenInfo                = cms.untracked.bool(True),
                               AK4                       = cms.untracked.bool(True),
                               ## trigger ###################################
                               printTriggerMenu          = cms.untracked.bool(False),
                               processName               = cms.string('HLT'),
                               triggerName               = cms.vstring(''), 
                               triggerResults  = cms.InputTag("TriggerResults","","HLT"),
                               triggerEvent    = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
                               ## gen services ##############################
                               EventInfo       = cms.InputTag("generator"),
                               GenParticles    = cms.InputTag("genParticles"),
                               jetFlavourInfos = cms.InputTag("genJetFlavourInfos"),
                               jetFlavourInfosPhysicsDef = cms.InputTag("genJetFlavourInfosPhysicsDef") ) 

#MET Filters
process.load('RecoMET.METFilters.primaryVertexFilter_cfi')
process.load('RecoMET.METFilters.globalTightHalo2016Filter_cfi')
process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')
process.load('RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi')
process.load('RecoMET.METFilters.BadChargedCandidateFilter_cfi')
process.load('RecoMET.METFilters.BadPFMuonFilter_cfi')
#process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.load('RecoMET.METFilters.ecalBadCalibFilter_cfi')

process.allMetFilterPaths=cms.Sequence( process.primaryVertexFilter*
                                        process.globalTightHalo2016Filter*
                                        process.HBHENoiseFilter*
                                        process.HBHENoiseIsoFilter*
                                        process.EcalDeadCellTriggerPrimitiveFilter*
                                        process.BadPFMuonFilter*
                                        process.BadChargedCandidateFilter*
                                        #process.eeBadScFilter* # Not in MC
                                        process.ecalBadCalibFilter )

##Type1 patMET Producer
process.load("JetMETCorrections.Type1MET.correctionTermsPfMetType0PFCandidate_cff")
process.load("JetMETCorrections.Type1MET.correctedMet_cff")
process.load('PhysicsTools.PatAlgos.producersLayer1.metProducer_cfi')
process.patMETs.addGenMET = cms.bool(False)
## Choose to use Type0 MET instead
process.patMETs.metSource = cms.InputTag("pfMetT0pc")

process.allMetPaths=cms.Sequence( process.correctionTermsPfMetType0PFCandidate*
                                  process.pfMetT0pc*
                                  process.patMETs )

process.allBTagPaths=cms.Sequence( process.pfImpactParameterTagInfosAK4PFCHS*
                                   process.pfImpactParameterAK8TagInfosAK4PFCHS*
                                   process.pfSecondaryVertexTagInfosAK4PFCHS*
                                   process.pfInclusiveSecondaryVertexFinderCvsLTagInfosAK4PFCHS*
                                   process.softPFMuonsTagInfosAK4PFCHS*
                                   process.softPFElectronsTagInfosAK4PFCHS*
                                   process.pfSecondaryVertexNegativeTagInfosAK4PFCHS*
                                   process.pfInclusiveSecondaryVertexFinderTagInfosAK4PFCHS*
                                   process.pfInclusiveSecondaryVertexFinderAK8TagInfosAK4PFCHS*
                                   process.pfBoostedDoubleSVAK8TagInfosAK4PFCHS*
                                   process.pfCombinedMVAV2BJetTagsAK4PFCHS*
                                   process.pfCombinedInclusiveSecondaryVertexV2BJetTagsAK4PFCHS*
                                   process.pfNegativeCombinedSecondaryVertexV2BJetTagsAK4PFCHS*
                                   process.pfPositiveCombinedSecondaryVertexV2BJetTagsAK4PFCHS*
                                   process.pfCombinedSecondaryVertexV2BJetTagsAK4PFCHS*
                                   process.pfBoostedDoubleSecondaryVertexAK8BJetTagsAK4PFCHS*
                                   process.pfCombinedCvsBJetTagsAK4PFCHS*
                                   process.pfCombinedCvsLJetTagsAK4PFCHS*
                                   process.pfSimpleSecondaryVertexHighPurBJetTagsAK4PFCHS*
                                   process.pfSimpleSecondaryVertexHighEffBJetTagsAK4PFCHS*
                                   process.pfJetBProbabilityBJetTagsAK4PFCHS*
                                   process.pfJetProbabilityBJetTagsAK4PFCHS*
                                   process.pfTrackCountingHighPurBJetTagsAK4PFCHS*
                                   process.pfTrackCountingHighEffBJetTagsAK4PFCHS )

process.allGenPaths=cms.Sequence( process.patJetPartons*
                                  process.selectedHadronsAndPartons*
                                  process.genJetFlavourInfos*
                                  process.genJetFlavourInfosPhysicsDef*
                                  process.patJetGenJetMatchAK4PFCHS*
                                  process.patJetPartonMatchAK4PFCHS*
                                  process.patJetFlavourAssociationAK4PFCHS ) 

process.allJetPaths=cms.Sequence( process.patJetChargeAK4PFCHS*
                                  process.patJetCorrFactorsAK4PFCHS*
                                  process.patJetsAK4PFCHS*
                                  process.selectedPatJetsAK4PFCHS )

#Try scheduled processs
process.path = cms.Path( process.allFilterPaths*
                         process.allMetPaths*
                         process.QGTaggerAK4PFCHS*
                         process.QGTagger*
                         process.jetTracksAssociatorAtVertexAK4PFCHS*
                         process.allBTagPaths*
                         process.allGenPaths*
                         process.allJetPaths*
                         process.ak4 )

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Output and Log
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.options.allowUnscheduled = cms.untracked.bool(True)