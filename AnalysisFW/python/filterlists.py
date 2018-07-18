import FWCore.ParameterSet.Config as cms

fltlist = {
  '16' : {
    'dt' : cms.vstring(
      "Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter",
      "Flag_goodVertices","Flag_eeBadScFilter","Flag_globalTightHalo2016Filter",
      "Flag_BadChargedCandidateFilter","Flag_BadPFMuonFilter"
    ),
    'mc' : cms.vstring(
      "Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter",
      "Flag_goodVertices","Flag_globalTightHalo2016Filter",
      "Flag_BadChargedCandidateFilter","Flag_BadPFMuonFilter"
    )
  },
  '17' : {
    'dt' : cms.vstring(
     "Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter",
     "Flag_goodVertices","Flag_eeBadScFilter","Flag_ecalBadCalibFilter",
     "Flag_globalTightHalo2016Filter","Flag_BadChargedCandidateFilter","Flag_BadPFMuonFilter"
    ),
    'mc' : cms.vstring(
     "Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter",
     "Flag_goodVertices","Flag_ecalBadCalibFilter",
     "Flag_globalTightHalo2016Filter","Flag_BadChargedCandidateFilter","Flag_BadPFMuonFilter"
    )
  },
  '18' : {
    'dt' : cms.vstring(
     "Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter",
     "Flag_goodVertices","Flag_eeBadScFilter","Flag_ecalBadCalibFilter",
     "Flag_globalTightHalo2016Filter","Flag_BadChargedCandidateFilter","Flag_BadPFMuonFilter"
    ),
    'mc' : cms.vstring()
  }
}
