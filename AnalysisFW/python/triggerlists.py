import FWCore.ParameterSet.Config as cms
# The trigger lists for each era follow this convention: AK4 - AK8 (incl. PFHT) - ZB
# Nesting: year - era - ak4/ak8/zb

trglist = {
  '16' : { 
    # This does not pass the DATACERT and AK8 triggers are missing
    "BI" : {
      'ak4' : cms.vstring("HLT_PFJet40_v4","HLT_PFJet60_v4","HLT_PFJet80_v4","HLT_PFJet140_v4","HLT_PFJet200_v4","HLT_PFJet260_v4","HLT_PFJet320_v4","HLT_PFJet400_v4","HLT_PFJet450_v4","HLT_PFJet500_v4"),
      'ak8' : cms.vstring(),
      'zb'  : cms.vstring("HLT_PFJet40_v4","HLT_PFJet60_v4","HLT_ZeroBias_v2")
    },
    "BII" : {
      'ak4' : cms.vstring("HLT_PFJet40_v4","HLT_PFJet40_v5","HLT_PFJet60_v4","HLT_PFJet60_v5","HLT_PFJet80_v4","HLT_PFJet80_v5","HLT_PFJet140_v4","HLT_PFJet140_v5","HLT_PFJet200_v4","HLT_PFJet200_v5","HLT_PFJet260_v4","HLT_PFJet260_v5","HLT_PFJet320_v4","HLT_PFJet320_v5","HLT_PFJet400_v4","HLT_PFJet400_v5","HLT_PFJet450_v4","HLT_PFJet450_v5","HLT_PFJet500_v4","HLT_PFJet500_v5"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v1","HLT_AK8PFJet60_v1","HLT_AK8PFJet80_v1","HLT_AK8PFJet140_v1","HLT_AK8PFJet200_v1","HLT_AK8PFJet260_v1","HLT_AK8PFJet320_v1","HLT_AK8PFJet400_v1","HLT_AK8PFJet450_v1","HLT_AK8PFJet500_v1"),
      'zb' : cms.vstring("HLT_PFJet40_v4","HLT_PFJet40_v5","HLT_ZeroBias_v2","HLT_ZeroBias_v3")
    },
    "C" : {
      'ak4' : cms.vstring("HLT_PFJet40_v5","HLT_PFJet40_v6","HLT_PFJet60_v5","HLT_PFJet60_v6","HLT_PFJet80_v5","HLT_PFJet140_v5","HLT_PFJet200_v5","HLT_PFJet260_v5","HLT_PFJet320_v5","HLT_PFJet400_v5","HLT_PFJet450_v5","HLT_PFJet500_v5"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v1","HLT_AK8PFJet40_v2","HLT_AK8PFJet60_v1","HLT_AK8PFJet80_v1","HLT_AK8PFJet140_v1","HLT_AK8PFJet200_v1","HLT_AK8PFJet260_v1","HLT_AK8PFJet320_v1","HLT_AK8PFJet400_v1","HLT_AK8PFJet450_v1","HLT_AK8PFJet500_v1"),
      'zb' : cms.vstring("HLT_PFJet40_v5","HLT_PFJet40_v6","HLT_ZeroBias_v3","HLT_ZeroBias_v4")
    },
    "D" : {
      'ak4' : cms.vstring("HLT_PFJet40_v6","HLT_PFJet60_v6","HLT_PFJet80_v5","HLT_PFJet140_v5","HLT_PFJet200_v5","HLT_PFJet260_v5","HLT_PFJet320_v5","HLT_PFJet400_v5","HLT_PFJet450_v5","HLT_PFJet500_v5"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v2","HLT_AK8PFJet60_v1","HLT_AK8PFJet80_v1","HLT_AK8PFJet140_v1","HLT_AK8PFJet200_v1","HLT_AK8PFJet260_v1","HLT_AK8PFJet320_v1","HLT_AK8PFJet400_v1","HLT_AK8PFJet450_v1","HLT_AK8PFJet500_v1"),
      'zb' : cms.vstring("HLT_PFJet40_v6","HLT_ZeroBias_v4")
    },
    "E" : {
      'ak4' : cms.vstring("HLT_PFJet40_v6","HLT_PFJet60_v6","HLT_PFJet80_v5","HLT_PFJet140_v5","HLT_PFJet200_v5","HLT_PFJet260_v5","HLT_PFJet320_v5","HLT_PFJet400_v5","HLT_PFJet450_v5","HLT_PFJet500_v5"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v2","HLT_AK8PFJet60_v1","HLT_AK8PFJet80_v1","HLT_AK8PFJet140_v1","HLT_AK8PFJet200_v1","HLT_AK8PFJet260_v1","HLT_AK8PFJet320_v1","HLT_AK8PFJet400_v1","HLT_AK8PFJet450_v1","HLT_AK8PFJet500_v1"),
      'zb' : cms.vstring("HLT_PFJet40_v6","HLT_ZeroBias_v4")
    },
    "Fe" : {
      'ak4' : cms.vstring("HLT_PFJet40_v6","HLT_PFJet40_v7","HLT_PFJet60_v6","HLT_PFJet60_v7","HLT_PFJet80_v5","HLT_PFJet80_v6","HLT_PFJet140_v5","HLT_PFJet140_v6","HLT_PFJet200_v5","HLT_PFJet200_v6","HLT_PFJet260_v5","HLT_PFJet260_v7","HLT_PFJet320_v5","HLT_PFJet320_v7","HLT_PFJet400_v5","HLT_PFJet400_v7","HLT_PFJet450_v5","HLT_PFJet450_v7","HLT_PFJet500_v5","HLT_PFJet500_v7"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v2","HLT_AK8PFJet40_v3","HLT_AK8PFJet60_v1","HLT_AK8PFJet60_v2","HLT_AK8PFJet80_v1","HLT_AK8PFJet80_v2","HLT_AK8PFJet140_v1","HLT_AK8PFJet140_v2","HLT_AK8PFJet200_v1","HLT_AK8PFJet200_v2","HLT_AK8PFJet260_v1","HLT_AK8PFJet260_v3","HLT_AK8PFJet320_v1","HLT_AK8PFJet320_v3","HLT_AK8PFJet400_v1","HLT_AK8PFJet400_v3","HLT_AK8PFJet450_v1","HLT_AK8PFJet450_v3","HLT_AK8PFJet500_v1","HLT_AK8PFJet500_v3"),
      'zb' : cms.vstring("HLT_PFJet40_v6","HLT_PFJet40_v7","HLT_ZeroBias_v4")
    },
    'Fl' : {
      'ak4' : cms.vstring("HLT_PFJet40_v7","HLT_PFJet60_v7","HLT_PFJet80_v6","HLT_PFJet140_v6","HLT_PFJet200_v6","HLT_PFJet260_v7","HLT_PFJet320_v7","HLT_PFJet400_v7","HLT_PFJet450_v7","HLT_PFJet500_v7"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v3","HLT_AK8PFJet60_v2","HLT_AK8PFJet80_v2","HLT_AK8PFJet140_v2","HLT_AK8PFJet200_v2","HLT_AK8PFJet260_v3","HLT_AK8PFJet320_v3","HLT_AK8PFJet400_v3","HLT_AK8PFJet450_v3","HLT_AK8PFJet500_v3"),
      'zb' : cms.vstring("HLT_PFJet40_v7","HLT_ZeroBias_v4")
    },
    'G' : {
      'ak4' : cms.vstring("HLT_PFJet40_v7","HLT_PFJet60_v7","HLT_PFJet80_v6","HLT_PFJet140_v6","HLT_PFJet200_v6","HLT_PFJet260_v7","HLT_PFJet320_v7","HLT_PFJet400_v7","HLT_PFJet450_v7","HLT_PFJet500_v7"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v3","HLT_AK8PFJet60_v2","HLT_AK8PFJet80_v2","HLT_AK8PFJet140_v2","HLT_AK8PFJet200_v2","HLT_AK8PFJet260_v3","HLT_AK8PFJet320_v3","HLT_AK8PFJet400_v3","HLT_AK8PFJet450_v3","HLT_AK8PFJet500_v3"),
      'zb' : cms.vstring("HLT_PFJet40_v7","HLT_ZeroBias_v4")
    },
    'H' : {
      'ak4' : cms.vstring("HLT_PFJet40_v9","HLT_PFJet60_v9","HLT_PFJet80_v8","HLT_PFJet140_v8","HLT_PFJet200_v8","HLT_PFJet260_v9","HLT_PFJet320_v9","HLT_PFJet400_v9","HLT_PFJet450_v9","HLT_PFJet500_v9"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v5","HLT_AK8PFJet60_v4","HLT_AK8PFJet80_v4","HLT_AK8PFJet140_v4","HLT_AK8PFJet200_v4","HLT_AK8PFJet260_v5","HLT_AK8PFJet320_v5","HLT_AK8PFJet400_TrimMass30_v1","HLT_AK8PFJet400_v5","HLT_AK8PFJet450_v5","HLT_AK8PFJet500_v5"),
      'zb' : cms.vstring("HLT_PFJet40_v9","HLT_ZeroBias_v4")
    }
  },
  '17' : {
    'B' : {
      'ak4' : cms.vstring('HLT_PFJet40_v10','HLT_PFJet40_v11','HLT_PFJet40_v12','HLT_PFJet60_v10','HLT_PFJet60_v11','HLT_PFJet60_v12','HLT_PFJet80_v10','HLT_PFJet80_v11','HLT_PFJet80_v9','HLT_PFJet140_v10','HLT_PFJet140_v9','HLT_PFJet200_v10','HLT_PFJet200_v9','HLT_PFJet260_v10','HLT_PFJet260_v11','HLT_PFJet320_v10','HLT_PFJet320_v11','HLT_PFJet400_v10','HLT_PFJet400_v11','HLT_PFJet450_v10','HLT_PFJet450_v11','HLT_PFJet450_v12','HLT_PFJet500_v10','HLT_PFJet500_v11','HLT_PFJet500_v12',),
      'ak8' : cms.vstring(),
      'zb' : cms.vstring()
    }
  },
  '18' : {
    'A' : {
      'ak4' : cms.vstring("HLT_PFJet40_v19","HLT_PFJet40_v20","HLT_PFJet40_v21","HLT_PFJet60_v19","HLT_PFJet60_v20","HLT_PFJet60_v21","HLT_PFJet80_v18","HLT_PFJet80_v19","HLT_PFJet80_v20","HLT_PFJet140_v17","HLT_PFJet140_v18","HLT_PFJet140_v19","HLT_PFJet200_v17","HLT_PFJet200_v18","HLT_PFJet200_v19","HLT_PFJet260_v18","HLT_PFJet260_v19","HLT_PFJet260_v20","HLT_PFJet320_v18","HLT_PFJet320_v19","HLT_PFJet320_v20","HLT_PFJet400_v18","HLT_PFJet400_v19","HLT_PFJet400_v20","HLT_PFJet450_v19","HLT_PFJet450_v20","HLT_PFJet450_v21","HLT_PFJet500_v19","HLT_PFJet500_v20","HLT_PFJet500_v21"),
      'ak8' : cms.vstring(),
      'zb' : cms.vstring()
    },
    'B' : {
      'ak4' : cms.vstring("HLT_PFJet40_v21","HLT_PFJet140_v19","HLT_PFJet60_v21","HLT_PFJet80_v20","HLT_PFJet200_v19","HLT_PFJet260_v20","HLT_PFJet320_v20","HLT_PFJet400_v20","HLT_PFJet450_v21","HLT_PFJet500_v21"),
      'ak8' : cms.vstring(),
      'zb' : cms.vstring()
    }
  }
}
